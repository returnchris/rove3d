#include "material.h"
#include "../rove.h"
#include "../core/utility.h"
#include "scene.h"
///
Material* AddMaterial(uint32_t map_flags,
                      int32_t double_sided,
                      float* albedo,
                      float metallic,
                      float smoothness,
                      float glass,
                      float emission,
                      float* albedo_bounds,
                      float* normal_bounds,
                      float* metallic_bounds,
                      float* emission_bounds)
{
  Material* material = TREE->materials.Emplace(1);
  material->map_flags = map_flags;
  material->double_sided = double_sided;
  material->albedo[0] = albedo[0];
  material->albedo[1] = albedo[1];
  material->albedo[2] = albedo[2];
  material->albedo[3] = albedo[3];
  material->metallic = metallic;
  material->smoothness = smoothness;
  material->glass = glass;
  material->emission = emission;
  material->albedo_bounds[0] = albedo_bounds[0];
  material->albedo_bounds[1] = albedo_bounds[1];
  material->albedo_bounds[2] = albedo_bounds[2];
  material->albedo_bounds[3] = albedo_bounds[3];
  material->normal_bounds[0] = normal_bounds[0];
  material->normal_bounds[1] = normal_bounds[1];
  material->normal_bounds[2] = normal_bounds[2];
  material->normal_bounds[3] = normal_bounds[3];
  material->metallic_bounds[0] = metallic_bounds[0];
  material->metallic_bounds[1] = metallic_bounds[1];
  material->metallic_bounds[2] = metallic_bounds[2];
  material->metallic_bounds[3] = metallic_bounds[3];
  material->emission_bounds[0] = emission_bounds[0];
  material->emission_bounds[1] = emission_bounds[1];
  material->emission_bounds[2] = emission_bounds[2];
  material->emission_bounds[3] = emission_bounds[3];
  SCENE->update_materials_flag = true;
  return material;
}
///
void SetMaterialMapFlags(uint32_t material_id,
                         uint32_t map_flags)
{
  Material* material = &TREE->materials[material_id];
  material->map_flags = map_flags;
  SCENE->update_materials_flag = true;
}
///
void SetMaterialDoubleSided(uint32_t material_id,
                            int32_t double_sided)
{
  Material* material = &TREE->materials[material_id];
  material->double_sided = double_sided;
  SCENE->update_materials_flag = true;
}
///
void SetMaterialAlbedo(uint32_t material_id,
                       float* albedo)
{
  Material* material = &TREE->materials[material_id];
  material->albedo[0] = albedo[0];
  material->albedo[1] = albedo[1];
  material->albedo[2] = albedo[2];
  material->albedo[3] = albedo[3];
  SCENE->update_materials_flag = true;
}
///
void SetMaterialMetallic(uint32_t material_id,
                         float metallic)
{
  Material* material = &TREE->materials[material_id];
  material->metallic = metallic; 
  SCENE->update_materials_flag = true;
}
///
void SetMaterialSmoothness(uint32_t material_id,
                           float smoothness)
{
  Material* material = &TREE->materials[material_id];
  material->smoothness = smoothness; 
  SCENE->update_materials_flag = true;
}
///
void SetMaterialGlass(uint32_t material_id,
                      float glass)
{
  Material* material = &TREE->materials[material_id];
  material->glass = glass; 
  SCENE->update_materials_flag = true;
}
///
void SetMaterialEmission(uint32_t material_id,
                         float emission)
{
  Material* material = &TREE->materials[material_id];
  if (((material->emission == 0.0f) && (emission > 0.0f)) ||
      ((material->emission > 0.0f) && (emission == 0.0f))) {
    SCENE->update_emitters_flag = true;
  }
  if (((material->emission < 0.0f) && (emission >= 0.0f)) ||
      ((material->emission >= 0.0f) && (emission < 0.0f))) {
    SCENE->update_portals_flag = true;
  }
  material->emission = emission; 
  SCENE->update_materials_flag = true;
}
///
void SetMaterialAlbedoBounds(uint32_t material_id,
                             float* bounds)
{
  Material* material = &TREE->materials[material_id];
  material->albedo_bounds[0] = bounds[0];
  material->albedo_bounds[1] = bounds[1];
  material->albedo_bounds[2] = bounds[2];
  material->albedo_bounds[3] = bounds[3];
  SCENE->update_materials_flag = true;
}
///
void SetMaterialNormalBounds(uint32_t material_id,
                             float* bounds)
{
  Material* material = &TREE->materials[material_id];
  material->normal_bounds[0] = bounds[0];
  material->normal_bounds[1] = bounds[1];
  material->normal_bounds[2] = bounds[2];
  material->normal_bounds[3] = bounds[3];
  SCENE->update_materials_flag = true;
}
///
void SetMaterialMetallicBounds(uint32_t material_id,
                               float* bounds)
{
  Material* material = &TREE->materials[material_id];
  material->metallic_bounds[0] = bounds[0];
  material->metallic_bounds[1] = bounds[1];
  material->metallic_bounds[2] = bounds[2];
  material->metallic_bounds[3] = bounds[3];
  SCENE->update_materials_flag = true;
}
///
void SetMaterialEmissionBounds(uint32_t material_id,
                               float* bounds)
{
  Material* material = &TREE->materials[material_id];
  material->emission_bounds[0] = bounds[0];
  material->emission_bounds[1] = bounds[1];
  material->emission_bounds[2] = bounds[2];
  material->emission_bounds[3] = bounds[3];
  SCENE->update_materials_flag = true;
}
