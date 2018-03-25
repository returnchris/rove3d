#pragma once
///
#include <cstdint>
#include <string>
#if defined(ROVE_WINDOWS)
  #define ROVE_EXPORT __declspec(dllexport) __stdcall
#else
  #define ROVE_EXPORT
#endif
///
typedef uint32_t RoveStatus;
#define ROVE_ERROR   0xFFFFFFFF
#define ROVE_SUCCESS 0x1
typedef uint32_t RoveLock;
#define ROVE_UNLOCKED 0x0
#define ROVE_LOCKED   0x1
typedef uint32_t RoveComputeAPI;
#define ROVE_COMPUTE_API_CL    0x0
#define ROVE_COMPUTE_API_CUDA  0x1
///
#define ROVE_MAP_FLAG_ALBEDO   0x1
#define ROVE_MAP_FLAG_NORMAL   0x2
#define ROVE_MAP_FLAG_METALLIC 0x4
#define ROVE_MAP_FLAG_EMISSION 0x8
///
#define ROVE_OPTIMIZATION_SPEED    0x0
#define ROVE_OPTIMIZATION_BALANCED 0x1
#define ROVE_OPTIMIZATION_MEMORY   0x2
///
extern "C"
{
  ///
  RoveLock ROVE_EXPORT RoveGetLock();
  RoveStatus ROVE_EXPORT RoveSetup(char* data_path,
                                   int32_t use_multi_device,
                                   int32_t use_integrated_device,
                                   uint32_t optimization_level,
                                   void* target);
  void ROVE_EXPORT RoveShutdown();
  RoveStatus ROVE_EXPORT RoveResize(void* target);
  RoveStatus ROVE_EXPORT RoveStartUpdate();
  void ROVE_EXPORT RoveFinishUpdate();
  void ROVE_EXPORT RoveResetRenderAtStart();
  void ROVE_EXPORT RoveResetRenderAtEnd();
  uint32_t ROVE_EXPORT RoveGetThreadCount();
  RoveComputeAPI ROVE_EXPORT RoveGetComputeAPI();
  uint32_t ROVE_EXPORT RoveGetDeviceCount();
  const char ROVE_EXPORT* RoveGetDeviceName(uint32_t device_index);
  RoveStatus ROVE_EXPORT RoveImportMesh(uint32_t vertex_count,
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
  void ROVE_EXPORT RoveFreeMesh(uint32_t mesh_id); 
  void ROVE_EXPORT RoveDefineSubMesh(uint32_t mesh_id,
                                     uint32_t sub_mesh_index,
                                     uint32_t triangle_offset,
                                     uint32_t triangle_count,
                                     uint32_t material_id);
  void ROVE_EXPORT RoveChangeSubMeshMaterial(uint32_t mesh_id,
                                             uint32_t sub_mesh_index,
                                             uint32_t material_id);
  void ROVE_EXPORT RoveSetMeshTransform(uint32_t mesh_id,
                                        float* transform);
  uint32_t ROVE_EXPORT RoveAddMaterial(uint32_t map_flags,
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
  void ROVE_EXPORT RoveSetMaterialMapFlags(uint32_t material_id,
                                           uint32_t map_flags);
  void ROVE_EXPORT RoveSetMaterialDoubleSided(uint32_t material_id,
                                              int32_t double_sided);
  void ROVE_EXPORT RoveSetMaterialAlbedo(uint32_t material_id,
                                         float* albedo);
  void ROVE_EXPORT RoveSetMaterialMetallic(uint32_t material_id,
                                           float metallic);
  void ROVE_EXPORT RoveSetMaterialSmoothness(uint32_t material_id,
                                             float smoothness);
  void ROVE_EXPORT RoveSetMaterialGlass(uint32_t material_id,
                                        float glass);
  void ROVE_EXPORT RoveSetMaterialEmission(uint32_t material_id,
                                           float emission);
  void ROVE_EXPORT RoveSetMaterialAlbedoBounds(uint32_t material_id,
                                               float* bounds);
  void ROVE_EXPORT RoveSetMaterialNormalBounds(uint32_t material_id,
                                               float* bounds);
  void ROVE_EXPORT RoveSetMaterialMetallicBounds(uint32_t material_id,
                                                 float* bounds);
  void ROVE_EXPORT RoveSetMaterialEmissionBounds(uint32_t material_id,
                                                 float* bounds);
  RoveStatus ROVE_EXPORT RoveSetAlbedoAtlas(uint8_t* atlas,
                                            uint32_t width,
                                            uint32_t height);
  RoveStatus ROVE_EXPORT RoveSetNormalAtlas(uint8_t* atlas,
                                            uint32_t width,
                                            uint32_t height);
  RoveStatus ROVE_EXPORT RoveSetMetallicAtlas(uint8_t* atlas,
                                              uint32_t width,
                                              uint32_t height);
  RoveStatus ROVE_EXPORT RoveSetEmissionAtlas(uint8_t* atlas,
                                              uint32_t width,
                                              uint32_t height);
  void ROVE_EXPORT RoveSetFieldOfView(float field_of_view);
  void ROVE_EXPORT RoveSetMaxBounces(uint32_t max_bounces);
  void ROVE_EXPORT RoveSetSamplesPerFrame(uint32_t sample_count);
  void ROVE_EXPORT RoveSetImageProperties(int32_t tonemap,
                                          float gamma,
                                          float exposure,
                                          float contrast,
                                          float saturation,
                                          float brightness);
  void ROVE_EXPORT RoveSetCameraApertureSize(float aperture_size);
  void ROVE_EXPORT RoveSetCameraFocalDepth(float focal_depth);
  void ROVE_EXPORT RoveSetCameraTransform(float* transform);
  void ROVE_EXPORT RoveSetEnvironmentType(uint32_t type);
  void ROVE_EXPORT RoveSetSunDirection(float* direction);
  void ROVE_EXPORT RoveSetSkyIntensity(float intensity);
  void ROVE_EXPORT RoveSetSunIntensity(float intensity);
  void ROVE_EXPORT RoveSetSunColor(float* color);
  int32_t ROVE_EXPORT RoveSetEnvironmentMap(uint8_t* map,
                                            uint32_t width,
                                            uint32_t height);
  void ROVE_EXPORT RoveSetEnvironmentProperties(float intensity,
                                                float rotation);
  ///
	#include "unity/IUnityGraphics.h"
	#include "unity/IUnityInterface.h"
  #if defined(ROVE_D3D11)
    #include "unity/IUnityGraphicsD3D11.h"
    void ROVE_EXPORT RoveSetD3D11Device(ID3D11Device* device);
  #endif
  UnityRenderingEvent ROVE_EXPORT RoveUnitySetupEvent(char* data_path,
                                                      int32_t use_multi_device,
                                                      int32_t use_integrated_device,
                                                      uint32_t optimization_level,
                                                      void* target);
  UnityRenderingEvent ROVE_EXPORT RoveUnityFinishUpdateEvent();
  UnityRenderingEvent ROVE_EXPORT RoveUnityResizeEvent(void* target);
}
