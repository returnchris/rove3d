#include <cfloat>
#include "../rove.h"
#include "../core/utility.h"
#include "../core/thread_pool.h"
#include "mesh.h"
#include "scene.h"
#include "tree.h"
///
void SetupTree()
{
  LOG("Setting up tree.");
  memset(TREE, 0, sizeof(Tree));
  TREE->triangles.Setup(true);
  TREE->nodes.Setup(true);
  TREE->materials.Setup(false);
  TREE->emitter_indices.Setup(false);
  TREE->portal_indices.Setup(false);
  TREE->dirty_materials.dirty = false;
  TREE->dirty_emitters.dirty = false;
  TREE->dirty_portals.dirty = false;
  TREE->lsh_map = (uint32_t*)malloc(sizeof(uint32_t) * 1024);
  for (uint64_t i = 0; i < 1024; ++i) {
    TREE->lsh_map[i] = 0;
    for (uint64_t j = 0; j < 10; ++j) {
      TREE->lsh_map[i] += (uint32_t)(((i & (1u << j)) > 0)) << (j * 3);
    }
  }
  TREE->lsh_buffers[0].Setup(0, 0);
  TREE->lsh_buffers[1].Setup(0, 0);
  TREE->bounds_buffer.Setup(16, 0);
  TREE->visited_buffer.Setup(0, 0);
  TREE->node_buffer.Setup(0, 0);
  TREE->thread_sub_trees.Setup(0, 0); 
  TREE->thread_sub_trees.Emplace(ROVE.thread_count);
  for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
    TREE->thread_sub_trees[t].Setup(0, 0);
  }
  TREE->thread_node_buffers.Setup(0, 0); 
  TREE->thread_node_buffers.Emplace(ROVE.thread_count);
  for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
    TREE->thread_node_buffers[t].Setup(0, 0);
  }
}
///
void ShutdownTree()
{
  LOG("Shutting down tree.");
  TREE->triangles.Shutdown();
  TREE->nodes.Shutdown();
  TREE->materials.Shutdown();
  TREE->emitter_indices.Shutdown();
  TREE->portal_indices.Shutdown();
  if (TREE->lsh_map) {
    free(TREE->lsh_map);
  }
  TREE->lsh_buffers[0].Shutdown();
  TREE->lsh_buffers[1].Shutdown();
  TREE->bounds_buffer.Shutdown();
  TREE->visited_buffer.Shutdown();
  TREE->node_buffer.Shutdown();
  if (TREE->thread_sub_trees.m_data) {
    for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
      TREE->thread_sub_trees[t].Shutdown();
    }
    TREE->thread_sub_trees.Shutdown();
  }
  if (TREE->thread_node_buffers.m_data) {
    for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
      TREE->thread_node_buffers[t].Shutdown();
    }
    TREE->thread_node_buffers.Shutdown();
  }
  memset(TREE, 0, sizeof(Tree));
}
///
int32_t BuildMeshSubTrees(Mesh* mesh)
{
  mesh->sub_trees.Clear();
  uint32_t max_sub_tree_height;
  if (ROVE.optimization_level == ROVE_OPTIMIZATION_SPEED) {
    max_sub_tree_height = 8;
  } else if (ROVE.optimization_level == ROVE_OPTIMIZATION_BALANCED) {
    max_sub_tree_height = 6;
  } else {
    max_sub_tree_height = 24;
  }
  if (mesh->triangles.m_count == 1) {
    TwoNode* nodes = &TREE->nodes[mesh->node_offset];
    nodes[0].extra[0] = mesh->triangle_offset | TREE_LEAF_SUB_TREE;
    nodes[0].extra[1] = 0 | TREE_LEAF;
    F4A f4a_min[4], f4a_max[4];
    F4_STORE(TREE->bounds_buffer[0].min, f4a_min);
    F4_STORE(TREE->bounds_buffer[0].max, f4a_max);
    nodes[0].l_bounds[0] = nodes[0].r_bounds[0] = nodes[0].r_bounds[3] = f4a_min[0];
    nodes[0].l_bounds[1] = nodes[0].r_bounds[1] = nodes[0].r_bounds[4] = f4a_min[1];
    nodes[0].l_bounds[2] = nodes[0].r_bounds[2] = nodes[0].r_bounds[5] = f4a_min[2];
    nodes[0].l_bounds[3] = f4a_max[0];
    nodes[0].l_bounds[4] = f4a_max[1];
    nodes[0].l_bounds[5] = f4a_max[2];
    SubTree* sub_tree = mesh->sub_trees.Emplace(1);
    sub_tree->height = 0;
    sub_tree->mesh_local_offset = 0;
    sub_tree->mesh_triangle_offset = mesh->triangle_offset;
    COPY_BOUNDS(nodes[0].l_bounds, sub_tree->bounds);
    return true;
  }
  uint64_t* lsh = TREE->lsh_buffers[1].m_data;
  Range* ranges = (Range*)TREE->lsh_buffers[0].m_data;
  TREE->visited_buffer.Clear();
  TREE->visited_buffer.Emplace(mesh->triangles.m_count);
  for (uint32_t i = 0; i < mesh->triangles.m_count; ++i) {
    TREE->visited_buffer[i] = TREE_NOT_VISITED;
  }
  TREE->node_buffer.Clear();
  TREE->node_buffer.Emplace(mesh->triangles.m_count);
  TwoNode* temp_tree = TREE->node_buffer.m_data;
  PARALLEL(mesh->triangles.m_count,
  {
    Vector<SubTree>* my_sub_trees = &TREE->thread_sub_trees[thread_id];
    my_sub_trees->Clear();
    TwoNode* parent;
    for (uint32_t t = thread_begin; t < thread_end; ++t) {
      int32_t orphaned = false;
      uint32_t height = 0;
      Range range;
      range.r = t;
      range.l = t;
      uint32_t tri_index = (lsh[t] >> 32);
      F4A f4a_min[4];
      F4A f4a_max[4];
      F4_STORE(TREE->bounds_buffer[tri_index].min, f4a_min);
      F4_STORE(TREE->bounds_buffer[tri_index].max, f4a_max);
      float bounds[6];
      bounds[0] = f4a_min[0];
      bounds[1] = f4a_min[1];
      bounds[2] = f4a_min[2];
      bounds[3] = f4a_max[0];
      bounds[4] = f4a_max[1];
      bounds[5] = f4a_max[2];
      uint32_t node_index = t | TREE_LEAF;
      uint32_t raw_node_index = t;
      uint32_t level = 0;
      while (true) {
        uint32_t left = range.l - (range.l > 0);
        uint32_t right = range.r + (range.r < (mesh->triangles.m_count - 1));
        uint32_t delta_right = lsh[range.r] ^ lsh[right];
        uint32_t delta_left = lsh[range.l] ^ lsh[left];
        uint32_t diff_right = 0;
        uint32_t diff_left = 0;
        if (delta_right != 0) {
          BSR32(delta_right, diff_right);
        }
        if (delta_left != 0) {
          BSR32(delta_left, diff_left);
        }
        uint32_t level_index = raw_node_index >> level;
        int32_t heuristic;
        if ((ROVE.optimization_level == ROVE_OPTIMIZATION_SPEED) ||
            (ROVE.optimization_level == ROVE_OPTIMIZATION_BALANCED)) {
          heuristic = (diff_right < diff_left);
        } else {
          heuristic = !(level_index & 0x1);
        }
        ///
        if ((range.l == 0) || 
            ((range.r < (mesh->triangles.m_count - 1)) && heuristic)) {
          uint32_t tri_count = (range.r - range.l) + 1;
          if (tri_count <= (1 << TREE_LEAF_TRIANGLE_FACTOR)) {
            node_index = range.l | TREE_LEAF;
            node_index |= (tri_count - 1) << (31 - TREE_LEAF_TRIANGLE_FACTOR);
            height = 0;
          }
          uint32_t parent_index = range.r;
          raw_node_index = parent_index;
          ranges[parent_index].l = range.l;
          parent = &temp_tree[parent_index];
          if (!orphaned) {
            parent->child[0] = node_index;
            COPY_BOUNDS(bounds, parent->l_bounds);
          }
          uint32_t right_height = TREE_NOT_VISITED;
          uint32_t parent_mark = orphaned ? (height | TREE_ORPHAN) : height;
          int32_t first = 
            TREE->visited_buffer[parent_index].
              compare_exchange_strong(right_height, parent_mark);
          if (first) {
            break;
          } else {
            if (orphaned) {
              if (!(right_height & TREE_ORPHAN)) {
                node_index = parent->child[1];
                COPY_BOUNDS(parent->r_bounds, bounds);
                height = right_height;
                orphaned = false;
              }
            } else if (!(right_height & TREE_ORPHAN)) {
              if (((std::max)(height, right_height) > MAX_SUB_TREE_HEIGHT)) {
                SubTree* new_sub_trees = my_sub_trees->Emplace(2);
                new_sub_trees[0].temp_tree_index = node_index;
                new_sub_trees[0].height = height;
                new_sub_trees[0].mesh_triangle_offset = mesh->triangle_offset;
                COPY_BOUNDS(bounds, new_sub_trees[0].bounds);
                new_sub_trees[1].temp_tree_index = parent->child[1];
                new_sub_trees[1].height = right_height;
                new_sub_trees[1].mesh_triangle_offset = mesh->triangle_offset;
                COPY_BOUNDS(parent->r_bounds, new_sub_trees[1].bounds);
                orphaned = true;
              } else {
                MERGE_BOUNDS(bounds, parent->r_bounds, bounds);
                node_index = parent_index;
                height = (std::max)(height, right_height) + 1; 
              }
            }
            range.r = ranges[parent_index].r;
          }
        }
        ///
        else {
          uint32_t tri_count = (range.r - range.l) + 1;
          if (tri_count <= (1 << TREE_LEAF_TRIANGLE_FACTOR)) {
            node_index = range.l | TREE_LEAF;
            node_index |= (tri_count - 1) << (31 - TREE_LEAF_TRIANGLE_FACTOR);
            height = 0;
          }
          uint32_t parent_index = left;
          raw_node_index = parent_index;
          ranges[parent_index].r = range.r;
          parent = &temp_tree[parent_index];
          if (!orphaned) {
            parent->child[1] = node_index;
            COPY_BOUNDS(bounds, parent->r_bounds);
          }
          uint32_t left_height = TREE_NOT_VISITED;
          uint32_t parent_mark = orphaned ? (height | TREE_ORPHAN) : height;
          int32_t first =
            TREE->visited_buffer[parent_index].
              compare_exchange_strong(left_height, parent_mark);
          if (first) {
            break;
          } else {
            if (orphaned) {
              if (!(left_height & TREE_ORPHAN)) {
                node_index = parent->child[0];
                COPY_BOUNDS(parent->l_bounds, bounds);
                height = left_height;
                orphaned = false;
              }
            } else if (!(left_height & TREE_ORPHAN)) {
              if (((std::max)(height, left_height) > MAX_SUB_TREE_HEIGHT)) {
                SubTree* new_sub_trees = my_sub_trees->Emplace(2);
                new_sub_trees[0].temp_tree_index = parent->child[0];
                new_sub_trees[0].height = left_height;
                new_sub_trees[0].mesh_triangle_offset = mesh->triangle_offset;
                COPY_BOUNDS(parent->l_bounds, new_sub_trees[0].bounds);
                new_sub_trees[1].temp_tree_index = node_index;
                new_sub_trees[1].height = height;
                new_sub_trees[1].mesh_triangle_offset = mesh->triangle_offset;
                COPY_BOUNDS(bounds, new_sub_trees[1].bounds);
                orphaned = true;
              } else {
                MERGE_BOUNDS(bounds, parent->l_bounds, bounds);
                node_index = parent_index;
                height = (std::max)(height, left_height) + 1;
              }
            }
            range.l = ranges[parent_index].l;
          }
        } 
        ///
        if ((range.r - range.l) >= (mesh->triangles.m_count - 1)) {
          uint32_t tri_count = (range.r - range.l) + 1;
          if (tri_count <= (1 << TREE_LEAF_TRIANGLE_FACTOR)) {
            node_index = range.l | TREE_LEAF;
            node_index |= (tri_count - 1) << (31 - TREE_LEAF_TRIANGLE_FACTOR);
            height = 0;
          }
          if (!orphaned) {
            SubTree* sub_tree = my_sub_trees->Emplace(1);
            sub_tree->temp_tree_index = node_index;
            sub_tree->height = height;
            sub_tree->mesh_triangle_offset = mesh->triangle_offset;
            COPY_BOUNDS(bounds, sub_tree->bounds);
          }
          break;
        }
        ++level;
      }
    }
  });
  for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
    uint32_t sub_tree_count = TREE->thread_sub_trees[t].m_count;
    if (sub_tree_count > 0) {
      SubTree* thread_sub_trees = mesh->sub_trees.Emplace(sub_tree_count);
      memcpy(thread_sub_trees, TREE->thread_sub_trees[t].m_data, 
             sizeof(SubTree) * sub_tree_count);
    }
  }
  PARALLEL(mesh->sub_trees.m_count,
  {
    Vector<TwoNode>* my_node_buffer = &TREE->thread_node_buffers[thread_id];
    my_node_buffer->Clear();
    uint32_t my_nodes_offset = 0;
    for (uint32_t s = thread_begin; s < thread_end; ++s) {
      SubTree* sub_tree = &mesh->sub_trees[s];
      sub_tree->mesh_local_offset = my_nodes_offset;
      uint32_t largest_local_index = 0;
      uint32_t temp_node_index = sub_tree->temp_tree_index;
      if (temp_node_index & TREE_LEAF) {
        TwoNode* new_node = my_node_buffer->Emplace(1);
        new_node->extra[0] = sub_tree->mesh_triangle_offset | 
                             TREE_LEAF_SUB_TREE;
        new_node->extra[1] = temp_node_index;
        COPY_BOUNDS(sub_tree->bounds, new_node->l_bounds);
        COPY_BOUNDS(sub_tree->bounds, new_node->r_bounds);
        my_nodes_offset = my_node_buffer->m_count;
        continue;
      }
      uint32_t node_index = 0; 
      uint32_t stack[64];
      uint32_t* p_stack = stack;
      uint32_t key = 0x0;
      uint32_t trail = 0x0;
      uint32_t level_bit = 0x1 << (sub_tree->height - 1);
      uint32_t highest_index = 0;
      uint32_t node_allocation = (0x1 << sub_tree->height);
      TwoNode* new_nodes = my_node_buffer->Emplace(node_allocation);
      while (true) {
        if (node_index > highest_index) {
          highest_index = node_index;
        }
        TwoNode* temp_node = &temp_tree[temp_node_index];
        TwoNode* node = &new_nodes[node_index];
        *node = *temp_node;
        if (node_index == 0) {
          node->extra[0] = sub_tree->height;
          node->extra[1] = sub_tree->mesh_triangle_offset;
        }
        if (temp_node->child[0] < TREE_LEAF) {
          if (temp_node->child[1] < TREE_LEAF) {
            *p_stack = temp_node->child[1];
            ++p_stack;
            trail |= level_bit;
          }
          temp_node_index = temp_node->child[0];
          node_index += 1; 
        } else {
          if (temp_node->child[1] < TREE_LEAF) {
            temp_node_index = temp_node->child[1];
            key |= level_bit;
            node_index += level_bit; 
          } else {
            if (trail == 0x0) {
              break;
            }
            --p_stack;
            temp_node_index = *p_stack;
            uint32_t shift;
            BSF32(trail, shift);
            trail ^= (0x1 << shift); 
            key = (key & (0xFFFFFFFF << shift)) ^ (0x1 << shift);
            uint32_t right_count;
            POPCNT32(key, right_count);
            node_index = key + ((32 - right_count) -
                                (32 - sub_tree->height) - shift);
            level_bit = (0x1 << shift);
          }
        }
        level_bit >>= 1;
      }
      my_node_buffer->m_count -= (node_allocation - (highest_index + 1));
      my_nodes_offset = my_node_buffer->m_count;
    }
  });
  TwoNode* nodes = &TREE->nodes[mesh->node_offset];
  uint32_t* merge_offsets = (uint32_t*)malloc(sizeof(uint32_t) *
                                              ROVE.thread_count);
  for (uint32_t t = 0; t < ROVE.thread_count; ++t) {
    if (TREE->thread_node_buffers[t].m_count > 0) {
      if ((nodes + TREE->thread_node_buffers[t].m_count) >= 
          (&TREE->nodes[mesh->node_offset] + mesh->node_allocation)) {
        ERR("Mesh tree memory exceeded, please use a lower optimization level.");
        return false;
      }
      memcpy(nodes, TREE->thread_node_buffers[t].m_data,
             sizeof(TwoNode) * TREE->thread_node_buffers[t].m_count);
    }
    merge_offsets[t] = nodes - &TREE->nodes[mesh->node_offset]; 
    nodes += TREE->thread_node_buffers[t].m_count;
  }
  mesh->node_count = nodes - &TREE->nodes[mesh->node_offset];
  PARALLEL(mesh->sub_trees.m_count,
  {
    for (uint32_t s = thread_begin; s < thread_end; ++s) {
      SubTree* sub_tree = &mesh->sub_trees[s];
      sub_tree->mesh_local_offset += merge_offsets[thread_id];
    }
  });
  free(merge_offsets);
  return true;
}
///
int32_t BuildTopTree()
{
  if (SCENE->mesh_indices.m_count == 0) {
    if (TREE->triangles.m_h_count == 0) {
      TreeTriangle* dummy_triangle = TREE->triangles.Emplace(1);
      memset(dummy_triangle, 0, sizeof(TreeTriangle));
      if (!TREE->triangles.Upload(0, 1)) {
        ERR("Failed to upload dummy triangle.");
        return false;
      }
    }
    TREE->top_tree_offset = TREE->nodes.m_h_count;
    TwoNode* new_node = TREE->nodes.Emplace(1);
    new_node->child[0] = TREE_INVALID_CHILD;
    new_node->child[1] = TREE_INVALID_CHILD;
    new_node->extra[0] = 1;
    new_node->extra[1] = 0;
    memset(new_node->l_bounds, 0, sizeof(float) * 6);
    memset(new_node->r_bounds, 0, sizeof(float) * 6);
    return true;
  }
  TREE->lsh_buffers[0].Clear();
  TREE->lsh_buffers[1].Clear();
  F4 factor = F4_DIV(F4_SET1(1023.0f),
                     F4_SUB(SCENE->bounds.max, SCENE->bounds.min));
  for (uint32_t m = 0; m < SCENE->mesh_indices.m_count; ++m) {
    uint32_t mesh_index = SCENE->mesh_indices[m];
    Mesh* mesh = &SCENE->meshes[mesh_index];
    uint64_t* lsh = TREE->lsh_buffers[0].Emplace(mesh->sub_trees.m_count);
    PARALLEL(mesh->sub_trees.m_count,
    {
      for (uint32_t s = thread_begin; s < thread_end; ++s) {
        SubTree* sub_tree = &mesh->sub_trees[s];
        F4 min = F4_SET(sub_tree->bounds[0], sub_tree->bounds[1],
                        sub_tree->bounds[2], 1.0f);
        F4 max = F4_SET(sub_tree->bounds[3], sub_tree->bounds[4],
                        sub_tree->bounds[5], 1.0f);
        F4 point = F4_MUL(F4_ADD(min, max), F4_SET1(0.5f));
        point = F4_MUL(F4_SUB(point, SCENE->bounds.min), factor);
        F4A f4a_point[4];
        F4_STORE(point, f4a_point);
        uint32_t sub_tree_root_offset = mesh->node_offset + 
                                        sub_tree->mesh_local_offset;
        lsh[s] = ((uint64_t)sub_tree_root_offset << 32) |
                 ((TREE->lsh_map[(uint32_t)f4a_point[0]]) |
                  (TREE->lsh_map[(uint32_t)f4a_point[1]] << 1) |
                  (TREE->lsh_map[(uint32_t)f4a_point[2]] << 2));
      }
    });
  }
  uint32_t sub_tree_count = TREE->lsh_buffers[0].m_count;
  if (sub_tree_count == 1) {
		uint32_t node_index = (TREE->lsh_buffers[0][0] >> 32);
    TwoNode* sub_tree_root = &TREE->nodes[node_index];
    float bounds[6];
    MERGE_BOUNDS(sub_tree_root->l_bounds, sub_tree_root->r_bounds, bounds);
    if (sub_tree_root->extra[0] & TREE_LEAF_SUB_TREE) {
      uint32_t triangle_offset = sub_tree_root->extra[0] & 
                                 TREE_LEAF_SUB_TREE_MASK;
      node_index = sub_tree_root->extra[1] + triangle_offset;
    } else {
      node_index |= TREE_SUB_TREE;
    }
    TREE->top_tree_offset = TREE->nodes.m_h_count;
    TwoNode* new_node = TREE->nodes.Emplace(1);
    new_node->child[0] = node_index;
    new_node->child[1] = TREE_INVALID_CHILD;
    new_node->extra[0] = 1;
    new_node->extra[1] = 0;
    COPY_BOUNDS(bounds, new_node->l_bounds);
    MIN_TO_BOUNDS(bounds, new_node->r_bounds);
    return true;
  }
  TREE->lsh_buffers[1].Emplace(sub_tree_count);
  uint32_t thread_partials = (std::min)(sub_tree_count, ROVE.thread_count);
  uint32_t* scan =
    (uint32_t*)malloc(sizeof(uint32_t) * (ROVE.thread_count * 1024 + 1));
  uint64_t* flip = TREE->lsh_buffers[0].m_data;
  uint64_t* flop = TREE->lsh_buffers[1].m_data;
  for (uint32_t pass = 0; pass < 3; ++pass) {
    uint32_t shift = pass * 10;
    memset(scan, 0, sizeof(uint32_t) * (ROVE.thread_count * 1024 + 1));
    PARALLEL(sub_tree_count,
    {
      for (uint32_t m = thread_begin; m < thread_end; ++m) {
        uint32_t digit = (flip[m] >> shift) & 0x3FF;
        ++scan[digit * thread_partials + thread_id + 1];
      }
    });
    for (uint32_t i = 2; i < 1024 * thread_partials + 1; ++i) {
      scan[i] += scan[i - 1];
    }
    scan[0] = 0;
    PARALLEL(sub_tree_count,
    {
      uint32_t* local_count = (uint32_t*)calloc(1024, sizeof(uint32_t));
      for (uint32_t m = thread_begin; m < thread_end; ++m) {
        uint32_t digit = (flip[m] >> shift) & 0x3FF;
        flop[scan[digit * thread_partials + thread_id] +
                  local_count[digit]++] = flip[m];
      }
      free(local_count);
    });
    // https://www.youtube.com/watch?v=Qa98AhEVAZQ
    std::swap(flip, flop);
  }
  free(scan);
  Range* ranges = (Range*)flop;
  TREE->visited_buffer.Clear();
  TREE->visited_buffer.Emplace(sub_tree_count);
  for (uint32_t i = 0; i < sub_tree_count; ++i) {
    TREE->visited_buffer[i] = TREE_NOT_VISITED;
  }
  TREE->node_buffer.Clear();
  TREE->node_buffer.Emplace(sub_tree_count);
  TwoNode* temp_tree = TREE->node_buffer.m_data;
  TwoNode* temp_tree_root = &temp_tree[0];
  uint32_t temp_tree_height = 0;
  PARALLEL(sub_tree_count,
  {
    TwoNode* parent;
    for (uint32_t s = thread_begin; s < thread_end; ++s) {
      uint32_t height = 0;
      Range range;
      range.r = s;
      range.l = s;
      uint32_t node_index = (flip[s] >> 32);
      uint32_t raw_node_index = s;
      TwoNode* sub_tree_root = &TREE->nodes[node_index];
      float bounds[6];
      MERGE_BOUNDS(sub_tree_root->l_bounds, sub_tree_root->r_bounds, bounds);
      if (sub_tree_root->extra[0] & TREE_LEAF_SUB_TREE) {
        uint32_t tri_offset = sub_tree_root->extra[0] & TREE_LEAF_SUB_TREE_MASK;
        node_index = sub_tree_root->extra[1] + tri_offset;
      } else {
        node_index |= TREE_SUB_TREE;
      }
      uint32_t level = 0;
			while (true) {
        uint32_t left = range.l - (range.l > 0);
        uint32_t right = range.r + (range.r < (sub_tree_count - 1));
        uint32_t delta_right = flip[range.r] ^ flip[right];
        uint32_t delta_left = flip[range.l] ^ flip[left];
        uint32_t diff_right = 0;
        uint32_t diff_left = 0;
        if (delta_right != 0) {
          BSR32(delta_right, diff_right);
        }
        if (delta_left != 0) {
          BSR32(delta_left, diff_left);
        }
        uint32_t level_index = raw_node_index >> level;
        int32_t heuristic;
        if (ROVE.optimization_level == ROVE_OPTIMIZATION_SPEED) {
          heuristic = (diff_right < diff_left);
        } else {
          heuristic = !(level_index & 0x1);
        }
        ///
        if ((range.l == 0) || 
            ((range.r < (sub_tree_count - 1)) && heuristic)) {
          uint32_t parent_index = range.r;
          raw_node_index = parent_index;
          ranges[parent_index].l = range.l;
          parent = &temp_tree[parent_index];
          parent->child[0] = node_index;
          COPY_BOUNDS(bounds, parent->l_bounds);
          uint32_t right_height = TREE_NOT_VISITED;
          int32_t first =
            TREE->visited_buffer[parent_index].compare_exchange_strong(right_height,
                                                                       height);
          if (first) {
            break;
          } else {
            range.r = ranges[parent_index].r;
            height = (std::max)(height, right_height) + 1;
            node_index = parent_index;
          }
        }
				///
        else {
          uint32_t parent_index = left;
          raw_node_index = parent_index;
          ranges[parent_index].r = range.r;
          parent = &temp_tree[parent_index];
          parent->child[1] = node_index;
          COPY_BOUNDS(bounds, parent->r_bounds);
          uint32_t left_height = TREE_NOT_VISITED;
          int32_t first =
            TREE->visited_buffer[parent_index].compare_exchange_strong(left_height,
                                                                       height);
          if (first) {
            break;
          } else {
            range.l = ranges[parent_index].l;
            height = (std::max)(height, left_height) + 1;
            node_index = parent_index;
          }
        }
        if ((range.r - range.l) >= (sub_tree_count - 1)) {
          temp_tree_root = parent;
          temp_tree_height = height;
          parent->extra[1] = TREE_ROOT;
          break;
        }
        ++level;
        MERGE_BOUNDS(parent->l_bounds, parent->r_bounds, bounds);
      }
    }
  });
  if (temp_tree_height >= 23) {
    LOG("** Warning: Excessive top-tree memory; " 
        "please use a lower optimization level.");
  }
  TREE->top_tree_offset = TREE->nodes.m_h_count;
  uint32_t temp_node_index = temp_tree_root - temp_tree;
  uint32_t node_index = 0; 
  uint32_t stack[64];
  uint32_t* p_stack = stack;
  uint32_t key = 0x0;
  uint32_t trail = 0x0;
  uint32_t level_bit = 0x1 << (temp_tree_height - 1);
  uint32_t highest_index = 0;
  uint32_t node_allocation = (0x1 << temp_tree_height);
  TwoNode* new_nodes = TREE->nodes.Emplace(node_allocation);
  while (true) {
    if (node_index > highest_index) {
      highest_index = node_index;
    }
    TwoNode* temp_node = &temp_tree[temp_node_index];
    TwoNode* node = &new_nodes[node_index];
    *node = *temp_node;
    if (node_index == 0) {
      node->extra[0] = temp_tree_height;
      node->extra[1] = 0;
    }
    if (temp_node->child[0] < TREE_SUB_TREE) {
      if (temp_node->child[1] < TREE_SUB_TREE) {
        *p_stack = temp_node->child[1];
        ++p_stack;
        trail |= level_bit;
      }
      temp_node_index = temp_node->child[0];
      node_index += 1; 
    } else {
      if (temp_node->child[1] < TREE_SUB_TREE) {
        temp_node_index = temp_node->child[1];
        key |= level_bit;
        node_index += level_bit;
      } else {
        if (trail == 0x0) {
          break;
        }
        --p_stack;
        temp_node_index = *p_stack;
        uint32_t shift;
        BSF32(trail, shift);
        trail ^= (0x1 << shift);
        key = (key & (0xFFFFFFFF << shift)) ^ (0x1 << shift);
        uint32_t right_count;
        POPCNT32(key, right_count);
        node_index = key + ((32 - right_count) -
                            (32 - temp_tree_height) - shift);
        level_bit = (0x1 << shift);
      }
    }
    level_bit >>= 1;
  }
  TREE->nodes.m_h_count -= (node_allocation - (highest_index + 1));
  return true;
}
