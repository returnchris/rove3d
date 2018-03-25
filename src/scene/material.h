#pragma once
///
#include <cstdint>
///
struct Material
{
  uint32_t map_flags;
  int32_t double_sided;
  float albedo[4];
  float metallic;
  float smoothness;
  float glass;
  float emission;
  float albedo_bounds[4];
  float normal_bounds[4];
  float metallic_bounds[4];
  float emission_bounds[4];
};
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
                      float* emission_bounds);
void SetMaterialMapFlags(uint32_t material_id,
                         uint32_t map_flags);
void SetMaterialDoubleSided(uint32_t material_id,
                            int32_t double_sided);
void SetMaterialAlbedo(uint32_t material_id,
                       float* albedo);
void SetMaterialMetallic(uint32_t material_id,
                         float metallic);
void SetMaterialSmoothness(uint32_t material_id,
                           float smoothness);
void SetMaterialGlass(uint32_t material_id,
                      float glass);
void SetMaterialEmission(uint32_t material_id,
                         float emission);
void SetMaterialAlbedoBounds(uint32_t material_id,
                             float* bounds);
void SetMaterialNormalBounds(uint32_t material_id,
                             float* bounds);
void SetMaterialMetallicBounds(uint32_t material_id,
                               float* bounds);
void SetMaterialEmissionBounds(uint32_t material_id,
                               float* bounds);
