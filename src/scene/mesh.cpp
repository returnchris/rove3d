#include "../rove.h"
#include "scene.h"
#include "mesh.h"
#include "tree.h"
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
                 uint32_t sub_mesh_count)
{
  LOG("Importing mesh: %u vertices, %u triangles. %s %s %s",
      vertex_count, triangle_count, 
      has_normals ? "Has normals." : "No normals.",
      has_tangents ? "Has tangents." : "No tangents.",
      has_uvs ? "Has UVs." : "No UVs.");
  Mesh* mesh = SCENE->meshes.Emplace();
  mesh->enabled = true;
  mesh->free_flag = false;
  mesh->update_flag = true;
  mesh->has_normals = has_normals;
  mesh->has_uvs = has_uvs;
  mesh->dirty_nodes.dirty = false;
  mesh->dirty_nodes.double_dirty = false;
  SET_IDENTITY_MATRIX(mesh->transform);
  mesh->sub_trees.Setup(0, 0);
  *SCENE->mesh_indices.Emplace(1) = (mesh - SCENE->meshes.m_items.m_data);
  mesh->sub_meshes.Setup(0, sub_mesh_count);
  mesh->sub_meshes.Emplace(sub_mesh_count);
  mesh->vertices.Setup(16, vertex_count);
  mesh->vertices.Emplace(vertex_count);
  mesh->triangles.Setup(0, triangle_count);
  mesh->triangles.Emplace(triangle_count);
  mesh->emitter_indices.Setup(0, 0);
  mesh->portal_indices.Setup(0, 0);
  for (uint32_t v = 0; v < vertex_count; ++v) {
    mesh->vertices[v].position[0] = vertices[v * 3];
    mesh->vertices[v].position[1] = vertices[v * 3 + 1];
    mesh->vertices[v].position[2] = vertices[v * 3 + 2];
    if (has_normals) {
      mesh->vertices[v].normal[0] = normals[v * 3];
      mesh->vertices[v].normal[1] = normals[v * 3 + 1];
      mesh->vertices[v].normal[2] = normals[v * 3 + 2];
    } else {
      mesh->vertices[v].normal[0] = 0.0f;
      mesh->vertices[v].normal[1] = 0.0f; 
      mesh->vertices[v].normal[2] = 0.0f; 
    }
    if (has_tangents) {
      mesh->vertices[v].tangent[0] = tangents[v * 4];
      mesh->vertices[v].tangent[1] = tangents[v * 4 + 1];
      mesh->vertices[v].tangent[2] = tangents[v * 4 + 2];
      mesh->vertices[v].tangent[3] = tangents[v * 4 + 3];
    } else {
      mesh->vertices[v].tangent[0] = 0.0f;
      mesh->vertices[v].tangent[1] = 0.0f; 
      mesh->vertices[v].tangent[2] = 0.0f; 
      mesh->vertices[v].tangent[3] = 0.0f; 
    }
    if (has_uvs) {
      mesh->vertices[v].uv[0] = uvs[v * 2];
      mesh->vertices[v].uv[1] = uvs[v * 2 + 1]; 
    } else {
      mesh->vertices[v].uv[0] = 0.0f;
      mesh->vertices[v].uv[1] = 0.0f; 
    }
  }
  for (uint32_t t = 0; t < triangle_count; ++t) {
    mesh->triangles[t].indices[0] = triangles[t * 3];
    mesh->triangles[t].indices[1] = triangles[t * 3 + 1];
    mesh->triangles[t].indices[2] = triangles[t * 3 + 2];
    mesh->triangles[t].sub_mesh_index = 0;
  }
  TreeTriangle* mesh_triangles = TREE->triangles.Emplace(triangle_count);
  if (!mesh_triangles) {
    ERR("Failed to allocate triangles for mesh.");
    return nullptr;
  }
  mesh->triangle_offset = (mesh_triangles - &TREE->triangles[0]);
  if (ROVE.optimization_level == ROVE_OPTIMIZATION_SPEED) {
    mesh->node_allocation = mesh->triangles.m_count * 6;
  } else if (ROVE.optimization_level == ROVE_OPTIMIZATION_BALANCED) {
    mesh->node_allocation = mesh->triangles.m_count * 4;
  } else {
    mesh->node_allocation = mesh->triangles.m_count * 2;
  }
  TwoNode* mesh_nodes = TREE->nodes.Emplace(mesh->node_allocation);
  if (!mesh_nodes) {
    ERR("Failed to allocate nodes for mesh.");
    return nullptr;
  }
  mesh->node_offset = (mesh_nodes - &TREE->nodes[0]);
  mesh->node_count = 0;
  SCENE->update_flag = true;
  LOG("Triangle offset: %u, node offset: %u.", mesh->triangle_offset,
      mesh->node_offset);
  return mesh;
}
///
void SetMeshFreeFlag(Mesh* mesh)
{
  mesh->free_flag = true;
  SCENE->free_flag = true;
}
///
void FreeMeshMemory(Mesh* mesh)
{
  LOG("Freeing mesh memory: %u vertices, %u triangles.",
      mesh->vertices.m_count, mesh->triangles.m_count);
  mesh->sub_trees.Shutdown();
  mesh->sub_meshes.Shutdown();
  mesh->vertices.Shutdown();
  mesh->triangles.Shutdown();
  mesh->emitter_indices.Shutdown();
  mesh->portal_indices.Shutdown();
}
///
void DefineSubMesh(Mesh* mesh, 
                   uint32_t sub_mesh_index,
                   uint32_t triangle_offset,
                   uint32_t triangle_count,
                   uint32_t material_id)
{
  LOG("Defining sub-mesh: %u, from triangles %u to %u.",
      sub_mesh_index, triangle_offset, triangle_offset + triangle_count);
  SubMesh* sub_mesh = &mesh->sub_meshes[sub_mesh_index];
  sub_mesh->triangle_offset = triangle_offset;
  sub_mesh->triangle_count = triangle_count;
  sub_mesh->material_id = material_id;
  mesh->update_flag = true;
  SCENE->update_flag = true;
  Material* material = &TREE->materials[sub_mesh->material_id];
  sub_mesh->is_emissive = (material->emission > 0.0f);
  sub_mesh->is_portal = (material->emission < 0.0f);
  for (uint32_t t = triangle_offset; t < triangle_offset + triangle_count; ++t) {
    mesh->triangles[t].sub_mesh_index = sub_mesh_index;
  }
  if (sub_mesh->is_emissive) {
    SCENE->update_emitters_flag = true;
  } else if (sub_mesh->is_portal) {
    SCENE->update_portals_flag = true;
  }
}
///
void ChangeSubMeshMaterial(Mesh* mesh,
                           uint32_t sub_mesh_index,
                           uint32_t material_id)
{
  SubMesh* sub_mesh = &mesh->sub_meshes[sub_mesh_index];
  sub_mesh->material_id = material_id;
  mesh->update_flag = true;
  SCENE->update_flag = true;
  Material* material = &TREE->materials[sub_mesh->material_id];
  sub_mesh->is_emissive = (material->emission > 0.0f);
  sub_mesh->is_portal = (material->emission < 0.0f);
  if (sub_mesh->is_emissive) {
    SCENE->update_emitters_flag = true;
  } else if (sub_mesh->is_portal) {
    SCENE->update_portals_flag = true;
  }
}
///
void SetMeshTransform(Mesh* mesh,
                      float* transform)
{
  memcpy(&mesh->transform, transform, sizeof(float) * 16);
  mesh->update_flag = true;
  SCENE->update_flag = true;
}
