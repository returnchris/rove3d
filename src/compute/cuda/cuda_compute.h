#if defined(ROVE_CUDA)
#pragma once
#include <random>
#include "../compute.h"
#include "../../rove.h"
#include <cuda.h>
#if defined(ROVE_GL)
  #include <cudaGL.h>
#elif defined(ROVE_D3D11)
  #include <cudaD3D11.h>
#endif
///
#define CHECK_CUDA(F, H) \
{ \
  CUresult result = F; \
  if (result != CUDA_SUCCESS) { \
    const char* error_name; \
    const char* error_string; \
    cuGetErrorName(result, &error_name); \
    cuGetErrorString(result, &error_string); \
    ERR("CUDA error %s: %s", error_name, error_string); \
    H \
  } \
}
///
#define CHECK_NVRTC(F, H) \
{ \
  nvrtcResult result = F; \
  if (result != NVRTC_SUCCESS) { \
    const char* error_string = nvrtcGetErrorString(result); \
    ERR("NVRTC error %u: %s", result, error_string); \
    H \
  } \
}
///
#define CUDA_ERR COMPUTE->error
///
struct Device 
{
  char* name;
  uint32_t id;
  int32_t available;
  int32_t is_integrated;
  CUcontext context;
  CUdevice cu_device;
  CUstream stream;
  CUstream io_stream;
  int32_t max_image_width;
  int32_t max_image_height;
  int32_t max_image_buffer_width;
  CUmodule module;
  CUfunction primary_function;
  CUfunction secondary_function;
  CUfunction nee_function;
  CUfunction env_sky_function;
  CUfunction env_map_function;
  CUfunction emission_function;
  CUfunction sample_function;
  CUfunction accumulate_function;
  CUfunction composite_two_function;
  CUfunction composite_three_function;
  CUfunction composite_four_function;
  CUfunction composite_five_function;
  void* primary_params[17];
  void* secondary_params[14];
  void* nee_params[14];
  void* env_sky_params[6];
  void* env_map_params[6];
  void* emission_params[10];
  void* sample_params[24];
  void* accumulate_params[6];
  void* composite_two_params[5];
  void* composite_three_params[6];
  void* composite_four_params[7];
  void* composite_five_params[8];
  CUarray target_array;
  CUsurfObject target;
  CUarray compositing_target_array;
  CUtexObject compositing_target;
  CUdeviceptr ray_states;
  CUarray albedo_atlas_array;
  CUtexObject albedo_atlas;
  CUarray normal_atlas_array;
  CUtexObject normal_atlas;
  CUarray metallic_atlas_array;
  CUtexObject metallic_atlas;
  CUarray emission_atlas_array;
  CUtexObject emission_atlas;
  CUarray environment_map_array;
  CUtexObject environment_map;
  CUarray sky_table_array;
  CUtexObject sky_table;
};
///
struct Compute 
{
  CUresult error;
  int32_t nvidia_platform;
  int32_t invert_image;
  int32_t first_frame_skip;
  int32_t reset_at_start;
  int32_t reset_at_end;
  uint32_t frame;
  uint32_t device_count;
  uint32_t compositing_target_count;
  std::default_random_engine random_generator;
  std::uniform_real_distribution<float> random_fdist;
  std::uniform_int_distribution<uint32_t> random_udist;
  Device* devices;
  Device* display_device;
  CUgraphicsResource interop_resource;
  CUarray target_array;
  CUsurfObject target;
};
#endif
