#include <cfloat>
#include "../rove.h"
#include "../core/utility.h"
#include "../core/thread_pool.h"
#include "../compute/cuda/cuda_dbuffer.h"
#include "../compute/cl/cl_dbuffer.h"
#include "tree.h"
#include "scene.h"
///
int32_t SetupScene()
{
  LOG("Setting up scene.");
  memset(SCENE, 0, sizeof(Scene));
  SCENE->meshes.Setup();
  SCENE->mesh_indices.Setup(0, 0);
  SCENE->lsh_buffers[0].Setup(0, 0);
  SCENE->lsh_buffers[1].Setup(0, 0);
  SCENE->vertex_buffer.Setup(16, 0);
  SCENE->environment_type = ENVIRONMENT_SKY;
  SCENE->camera.aperture_size = 0.0f;
  SCENE->camera.focal_depth = 10.0f;
  SET_IDENTITY_MATRIX(SCENE->camera.transform);
  float direction[3] = { 0.0f, -1.0f, 0.0f };
  SetSunDirection(direction);
  SCENE->sun.sun_intensity = 1.0f;
  SCENE->sun.sky_intensity = 1.0f;
  SCENE->sun.color[0] = 1.0f;
  SCENE->sun.color[1] = 1.0f;
  SCENE->sun.color[2] = 1.0f;
  SCENE->sun.color[3] = 1.0f;
  SCENE->image_properties.tonemap = true;
  SCENE->image_properties.gamma = 1.25f;
  SCENE->image_properties.exposure = 1.0f;
  SCENE->image_properties.contrast = 1.2f;
  SCENE->image_properties.saturation = 1.2f;
  SCENE->image_properties.brightness = 1.0f;
  uint8_t empty_texture[4] = { 255, 255, 255, 255 };
  uint8_t empty_half_texture[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  if (!(SetAlbedoAtlas(empty_texture, 1, 1)) ||
      !(SetNormalAtlas(empty_texture, 1, 1)) ||
      !(SetMetallicAtlas(empty_texture, 1, 1)) ||
      !(SetEmissionAtlas(empty_texture, 1, 1)) ||
      !(SetEnvironmentMap(empty_half_texture, 1, 1))) {
    ERR("Failed to initialize atlases.");
    return false;
  }
  if (!SetupSky()) {
    LOG("Failed to setup sky.");
    return false;
  }
  LOG("Creating default material.");
  float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  float no_bounds[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  AddMaterial(0x0, false, white, 0.0f, 0.0f, 0.0f, 0.0f,
              no_bounds, no_bounds, no_bounds, no_bounds);
  SCENE->update_flag = true;
  SCENE->update_emitters_flag = true;
  SCENE->update_portals_flag = true;
  return true;
}
///
void ShutdownScene()
{
  LOG("Shutting down scene.");
  for (uint32_t m = 0; m < SCENE->mesh_indices.m_count; ++m) {
    FreeMeshMemory(&SCENE->meshes[SCENE->mesh_indices[m]]);
  }
  SCENE->meshes.Shutdown();
  SCENE->mesh_indices.Shutdown();
  SCENE->lsh_buffers[0].Shutdown();
  SCENE->lsh_buffers[1].Shutdown();
  SCENE->vertex_buffer.Shutdown();
  memset(SCENE, 0, sizeof(Scene));
}
///
void UpdateScene(RoveStatus* scene_status)
{
  if (SCENE->free_flag) {
    uint32_t* mesh_cursor = SCENE->mesh_indices.m_data;
    TreeTriangle* triangle_cursor = TREE->triangles.m_h_data;
    TwoNode* node_cursor = TREE->nodes.m_h_data;
    uint32_t mesh_count = SCENE->mesh_indices.m_count;
    for (uint32_t m = 0; m < mesh_count; ++m) {
      uint32_t mesh_index = SCENE->mesh_indices[m];
      Mesh* mesh = &SCENE->meshes[mesh_index];
      if (mesh->free_flag) {
        for (uint32_t s = 0; s < mesh->sub_meshes.m_count; ++s) {
          SubMesh* sub_mesh = &mesh->sub_meshes[s];
          if (sub_mesh->is_emissive) {
            SCENE->update_emitters_flag = true;
          } else if (sub_mesh->is_portal) {
            SCENE->update_portals_flag = true; 
          }
        }
        TREE->triangles.m_h_count -= mesh->triangles.m_count;
        TREE->nodes.m_h_count -= mesh->node_allocation;
        FreeMeshMemory(mesh);
        SCENE->meshes.Free(mesh);
        --SCENE->mesh_indices.m_count;
        SCENE->update_flag = true;
        continue;
      }
      *mesh_cursor = mesh_index;
      ++mesh_cursor;
      TreeTriangle* mesh_triangles = &TREE->triangles[mesh->triangle_offset];
      if (mesh_triangles != triangle_cursor) {
        for (uint32_t s = 0; s < mesh->sub_meshes.m_count; ++s) {
          SubMesh* sub_mesh = &mesh->sub_meshes[s];
          if (sub_mesh->is_emissive) {
            SCENE->update_emitters_flag = true;
          } else if (sub_mesh->is_portal) {
            SCENE->update_portals_flag = true; 
          }
        }
        memcpy(triangle_cursor, mesh_triangles, 
               sizeof(TreeTriangle) * mesh->triangles.m_count);
        mesh->triangle_offset = triangle_cursor - &TREE->triangles[0];
        mesh->dirty_triangles.dirty = true;
        mesh->dirty_triangles.double_dirty = true;
      }
      triangle_cursor += mesh->triangles.m_count;
      TwoNode* mesh_nodes = &TREE->nodes[mesh->node_offset];
      if (mesh_nodes != node_cursor) {
        memcpy(node_cursor, mesh_nodes, sizeof(TwoNode) * mesh->node_allocation);
        mesh->node_offset = node_cursor - &TREE->nodes[0];
        mesh->dirty_nodes.double_dirty = true;
        mesh->dirty_nodes.dirty = true;
        mesh->dirty_nodes.double_dirty = true;
      }
      node_cursor += mesh->node_allocation;
    }
    SCENE->free_flag = false;
  }
  if (SCENE->update_flag || 
      SCENE->update_emitters_flag || 
      SCENE->update_portals_flag) {
    if (SCENE->update_emitters_flag) {
      TREE->emitter_indices.Clear();
    }
    if (SCENE->update_portals_flag) {
      TREE->portal_indices.Clear();
    }
    SCENE->bounds.min = F4_SET1(FLT_MAX);
    SCENE->bounds.max = F4_SET1(-FLT_MAX);
    for (uint32_t m = 0; m < SCENE->mesh_indices.m_count; ++m) {
      Mesh* mesh = &SCENE->meshes[SCENE->mesh_indices[m]];
      if (!mesh->update_flag) {
        SCENE->bounds.min = F4_MIN(SCENE->bounds.min, mesh->bounds.min);
        SCENE->bounds.max = F4_MAX(SCENE->bounds.max, mesh->bounds.max);
        for (uint32_t s = 0; s < mesh->sub_trees.m_count; ++s) {
          SubTree* sub_tree = &mesh->sub_trees[s];
					TwoNode* root = &TREE->nodes[mesh->node_offset + 
                                       sub_tree->mesh_local_offset];
					if (root->extra[0] & TREE_LEAF_SUB_TREE) {
						root->extra[0] = mesh->triangle_offset | TREE_LEAF_SUB_TREE;
					} else {
						root->extra[1] = mesh->triangle_offset;
					}
					sub_tree->mesh_triangle_offset = mesh->triangle_offset;
        }
        if (mesh->dirty_triangles.dirty) {
          if (!TREE->triangles.Upload(mesh->triangle_offset, 
                                      mesh->triangles.m_count)) {
            ERR("Failed to upload triangles for mesh.");
            *scene_status = ROVE_ERROR;
            return;
          }
          if (mesh->dirty_triangles.double_dirty) {
            mesh->dirty_triangles.double_dirty = false;
          } else {
            mesh->dirty_triangles.dirty = false;
          }
        }
        if (mesh->dirty_nodes.dirty) {
          if (!TREE->nodes.Upload(mesh->node_offset, mesh->node_count)) {
            ERR("Failed to upload nodes for mesh.");
            *scene_status = ROVE_ERROR;
            return;
          }
          if (mesh->dirty_nodes.double_dirty) {
            mesh->dirty_nodes.double_dirty = false;
          } else {
            mesh->dirty_nodes.dirty = false;
          }
        }
        if (SCENE->update_emitters_flag) {
          if (SCENE->update_materials_flag) {
            if (mesh->emitter_indices.m_count > 0) {
              uint32_t emitter_count = mesh->emitter_indices.m_count;
              EmitterIndex* emitter_cursor = mesh->emitter_indices.m_data;
              for (uint32_t l = 0; l < emitter_count; ++l) {
                uint32_t ti = mesh->emitter_indices[l].mesh_triangle_offset +
                              mesh->emitter_indices[l].triangle_offset;
                TreeTriangle* d_triangle = &TREE->triangles[ti];
                Material* material = &TREE->materials[d_triangle->material_id];
                if (material->emission <= 0.0f) {
                  --mesh->emitter_indices.m_count;
                  continue;
                } 
                EmitterIndex* current_emitter = &mesh->emitter_indices[l];
                current_emitter->mesh_triangle_offset = mesh->triangle_offset;
                *emitter_cursor = *current_emitter;
                ++emitter_cursor;
              }
            }
            for (uint32_t s = 0; s < mesh->sub_meshes.m_count; ++s) {
              SubMesh* sub_mesh = &mesh->sub_meshes[s];
              Material* material = &TREE->materials[sub_mesh->material_id];
              if (sub_mesh->is_emissive && (material->emission <= 0.0f)) {
                sub_mesh->is_emissive = false;
              } else if (!sub_mesh->is_emissive && (material->emission > 0.0f)) {
                sub_mesh->is_emissive = true;
              }
              if (sub_mesh->is_emissive) {
                EmitterIndex* new_emitters = 
                  mesh->emitter_indices.Emplace(sub_mesh->triangle_count);
                for (uint32_t t = 0; t < sub_mesh->triangle_count; ++t) {
                  new_emitters[t].mesh_triangle_offset = mesh->triangle_offset;
                  new_emitters[t].triangle_offset = sub_mesh->triangle_offset + t;
                }
              }
            }
          }
          if (mesh->emitter_indices.m_count > 0) {
            EmitterIndex* emitters = 
              TREE->emitter_indices.Emplace(mesh->emitter_indices.m_count);
            memcpy(emitters, mesh->emitter_indices.m_data,
                   sizeof(EmitterIndex) * mesh->emitter_indices.m_count);
          }
        }
        if (SCENE->update_portals_flag) {
          if (SCENE->update_materials_flag) {
            if (mesh->portal_indices.m_count > 0) {
              uint32_t portal_count = mesh->portal_indices.m_count;
              PortalIndex* portal_cursor = mesh->portal_indices.m_data;
              for (uint32_t p = 0; p < portal_count; ++p) {
                uint32_t ti = mesh->portal_indices[p].mesh_triangle_offset +
                              mesh->portal_indices[p].triangle_offset;
                TreeTriangle* d_triangle = &TREE->triangles[ti];
                Material* material = &TREE->materials[d_triangle->material_id];
                if (material->emission >= 0.0f) {
                  --mesh->portal_indices.m_count;
                  continue;
                } 
                PortalIndex* current_portal = &mesh->portal_indices[p];
                current_portal->mesh_triangle_offset = mesh->triangle_offset;
                *portal_cursor = *current_portal;
                ++portal_cursor;
              }
            }
            for (uint32_t s = 0; s < mesh->sub_meshes.m_count; ++s) {
              SubMesh* sub_mesh = &mesh->sub_meshes[s];
              Material* material = &TREE->materials[sub_mesh->material_id];
              if (sub_mesh->is_portal && (material->emission >= 0.0f)) {
                sub_mesh->is_portal = false;
              } else if (!sub_mesh->is_portal && (material->emission < 0.0f)) {
                sub_mesh->is_portal = true;
              }
              if (sub_mesh->is_portal) {
                PortalIndex* new_portals = 
                  mesh->portal_indices.Emplace(sub_mesh->triangle_count);
                for (uint32_t t = 0; t < sub_mesh->triangle_count; ++t) {
                  new_portals[t].mesh_triangle_offset = mesh->triangle_offset;
                  new_portals[t].triangle_offset = sub_mesh->triangle_offset + t;
                }
              }
            }
          }
          if (mesh->portal_indices.m_count > 0) {
            PortalIndex* portals = 
              TREE->portal_indices.Emplace(mesh->portal_indices.m_count);
            memcpy(portals, mesh->portal_indices.m_data,
                   sizeof(PortalIndex) * mesh->portal_indices.m_count);
          }
        }
      } else {
        if (SCENE->update_emitters_flag || SCENE->update_portals_flag) {
          if (SCENE->update_emitters_flag) {
            mesh->emitter_indices.Clear();
          }
          if (SCENE->update_portals_flag) {
            mesh->portal_indices.Clear();
          }
					for (uint32_t s = 0; s < mesh->sub_meshes.m_count; ++s) {
						SubMesh* sub_mesh = &mesh->sub_meshes[s];
            Material* material = &TREE->materials[sub_mesh->material_id];
						if (SCENE->update_emitters_flag) {
              if (sub_mesh->is_emissive && (material->emission <= 0.0f)) {
                sub_mesh->is_emissive = false;
              } else if (!sub_mesh->is_emissive && (material->emission > 0.0f)) {
                sub_mesh->is_emissive = true;
              }
              if (sub_mesh->is_emissive) {
							  mesh->emitter_indices.Emplace(sub_mesh->triangle_count);
              }
						} 
            if (SCENE->update_portals_flag) {
              if (sub_mesh->is_portal && (material->emission >= 0.0f)) {
                sub_mesh->is_portal = false;
              } else if (!sub_mesh->is_portal && (material->emission < 0.0f)) {
                sub_mesh->is_portal = true;
              }
              if (sub_mesh->is_portal) {
							  mesh->portal_indices.Emplace(sub_mesh->triangle_count);
              }
						}
					}
        }
        TREE->bounds_buffer.Clear();
        TREE->bounds_buffer.Emplace(mesh->triangles.m_count);
        Vector<Bounds> thread_bounds;
        thread_bounds.Setup(16, ROVE.thread_count);
        SCENE->vertex_buffer.Clear();
        SCENE->vertex_buffer.Emplace(mesh->triangles.m_count * 3);
        F4 transform_x = F4_LOAD(&mesh->transform[0]);
        F4 transform_y = F4_LOAD(&mesh->transform[4]);
        F4 transform_z = F4_LOAD(&mesh->transform[8]);
        F4 transform_w = F4_LOAD(&mesh->transform[12]);
        PARALLEL(mesh->triangles.m_count, 
        {
          Bounds* my_thread_bounds = &thread_bounds[thread_id];
          my_thread_bounds->min = F4_SET1(FLT_MAX);
          my_thread_bounds->max = F4_SET1(-FLT_MAX);
          Bounds* bounds_buffer = TREE->bounds_buffer.m_data;
          for (uint32_t t = thread_begin; t < thread_end; ++t) {
            uint32_t* indices = mesh->triangles[t].indices;
            F4 vertices[3];
            vertices[0] = F4_LOAD(&mesh->vertices[indices[0]].position[0]);
            vertices[1] = F4_LOAD(&mesh->vertices[indices[1]].position[0]);
            vertices[2] = F4_LOAD(&mesh->vertices[indices[2]].position[0]);
            F4 temp[9];
            temp[0] = F4_MUL(F4_PERMUTE(vertices[0], XXXX), transform_x);
            temp[1] = F4_MUL(F4_PERMUTE(vertices[0], YYYY), transform_y);
            temp[2] = F4_MUL(F4_PERMUTE(vertices[0], ZZZZ), transform_z);
            temp[3] = F4_MUL(F4_PERMUTE(vertices[1], XXXX), transform_x);
            temp[4] = F4_MUL(F4_PERMUTE(vertices[1], YYYY), transform_y);
            temp[5] = F4_MUL(F4_PERMUTE(vertices[1], ZZZZ), transform_z);
            temp[6] = F4_MUL(F4_PERMUTE(vertices[2], XXXX), transform_x);
            temp[7] = F4_MUL(F4_PERMUTE(vertices[2], YYYY), transform_y);
            temp[8] = F4_MUL(F4_PERMUTE(vertices[2], ZZZZ), transform_z);
            F4* tx_vertices = &SCENE->vertex_buffer[t * 3];
            tx_vertices[0] = F4_ADD(F4_ADD(F4_ADD(temp[0], temp[1]), temp[2]),
                                    transform_w);
            tx_vertices[1] = F4_ADD(F4_ADD(F4_ADD(temp[3], temp[4]), temp[5]),
                                    transform_w);
            tx_vertices[2] = F4_ADD(F4_ADD(F4_ADD(temp[6], temp[7]), temp[8]),
                                    transform_w);
            bounds_buffer[t].min = F4_MIN(F4_MIN(tx_vertices[0], tx_vertices[1]), 
                                          tx_vertices[2]);
            bounds_buffer[t].max = F4_MAX(F4_MAX(tx_vertices[0], tx_vertices[1]), 
                                          tx_vertices[2]);
            my_thread_bounds->min = F4_MIN(my_thread_bounds->min,
                                           bounds_buffer[t].min);
            my_thread_bounds->max = F4_MAX(my_thread_bounds->max,
                                           bounds_buffer[t].max);
          }
        });
        mesh->bounds.min = thread_bounds[0].min;
        mesh->bounds.max = thread_bounds[0].max;
        for (uint32_t t = 1; t < ROVE.thread_count; ++t) {
          mesh->bounds.min = F4_MIN(mesh->bounds.min,
                                    thread_bounds[t].min);
          mesh->bounds.max = F4_MAX(mesh->bounds.max,
                                    thread_bounds[t].max);
        }
        SCENE->bounds.min = F4_MIN(SCENE->bounds.min, mesh->bounds.min);
        SCENE->bounds.max = F4_MAX(SCENE->bounds.max, mesh->bounds.max);
        thread_bounds.Shutdown();
        TREE->lsh_buffers[0].Clear();
        TREE->lsh_buffers[1].Clear();
        TREE->lsh_buffers[0].Emplace(mesh->triangles.m_count);
        TREE->lsh_buffers[1].Emplace(mesh->triangles.m_count);
        F4 factor = F4_DIV(F4_SET1(1023.0f),
                           F4_SUB(mesh->bounds.max, mesh->bounds.min));
        PARALLEL(mesh->triangles.m_count,
        {
          for (uint32_t t = thread_begin; t < thread_end; ++t) {
            F4 point = F4_MUL(F4_ADD(TREE->bounds_buffer[t].min,
                                     TREE->bounds_buffer[t].max), 
                              F4_SET1(0.5f));
            point = F4_MUL(F4_SUB(point, mesh->bounds.min), factor);
            F4A f4a_point[4];
            F4_STORE(point, f4a_point);
            TREE->lsh_buffers[0][t] = 
                       ((uint64_t)t << 32) |
                       ((TREE->lsh_map[(uint32_t)f4a_point[0]]) |
                        (TREE->lsh_map[(uint32_t)f4a_point[1]] << 1) |
                        (TREE->lsh_map[(uint32_t)f4a_point[2]] << 2));
          }
        });
        uint32_t thread_partials = (std::min)(mesh->triangles.m_count, 
                                              ROVE.thread_count);
        uint32_t* scan = (uint32_t*)malloc(sizeof(uint32_t) * 
                                           (ROVE.thread_count * 1024 + 1));
        uint64_t* flip = TREE->lsh_buffers[0].m_data;
        uint64_t* flop = TREE->lsh_buffers[1].m_data;
        for (uint32_t pass = 0; pass < 3; ++pass) {
          uint32_t shift = pass * 10;
          memset(scan, 0, sizeof(uint32_t) * (ROVE.thread_count * 1024 + 1));
          PARALLEL(mesh->triangles.m_count,
          {
            for (uint32_t t = thread_begin; t < thread_end; ++t) {
              uint32_t digit = (flip[t] >> shift) & 0x3FF;
              ++scan[digit * thread_partials + thread_id + 1];
            }
          });
          for (uint32_t i = 2; i < 1024 * thread_partials + 1; ++i) {
            scan[i] += scan[i - 1];
          }
          scan[0] = 0;
          PARALLEL(mesh->triangles.m_count,
          {
            uint32_t* local_count = (uint32_t*)calloc(1024, sizeof(uint32_t));
            for (uint32_t t = thread_begin; t < thread_end; ++t) {
              uint32_t digit = (flip[t] >> shift) & 0x3FF;
              flop[scan[digit * thread_partials + thread_id] +
                        local_count[digit]++] = flip[t];
            }
            free(local_count);
          });
          // https://www.youtube.com/watch?v=Qa98AhEVAZQ
          std::swap(flip, flop);
        }
        free(scan);
        std::atomic<uint32_t> atomic_emitter_index(0); 
        std::atomic<uint32_t> atomic_portal_index(0); 
        PARALLEL(mesh->triangles.m_count, 
        {
          for (uint32_t t = thread_begin; t < thread_end; ++t) {
            uint32_t tri_index = (flip[t] >> 32); 
            uint32_t* indices = mesh->triangles[tri_index].indices;
            F4 normals[3];
						normals[0] = F4_LOAD(&mesh->vertices[indices[0]].normal[0]);
						normals[1] = F4_LOAD(&mesh->vertices[indices[1]].normal[0]);
						normals[2] = F4_LOAD(&mesh->vertices[indices[2]].normal[0]);
            F4 temp_n[3][3];
            temp_n[0][0] = F4_MUL(F4_PERMUTE(normals[0], XXXX), transform_x);
						temp_n[0][1] = F4_MUL(F4_PERMUTE(normals[0], YYYY), transform_y);
						temp_n[0][2] = F4_MUL(F4_PERMUTE(normals[0], ZZZZ), transform_z);
						temp_n[1][0] = F4_MUL(F4_PERMUTE(normals[1], XXXX), transform_x);
						temp_n[1][1] = F4_MUL(F4_PERMUTE(normals[1], YYYY), transform_y);
						temp_n[1][2] = F4_MUL(F4_PERMUTE(normals[1], ZZZZ), transform_z);
						temp_n[2][0] = F4_MUL(F4_PERMUTE(normals[2], XXXX), transform_x);
						temp_n[2][1] = F4_MUL(F4_PERMUTE(normals[2], YYYY), transform_y);
						temp_n[2][2] = F4_MUL(F4_PERMUTE(normals[2], ZZZZ), transform_z);
            normals[0] = F4_ADD(F4_ADD(temp_n[0][0], temp_n[0][1]), temp_n[0][2]);
						normals[1] = F4_ADD(F4_ADD(temp_n[1][0], temp_n[1][1]), temp_n[1][2]);
						normals[2] = F4_ADD(F4_ADD(temp_n[2][0], temp_n[2][1]), temp_n[2][2]);
            /*
						normals[0] = F4_NORMALIZE(normals[0]);
						normals[1] = F4_NORMALIZE(normals[1]);
						normals[2] = F4_NORMALIZE(normals[2]);
            */
            F4 tangents[3];
						tangents[0] = F4_LOAD(&mesh->vertices[indices[0]].tangent[0]);
						tangents[1] = F4_LOAD(&mesh->vertices[indices[1]].tangent[0]);
						tangents[2] = F4_LOAD(&mesh->vertices[indices[2]].tangent[0]);
            F4 temp_t[3][3];
            temp_t[0][0] = F4_MUL(F4_PERMUTE(tangents[0], XXXX), transform_x);
						temp_t[0][1] = F4_MUL(F4_PERMUTE(tangents[0], YYYY), transform_y);
						temp_t[0][2] = F4_MUL(F4_PERMUTE(tangents[0], ZZZZ), transform_z);
						temp_t[1][0] = F4_MUL(F4_PERMUTE(tangents[1], XXXX), transform_x);
						temp_t[1][1] = F4_MUL(F4_PERMUTE(tangents[1], YYYY), transform_y);
						temp_t[1][2] = F4_MUL(F4_PERMUTE(tangents[1], ZZZZ), transform_z);
						temp_t[2][0] = F4_MUL(F4_PERMUTE(tangents[2], XXXX), transform_x);
						temp_t[2][1] = F4_MUL(F4_PERMUTE(tangents[2], YYYY), transform_y);
						temp_t[2][2] = F4_MUL(F4_PERMUTE(tangents[2], ZZZZ), transform_z);
            tangents[0] = F4_BLEND(
                            F4_ADD(F4_ADD(temp_t[0][0], temp_t[0][1]), temp_t[0][2]),
                            tangents[0], 0x8); 
            tangents[1] = F4_BLEND(
                            F4_ADD(F4_ADD(temp_t[1][0], temp_t[1][1]), temp_t[1][2]),
                            tangents[1], 0x8); 
            tangents[2] = F4_BLEND(
                            F4_ADD(F4_ADD(temp_t[2][0], temp_t[2][1]), temp_t[2][2]),
                            tangents[2], 0x8); 
            /*
						tangents[0] = F4_NORMALIZE(tangents[0]);
						tangents[1] = F4_NORMALIZE(tangents[1]);
						tangents[2] = F4_NORMALIZE(tangents[2]);
            */
            F4 vertices[3];
            vertices[0] = SCENE->vertex_buffer[tri_index * 3]; 
            vertices[1] = SCENE->vertex_buffer[tri_index * 3 + 1]; 
            vertices[2] = SCENE->vertex_buffer[tri_index * 3 + 2]; 
            F4 edge[2];
            edge[0] = F4_SUB(vertices[1], vertices[0]);
            edge[1] = F4_SUB(vertices[0], vertices[2]);
            F4 surf_normal = F4_SUB(F4_MUL(edge[1], F4_PERMUTE(edge[0], YZXW)),
                                    F4_MUL(edge[0], F4_PERMUTE(edge[1], YZXW)));
            surf_normal = F4_PERMUTE(surf_normal, YZXW);
            F4A f4a_vertex[4];
            F4A f4a_edges[2][4];
            F4A f4a_surf_normal[4];
            F4A f4a_normals[3][4];
            F4A f4a_tangents[3][4];
            F4_STORE(vertices[0], f4a_vertex);
            F4_STORE(edge[0], f4a_edges[0]);
            F4_STORE(edge[1], f4a_edges[1]);
            F4_STORE(surf_normal, f4a_surf_normal);
            F4_STORE(normals[0], f4a_normals[0]);
            F4_STORE(normals[1], f4a_normals[1]);
            F4_STORE(normals[2], f4a_normals[2]);
            F4_STORE(tangents[0], f4a_tangents[0]);
            F4_STORE(tangents[1], f4a_tangents[1]);
            F4_STORE(tangents[2], f4a_tangents[2]);
            TreeTriangle* triangle = 
						  &TREE->triangles[mesh->triangle_offset + t];
            triangle->vertex[0] = f4a_vertex[0];
            triangle->vertex[1] = f4a_vertex[1];
            triangle->vertex[2] = f4a_vertex[2];
            triangle->edges[0][0] = f4a_edges[0][0];
            triangle->edges[0][1] = f4a_edges[0][1];
            triangle->edges[0][2] = f4a_edges[0][2];
            triangle->edges[1][0] = f4a_edges[1][0];
            triangle->edges[1][1] = f4a_edges[1][1];
            triangle->edges[1][2] = f4a_edges[1][2];
            triangle->surface_normal[0] = f4a_surf_normal[0];
            triangle->surface_normal[1] = f4a_surf_normal[1];
            triangle->surface_normal[2] = f4a_surf_normal[2];
            triangle->vertex_normals[0][0] = f4a_normals[0][0];
            triangle->vertex_normals[0][1] = f4a_normals[0][1];
            triangle->vertex_normals[0][2] = f4a_normals[0][2];
            triangle->vertex_normals[1][0] = f4a_normals[1][0];
            triangle->vertex_normals[1][1] = f4a_normals[1][1];
            triangle->vertex_normals[1][2] = f4a_normals[1][2];
            triangle->vertex_normals[2][0] = f4a_normals[2][0];
            triangle->vertex_normals[2][1] = f4a_normals[2][1];
            triangle->vertex_normals[2][2] = f4a_normals[2][2];
            triangle->vertex_tangents[0][0] = f4a_tangents[0][0];
            triangle->vertex_tangents[0][1] = f4a_tangents[0][1];
            triangle->vertex_tangents[0][2] = f4a_tangents[0][2];
            triangle->vertex_tangents[0][3] = f4a_tangents[0][3];
            triangle->vertex_tangents[1][0] = f4a_tangents[1][0];
            triangle->vertex_tangents[1][1] = f4a_tangents[1][1];
            triangle->vertex_tangents[1][2] = f4a_tangents[1][2];
            triangle->vertex_tangents[1][3] = f4a_tangents[1][3];
            triangle->vertex_tangents[2][0] = f4a_tangents[2][0];
            triangle->vertex_tangents[2][1] = f4a_tangents[2][1];
            triangle->vertex_tangents[2][2] = f4a_tangents[2][2];
            triangle->vertex_tangents[2][3] = f4a_tangents[2][3];
            triangle->vertex_uvs[0][0] = mesh->vertices[indices[0]].uv[0];
            triangle->vertex_uvs[0][1] = mesh->vertices[indices[0]].uv[1];
            triangle->vertex_uvs[1][0] = mesh->vertices[indices[1]].uv[0];
            triangle->vertex_uvs[1][1] = mesh->vertices[indices[1]].uv[1];
            triangle->vertex_uvs[2][0] = mesh->vertices[indices[2]].uv[0];
            triangle->vertex_uvs[2][1] = mesh->vertices[indices[2]].uv[1];
            triangle->inverse_area = 2.0f / VECTOR_LENGTH(f4a_surf_normal);
						SubMesh* sub_mesh = 
              &mesh->sub_meshes[mesh->triangles[tri_index].sub_mesh_index];
						if (SCENE->update_emitters_flag && sub_mesh->is_emissive) {
							uint32_t i = atomic_emitter_index++;
							mesh->emitter_indices[i].mesh_triangle_offset = mesh->triangle_offset;
							mesh->emitter_indices[i].triangle_offset = t;
						} else if (SCENE->update_portals_flag && sub_mesh->is_portal) {
							uint32_t i = atomic_portal_index++;
							mesh->portal_indices[i].mesh_triangle_offset = mesh->triangle_offset;
							mesh->portal_indices[i].triangle_offset = t;
						}
            triangle->material_id = sub_mesh->material_id;;
          }
        });
        if (!TREE->triangles.Upload(mesh->triangle_offset, 
                                    mesh->triangles.m_count)) {
          ERR("Failed to upload triangles for mesh.");
          *scene_status = ROVE_ERROR;
          return;
        }
        if (SCENE->update_emitters_flag && (mesh->emitter_indices.m_count > 0)) {
          EmitterIndex* emitters = 
            TREE->emitter_indices.Emplace(mesh->emitter_indices.m_count);
          memcpy(emitters, mesh->emitter_indices.m_data,
                 sizeof(EmitterIndex) * mesh->emitter_indices.m_count);
        }
        if (SCENE->update_portals_flag && (mesh->portal_indices.m_count > 0)) {
          PortalIndex* portals = 
            TREE->portal_indices.Emplace(mesh->portal_indices.m_count);
          memcpy(portals, mesh->portal_indices.m_data,
                 sizeof(PortalIndex) * mesh->portal_indices.m_count);
        }
        if (!BuildMeshSubTrees(mesh)) {
          ERR("Failed to build sub-trees for mesh.");
          *scene_status = ROVE_ERROR;
          return;
        }
        if (!TREE->nodes.Upload(mesh->node_offset, mesh->node_count)) {
          ERR("Failed to upload nodes for mesh.");
          *scene_status = ROVE_ERROR;
          return;
        }
        mesh->dirty_triangles.dirty = true;
        mesh->dirty_nodes.dirty = true;
        mesh->update_flag = false;
      }
    }
  }
  if (SCENE->update_flag) {
    if (!BuildTopTree()) {
      ERR("Failed to build top tree.");
      *scene_status = ROVE_ERROR;
      return;
    }
  }
  *scene_status = ROVE_SUCCESS;
  return;
}
///
int32_t UploadScene()
{
  if (SCENE->update_flag) {
    if (!TREE->nodes.Upload(TREE->top_tree_offset, 
                            TREE->nodes.m_h_count - 
                            TREE->top_tree_offset)) {
      ERR("Failed to upload top tree nodes.");
      return false;
    }
    TREE->nodes.m_h_count = TREE->top_tree_offset;
  }
  if (SCENE->update_materials_flag) {
    if (!TREE->materials.Upload(0, TREE->materials.m_h_count)) {
      ERR("Failed to upload materials.");
      return false;
    }
    TREE->dirty_materials.dirty = true;
  } else if (TREE->dirty_materials.dirty) {
    if (!TREE->materials.Upload(0, TREE->materials.m_h_count)) {
      ERR("Failed to upload materials.");
      return false;
    }
    TREE->dirty_materials.dirty = false;
  }
  if (SCENE->update_emitters_flag) {
    if (!TREE->emitter_indices.Upload(0, TREE->emitter_indices.m_h_count)) {
      ERR("Failed to upload emitters.");
      return false;
    }
    TREE->dirty_emitters.dirty = true;
  } else if (TREE->dirty_emitters.dirty) {
    if (!TREE->emitter_indices.Upload(0, TREE->emitter_indices.m_h_count)) {
      ERR("Failed to upload emitters.");
      return false;
    }
    TREE->dirty_emitters.dirty = false;
  }
  if (SCENE->update_portals_flag) {
    if (!TREE->portal_indices.Upload(0, TREE->portal_indices.m_h_count)) {
      ERR("Failed to upload portals.");
      return false;
    }
    TREE->dirty_portals.dirty = true;
  } else if (TREE->dirty_portals.dirty) {
    if (!TREE->portal_indices.Upload(0, TREE->portal_indices.m_h_count)) {
      ERR("Failed to upload portals.");
      return false;
    }
    TREE->dirty_portals.dirty = false;
  }
  return true;
}
///
void SetCameraApertureSize(float aperture_size)
{
  SCENE->camera.aperture_size = aperture_size;
}
///
void SetCameraFocalDepth(float focal_depth)
{
  SCENE->camera.focal_depth = focal_depth;
}
///
void SetCameraTransform(float* transform)
{
  memcpy(&SCENE->camera.transform, transform, sizeof(float) * 16);
}
///
void SetEnvironmentType(uint32_t type)
{
  SCENE->environment_type = type;
}
///
void SetSunDirection(float* direction)
{
  float inverse_length = -1.0f / sqrt(direction[0] * direction[0] +
                                      direction[1] * direction[1] +
                                      direction[2] * direction[2]);
  SCENE->sun.direction[0] = direction[0] * inverse_length;
  SCENE->sun.direction[1] = direction[1] * inverse_length;
  SCENE->sun.direction[2] = direction[2] * inverse_length;
}
///
void SetSkyIntensity(float intensity)
{
  SCENE->sun.sky_intensity = intensity;
}
///
void SetSunIntensity(float intensity)
{
  SCENE->sun.sun_intensity = intensity;
}
///
void SetSunColor(float* color)
{
  SCENE->sun.color[0] = color[0];
  SCENE->sun.color[1] = color[1];
  SCENE->sun.color[2] = color[2];
  SCENE->sun.color[3] = color[3];
}
///
void SetEnvironmentProperties(float intensity,
                              float rotation)
{
  SCENE->environment_properties.intensity = intensity;
  SCENE->environment_properties.rotation = rotation;
}
///
void SetImageProperties(int32_t tonemap, 
                        float gamma, 
                        float exposure, 
                        float contrast, 
                        float saturation, 
                        float brightness)
{
  SCENE->image_properties.tonemap = tonemap;
  SCENE->image_properties.gamma = gamma;
  SCENE->image_properties.exposure = exposure;
  SCENE->image_properties.contrast = contrast;
  SCENE->image_properties.saturation = saturation;
  SCENE->image_properties.brightness = brightness;
}
