#include <thread>
#include "rove_api.h"
#include "rove.h"
#include "core/utility.h"
#include "compute/compute.h"
#include "core/thread_pool.h"
#include "compute/cl/cl_compute.h"
#include "compute/cuda/cuda_compute.h"
#include "scene/scene.h"
#include "scene/tree.h"
///
Rove rove = {};
#if defined(ROVE_D3D11)
	ID3D11Device* rove_d3d_device = nullptr;
#endif
///
RoveLock RoveGetLock()
{
  return ROVE.lock;
}
///
RoveStatus RoveSetup(char* data_path,
                     int32_t use_multi_device,
                     int32_t use_integrated_device,
                     uint32_t optimization_level,
                     void* target)
{
  std::string data_path_string(data_path);
  if (ROVE.data_path && (data_path_string == ROVE.data_path)) {
    memset(&ROVE, 0, sizeof(Rove));
    ROVE.data_path = data_path;
  } else {
    memset(&ROVE, 0, sizeof(Rove));
    STRING_TO_CHARS(data_path_string, ROVE.data_path);
  }
  ROVE.lock = ROVE_LOCKED;
  std::string log_path_string = data_path_string + "/rove.log";
  ROVE.log = fopen(log_path_string.c_str(), "w");
  if (!ROVE.log) {
    printf("\nRove3D failed to create log file at: %s.\n", 
           data_path_string.c_str());
    return ROVE_ERROR;
  }
  std::string stdout_path_string = data_path_string + "/stdout.log";
  freopen(stdout_path_string.c_str(), "w", stdout);
  LOG("Rove3D v.0.1.0\n---");
  LOG("Checking system.");
  #if defined(ROVE_LINUX)
    LOG("Linux.");
  #elif defined(ROVE_MAC)
    LOG("Mac.");
  #elif defined(ROVE_WINDOWS)
    LOG("Windows.");
  #endif
  ROVE.thread_count = std::thread::hardware_concurrency() / 2;
  LOG("CPU threads: %u.", ROVE.thread_count);
  ROVE.use_multi_device = use_multi_device;
  ROVE.use_integrated_device = use_integrated_device;
  LOG("Multiple GPU devices: %s.", ROVE.use_multi_device ? 
                                   "Enabled" : "Disabled");
  LOG("Integrated GPU device: %s.", ROVE.use_integrated_device ? 
                                   "Enabled" : "Disabled");
  if (optimization_level == ROVE_OPTIMIZATION_SPEED) {
    LOG("Optimization level: Speed.");
  } else if (optimization_level == ROVE_OPTIMIZATION_BALANCED) {
    LOG("Optimization level: Balalanced.");
  } else {
    LOG("Optimization level: Memory.");
  }
  #if defined(ROVE_GL)
    LOG("Graphics API: OpenGL.");
    glBindTexture(GL_TEXTURE_2D, (GLuint)((uint64_t)target));
    GLint width, height, format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, 
                             &format);
    if (format != GL_RGBA8) {
      ERR("Invalid target internal format: %i; must use GL_RGBA8.", format);
      return ROVE_ERROR;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    ROVE.target_width = width;
    ROVE.target_height = height;
    LOG("%u x %u.", ROVE.target_width, ROVE.target_height);
  #elif defined(ROVE_D3D11)
    LOG("Graphics API: Direct3D 11.");
    if (rove_d3d_device == nullptr) {
        ERR("D3D device missing; please use RoveSetD3D11Device.");
        return ROVE_ERROR;
    }
    ID3D11Texture2D* d3d11_tex = (ID3D11Texture2D*)target;
    D3D11_TEXTURE2D_DESC desc;
    d3d11_tex->GetDesc(&desc);
    if (desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM) {
      ERR("Invalid target DXGI_FORMAT: %i; must use R8G8B8A8_UNORM.", 
          desc.Format);
      return ROVE_ERROR;
    }
    ROVE.target_width = desc.Width;
    ROVE.target_height = desc.Height;
    LOG("%u x %u", ROVE.target_width, ROVE.target_height);
  #endif
  if ((ROVE.target_width < 1) || (ROVE.target_width > 16384) ||
      (ROVE.target_height < 1) || (ROVE.target_height > 16384)) {
    ERR("Invalid target dimensions.");
    return ROVE_ERROR;
  }
  ROVE.target = target;
  ROVE.field_of_view = 45.0f;
  ROVE.max_bounces = 4;
  ROVE.samples_per_frame = 1;
  ROVE.optimization_level = optimization_level;
  THREAD_POOL = (ThreadPool*)malloc(sizeof(ThreadPool));
  THREAD_POOL->Setup();
  COMPUTE = (Compute*)malloc(sizeof(Compute));
  if (!SetupCompute()) {
    ERR("Failed to setup compute.");
    return ROVE_ERROR;
  }
  TREE = (Tree*)malloc(sizeof(Tree));
  SetupTree();
  SCENE = (Scene*)malloc(sizeof(Scene));
  if (!SetupScene()) {
    ERR("Failed to setup scene.");
    return ROVE_ERROR;
  }
  ResetFieldOfView();
  LOG("Rove3D setup successful with %u available device(s).\n---",
      COMPUTE->compositing_target_count + 1);
  ROVE.lock = ROVE_UNLOCKED;
  return ROVE_SUCCESS;
}
///
void RoveShutdown()
{
  LOG("---\nShutting down.");
  if (ROVE.scene_thread.joinable()) {
    ROVE.scene_thread.join();
  }
  if (ROVE.data_path) {
    free(ROVE.data_path);
  }
  if (THREAD_POOL) {
    THREAD_POOL->Shutdown();
    free(THREAD_POOL);
  }
  if (SCENE) {
    ShutdownScene();
    free(SCENE);
  }
  if (TREE) {
    ShutdownTree();
    free(TREE);
  }
  if (COMPUTE) {
    ShutdownCompute();
    free(COMPUTE);
  }
  LOG("---\nThank you for using Rove3D.");
  if (ROVE.log) {
    fclose(ROVE.log);
  }
  memset(&ROVE, 0, sizeof(Rove));
  ROVE.lock = ROVE_LOCKED;
}
///
RoveStatus RoveResize(void* target)
{
  if (ROVE.lock == ROVE_LOCKED) {
    ERR("Trying to resize under lock.");
    return ROVE_ERROR;
  }
  ROVE.lock = ROVE_LOCKED;
  LOG("Resizing targets.");
  #if defined(ROVE_GL)
    glBindTexture(GL_TEXTURE_2D, (GLuint)((uint64_t)target));
    GLint width, height, format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, 
                             &format);
    glBindTexture(GL_TEXTURE_2D, 0);
    if (format != GL_RGBA8) {
      ERR("Invalid target internal format: %i; must use GL_RGBA8.", format);
      return ROVE_ERROR;
    }
    ROVE.target_width = width;
    ROVE.target_height = height;
    LOG("%u x %u.", ROVE.target_width, ROVE.target_height);
  #elif defined(ROVE_D3D11)
    ID3D11Texture2D* d3d11_tex = (ID3D11Texture2D*)target;
    D3D11_TEXTURE2D_DESC desc;
    d3d11_tex->GetDesc(&desc);
    if (desc.Format != DXGI_FORMAT_R8G8B8A8_UNORM) {
      ERR("Invalid target DXGI_FORMAT: %i; must use R8G8B8A8_UNORM.", 
          desc.Format);
      return ROVE_ERROR;
    }
    ROVE.target_width = desc.Width;
    ROVE.target_height = desc.Height;
    LOG("%u x %u", ROVE.target_width, ROVE.target_height);
  #endif
  if ((ROVE.target_width < 1) || (ROVE.target_width > 16384) ||
      (ROVE.target_height < 1) || (ROVE.target_height > 16384)) {
    ERR("Invalid target dimensions.");
    return ROVE_ERROR;
  }
  ROVE.target = target;
  if (!ResizeTargets()) {
    return ROVE_ERROR;
  }
  ResetFieldOfView();
  ResetRenderAtStart();
  ROVE.lock = ROVE_UNLOCKED;
  return ROVE_SUCCESS;
}
///
RoveStatus RoveStartUpdate()
{
  ROVE.lock = ROVE_LOCKED;
  RoveStatus device_status = ROVE_SUCCESS;
  RoveStatus scene_status = ROVE_SUCCESS;
  uint32_t emitter_count = TREE->emitter_indices.m_h_count;
  uint32_t portal_count = TREE->portal_indices.m_h_count;
  ROVE.scene_thread = std::thread(UpdateScene, &scene_status);
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    if (COMPUTE->reset_at_start) {
      COMPUTE->frame = 1;
      COMPUTE->reset_at_start = false;
    }
    if (!StartDeviceRender(d, emitter_count, portal_count)) {
      ERR("Failed to start device render for device: %u.", d);
      device_status = ROVE_ERROR;
    }
  }
  if (ROVE.scene_thread.joinable()) {
    ROVE.scene_thread.join();
  }
  if ((device_status != ROVE_SUCCESS) ||
      (scene_status != ROVE_SUCCESS)) {
    return ROVE_ERROR;
  }
  if (!UploadScene()) {
    ERR("Failed to upload scene.");
    return ROVE_ERROR;
  }
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    FinishDeviceRender(d);
  }
  if (SCENE->update_flag) {
    TREE->triangles.FlipBuffer();
    TREE->nodes.FlipBuffer();
    SCENE->update_flag = false;
  }
  if (SCENE->update_materials_flag) {
    TREE->materials.FlipBuffer();
    SCENE->update_materials_flag = false;
  }
  if (SCENE->update_emitters_flag) {
    TREE->emitter_indices.FlipBuffer();
    SCENE->update_emitters_flag = false;
  }
  if (SCENE->update_portals_flag) {
    TREE->portal_indices.FlipBuffer();
    SCENE->update_portals_flag = false;
  }
  ROVE.lock = ROVE_UNLOCKED;
  return ROVE_SUCCESS;
}
///
void RoveFinishUpdate()
{
  CompositeFinalImage();
  if (COMPUTE->reset_at_end) {
    COMPUTE->frame = 1;
    COMPUTE->reset_at_end = false;
  }
}
///
void ROVE_EXPORT RoveResetRenderAtStart()
{
  ResetRenderAtStart(); 
}
///
void ROVE_EXPORT RoveResetRenderAtEnd()
{
  ResetRenderAtEnd(); 
}
///
uint32_t RoveGetThreadCount()
{
  return ROVE.thread_count;
}
///
RoveComputeAPI RoveGetComputeAPI()
{
  #if defined(ROVE_CL)
    return ROVE_COMPUTE_API_CL;
  #elif defined(ROVE_CUDA)
    return ROVE_COMPUTE_API_CUDA;
  #endif
}
///
uint32_t RoveGetDeviceCount()
{
  return (COMPUTE->compositing_target_count + 1);
}
///
const char* RoveGetDeviceName(uint32_t device_index)
{
  uint32_t available_device_count = 0;
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    Device* device = &COMPUTE->devices[d];
    if (!device->available) {
      continue;
    } 
    if (available_device_count++ == device_index) {
      return device->name;
    }
  }
  return nullptr;
}
///
RoveStatus RoveImportMesh(uint32_t vertex_count,
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
  if (ROVE.lock == ROVE_UNLOCKED) {
    Mesh* mesh = ImportMesh(vertex_count, triangle_count, has_normals, 
                            has_tangents, has_uvs, vertices, triangles, 
                            normals, tangents, uvs, sub_mesh_count);
    if (!mesh) {
      return ROVE_ERROR;
    }
    ResetRenderAtEnd();
    return (mesh - SCENE->meshes.m_items.m_data);
  } else {
    ERR("Called RoveImportMesh() while Rove was locked.");
    return ROVE_ERROR;
  }
}
///
void RoveFreeMesh(uint32_t mesh_id)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    Mesh* mesh = &SCENE->meshes[mesh_id];
    SetMeshFreeFlag(mesh);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveFreeMesh() while Rove was locked.");
  }
}
///
void RoveDefineSubMesh(uint32_t mesh_id,
                       uint32_t sub_mesh_index,
                       uint32_t triangle_offset,
                       uint32_t triangle_count,
                       uint32_t material_id)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    Mesh* mesh = &SCENE->meshes[mesh_id];
    DefineSubMesh(mesh, sub_mesh_index, triangle_offset, triangle_count,
                  material_id);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveDefineSubMesh() while Rove was locked.");
  }
}
///
void RoveChangeSubMeshMaterial(uint32_t mesh_id,
                               uint32_t sub_mesh_index,
                               uint32_t material_id)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    Mesh* mesh = &SCENE->meshes[mesh_id];
    ChangeSubMeshMaterial(mesh, sub_mesh_index, material_id);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveChangeSubMeshMaterial() while Rove was locked.");
  }
}
///
void RoveSetMeshTransform(uint32_t mesh_id,
                          float* transform)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    Mesh* mesh = &SCENE->meshes[mesh_id];
    SetMeshTransform(mesh, transform);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMeshTransform() while Rove was locked.");
  }
}
///
uint32_t RoveAddMaterial(uint32_t map_flags,
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
  if (ROVE.lock == ROVE_UNLOCKED) {
    Material* material = AddMaterial(map_flags, double_sided, albedo,
                                     metallic, smoothness, glass, emission,
																		 albedo_bounds, normal_bounds,
																		 metallic_bounds, emission_bounds);
    return (material - TREE->materials.m_h_data);
  } else {
    ERR("Called RoveAddMaterial() while Rove was locked.");
    return ROVE_ERROR;
  }
}
///
void RoveSetMaterialMapFlags(uint32_t material_id,
                             uint32_t map_flags)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialMapFlags(material_id, map_flags);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialMapFlags() while Rove was locked.");
  }
}
///
void RoveSetMaterialDoubleSided(uint32_t material_id,
                                int32_t double_sided)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialDoubleSided(material_id, double_sided);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialDoubleSided() while Rove was locked.");
  }
}
///
void RoveSetMaterialAlbedo(uint32_t material_id,
                           float* albedo)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialAlbedo(material_id, albedo);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialAlbedo() while Rove was locked.");
  }
}
///
void RoveSetMaterialMetallic(uint32_t material_id,
                             float metallic)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialMetallic(material_id, metallic);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialMetallic() while Rove was locked.");
  }
}
///
void RoveSetMaterialSmoothness(uint32_t material_id,
                               float smoothness)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialSmoothness(material_id, smoothness);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialSmoothness() while Rove was locked.");
  }
}
///
void RoveSetMaterialGlass(uint32_t material_id,
                          float glass)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialGlass(material_id, glass);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialGlass() while Rove was locked.");
  }
}
///
void RoveSetMaterialEmission(uint32_t material_id,
                             float emission)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialEmission(material_id, emission);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialEmission() while Rove was locked.");
  }
}
///
void RoveSetMaterialAlbedoBounds(uint32_t material_id,
                                 float* bounds)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialAlbedoBounds(material_id, bounds);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialAlbedoBounds() while Rove was locked.");
  }
}
///
void RoveSetMaterialNormalBounds(uint32_t material_id,
                                 float* bounds)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialNormalBounds(material_id, bounds);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialNormalBounds() while Rove was locked.");
  }
}
///
void RoveSetMaterialMetallicBounds(uint32_t material_id,
                                   float* bounds)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialMetallicBounds(material_id, bounds);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialMetallicBounds() while Rove was locked.");
  }
}
///
void RoveSetMaterialEmissionBounds(uint32_t material_id,
                                   float* bounds)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		SetMaterialEmissionBounds(material_id, bounds);
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMaterialEmissionBounds() while Rove was locked.");
  }
}
///
RoveStatus RoveSetAlbedoAtlas(uint8_t* atlas,
                              uint32_t width,
                              uint32_t height)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		if (!SetAlbedoAtlas(atlas, width, height)) {
      return ROVE_ERROR;
    }
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetAlbedoAtlas() while Rove was locked.");
  }
  return ROVE_SUCCESS;
}
///
RoveStatus RoveSetNormalAtlas(uint8_t* atlas,
                              uint32_t width,
                              uint32_t height)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		if (!SetNormalAtlas(atlas, width, height)) {
      return ROVE_ERROR;
    }
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetNormalAtlas() while Rove was locked.");
  }
  return ROVE_SUCCESS;
}
///
RoveStatus RoveSetMetallicAtlas(uint8_t* atlas,
                                uint32_t width,
                                uint32_t height)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		if (!SetMetallicAtlas(atlas, width, height)) {
      return ROVE_ERROR;
    }
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetMetallicAtlas() while Rove was locked.");
  }
  return ROVE_SUCCESS;
}
///
RoveStatus RoveSetEmissionAtlas(uint8_t* atlas,
                                uint32_t width,
                                uint32_t height)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		if (!SetEmissionAtlas(atlas, width, height)) {
      return ROVE_ERROR;
    }
    ResetRenderAtEnd();
  } else {
    ERR("Called RoveSetEmissionAtlas() while Rove was locked.");
  }
  return ROVE_SUCCESS;
}
///
void RoveSetFieldOfView(float field_of_view)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    ROVE.field_of_view = field_of_view;
    ResetFieldOfView();
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetFieldOfView() while Rove was locked.");
  }
}
///
void RoveSetMaxBounces(uint32_t max_bounces)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    ROVE.max_bounces = max_bounces;
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetMaxBounces() while Rove was locked.");
  }
}
///
void RoveSetSamplesPerFrame(uint32_t sample_count)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    ROVE.samples_per_frame = sample_count;
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetSamplesPerFrame() while Rove was locked.");
  }
}
///
void RoveSetImageProperties(int32_t tonemap,
                            float gamma,
                            float exposure,
                            float contrast,
                            float saturation,
                            float brightness)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetImageProperties(tonemap, gamma, exposure, contrast, saturation, 
                       brightness);
  } else {
    ERR("Called RoveSetImageProperties() while Rove was locked.");
  }
}
///
void RoveSetCameraApertureSize(float aperture_size) 
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetCameraApertureSize(aperture_size);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetCameraApertureSize() while Rove was locked.");
  }
}
///
void RoveSetCameraFocalDepth(float focal_depth) 
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetCameraFocalDepth(focal_depth);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetCameraFocalDepth() while Rove was locked.");
  }
}
///
void RoveSetCameraTransform(float* transform) 
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetCameraTransform(transform);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetCameraTransform() while Rove was locked.");
  }
}
///
void ROVE_EXPORT RoveSetEnvironmentType(uint32_t type)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetEnvironmentType(type);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetEnvironmentType() while Rove was locked.");
  }
}
///
void ROVE_EXPORT RoveSetSunDirection(float* direction)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetSunDirection(direction);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetSunDirection() while Rove was locked.");
  }
}
///
void ROVE_EXPORT RoveSetSkyIntensity(float intensity)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetSkyIntensity(intensity);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetSkyIntensity() while Rove was locked.");
  }
}
///
void ROVE_EXPORT RoveSetSunIntensity(float intensity)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetSunIntensity(intensity);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetSunIntensity() while Rove was locked.");
  }
}
///
void ROVE_EXPORT RoveSetSunColor(float* color)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetSunColor(color);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetSunColor() while Rove was locked.");
  }
}
///
int32_t ROVE_EXPORT RoveSetEnvironmentMap(uint8_t* map,
                                          uint32_t width,
                                          uint32_t height)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
		if (!SetEnvironmentMap(map, width, height)) {
      return false;
    }
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetEnvironmentMap() while Rove was locked.");
  }
  return true;
}
///
void ROVE_EXPORT RoveSetEnvironmentProperties(float intensity,
                                              float rotation)
{
  if (ROVE.lock == ROVE_UNLOCKED) {
    SetEnvironmentProperties(intensity, rotation);
    ResetRenderAtStart();
  } else {
    ERR("Called RoveSetEnvironmentProperties() while Rove was locked.");
  }
}
///
#if defined(ROVE_D3D11)
  void RoveSetD3D11Device(ID3D11Device* device)
  {
    rove_d3d_device = device;
  }
#endif
///
UnityRenderingEvent RoveUnitySetupEvent(char* data_path,
                                        int32_t use_multi_device,
                                        int32_t use_integrated_device,
                                        uint32_t optimization_level,
                                        void* target)
{
  std::string path_string(data_path);
  STRING_TO_CHARS(path_string, ROVE.data_path);
  ROVE.use_multi_device = use_multi_device;
  ROVE.use_integrated_device = use_integrated_device;
  ROVE.optimization_level = optimization_level;
  ROVE.target = target;
  return [](int32_t event_type) { RoveSetup(ROVE.data_path, 
                                            ROVE.use_multi_device,
                                            ROVE.use_integrated_device,
                                            ROVE.optimization_level,
                                            ROVE.target); };
}
///
UnityRenderingEvent RoveUnityFinishUpdateEvent()
{
  return [](int32_t event_type) { RoveFinishUpdate(); };
}
///
UnityRenderingEvent RoveUnityResizeEvent(void* target)
{
  ROVE.target = target;
  return [](int32_t event_type) { RoveResize(ROVE.target); };
}
