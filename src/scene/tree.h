#pragma once
#include <cstdint>
#include "../compute/cl/cl_dbuffer.h"
#include "../compute/cuda/cuda_dbuffer.h"
#include "../core/vector.h"
#include "material.h"
///
#define MAX_SUB_TREE_HEIGHT 6
#define TREE_LEAF_TRIANGLE_FACTOR 3 // Don't forget to change in kernels. 
#define TREE_LEAF 0x80000000
#define TREE_LEAF_MASK 0x7FFFFFFF
#define TREE_SUB_TREE 0x40000000
#define TREE_SUB_TREE_MASK 0x3FFFFFFF
#define TREE_LEAF_SUB_TREE 0x20000000
#define TREE_LEAF_SUB_TREE_MASK 0x1FFFFFFF
#define TREE_ROOT 0xFFFFFFFF
#define TREE_INVALID_CHILD 0xFFFFFFFF
///
#define TREE_NOT_VISITED 0xFFFFFFFF
#define TREE_ORPHAN 0x80000000
/// 64 bytes
struct TwoNode
{
  uint32_t child[2];
  uint32_t extra[2];
  float l_bounds[6];
  float r_bounds[6];
};
/// 128 bytes
struct FourNode
{
  uint32_t child[4];
  uint32_t extra[2];
  float ll_bounds[6];
  float lr_bounds[6];
  float rl_bounds[6];
  float rr_bounds[6];
  float padding[2];
};
/// 168 bytes
struct TreeTriangle
{
  float vertex[3];
  float edges[2][3];
  float vertex_normals[3][3];
  float vertex_tangents[3][4];
  float vertex_uvs[3][2];
  float surface_normal[3];
  float inverse_area;
  uint32_t material_id;
  uint32_t pad;
};
///
struct EmitterIndex
{
  uint32_t mesh_triangle_offset;
  uint32_t triangle_offset;
};
///
struct PortalIndex
{
  uint32_t mesh_triangle_offset;
  uint32_t triangle_offset;
};
///
struct Range
{
  int32_t l;
  int32_t r;
};
///
struct Bounds
{
  F4 min;
  F4 max;
};
///
struct SubTree
{
  uint32_t temp_tree_index;
  uint32_t height;
  uint32_t mesh_local_offset;
  uint32_t mesh_triangle_offset;
  float bounds[6];
};
///
struct Dirty
{
  int32_t dirty;
  int32_t double_dirty;
};
///
struct Tree
{
  uint32_t top_tree_offset;
  DBuffer<TreeTriangle> triangles;
  DBuffer<TwoNode> nodes; 
  DBuffer<Material> materials;
  DBuffer<EmitterIndex> emitter_indices;
  DBuffer<PortalIndex> portal_indices;
  Dirty dirty_materials;
  Dirty dirty_emitters;
  Dirty dirty_portals;
  uint32_t* lsh_map;
  Vector<uint64_t> lsh_buffers[2];
  Vector<Bounds> bounds_buffer;
  Vector<std::atomic<uint32_t>> visited_buffer;
  Vector<TwoNode> node_buffer;
  Vector<Vector<SubTree>> thread_sub_trees;
  Vector<Vector<TwoNode>> thread_node_buffers;
};
///
void SetupTree();
void ShutdownTree();
int32_t BuildMeshSubTrees(struct Mesh* mesh);
int32_t BuildTopTree();
