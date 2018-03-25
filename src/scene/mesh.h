#pragma once
///
#include <cstdint>
#include "tree.h"
#include "../core/vector.h"
///
struct Vertex
{
  float position[4];
  float normal[4];
  float tangent[4];
  float uv[2];
  float padding[2];
};
///
struct Triangle
{
  uint32_t indices[3];
  uint32_t sub_mesh_index;
};
///
struct SubMesh
{
  uint32_t triangle_offset;
  uint32_t triangle_count;
  uint32_t material_id;
  int32_t is_emissive;
  int32_t is_portal;
};
///
struct Mesh
{
  int32_t enabled;
  int32_t free_flag;
  int32_t update_flag;
  int32_t has_normals;
  int32_t has_tangents;
  int32_t has_uvs;
  Dirty dirty_triangles;
  Dirty dirty_nodes;
  Bounds bounds;
  float transform[16];
  Vector<SubTree> sub_trees;
  Vector<SubMesh> sub_meshes;
  Vector<Vertex> vertices;
  Vector<Triangle> triangles;
  Vector<EmitterIndex> emitter_indices;
  Vector<PortalIndex> portal_indices;
  uint32_t triangle_offset;
  uint32_t node_offset;
  uint32_t node_allocation;
  uint32_t node_count;
};
///
Mesh* ImportMesh(uint32_t vertex_count,
                 uint32_t triangle_count,
                 int32_t has_normals,
                 int32_t has_tangents,
                 int32_t has_uvs,
                 float* vertices,
                 int32_t* triangles,
                 float* normals,
                 float* tangents,
                 float* uvs,
                 uint32_t sub_mesh_count);
void SetMeshFreeFlag(Mesh* mesh);
void FreeMeshMemory(Mesh* mesh);
void DefineSubMesh(Mesh* mesh, 
                   uint32_t sub_mesh_index,
                   uint32_t triangle_offset,
                   uint32_t triangle_count,
                   uint32_t material_id);
void ChangeSubMeshMaterial(Mesh* mesh,
                           uint32_t sub_mesh_index,
                           uint32_t material_id);
void SetMeshTransform(Mesh* mesh,
                      float* transform);
