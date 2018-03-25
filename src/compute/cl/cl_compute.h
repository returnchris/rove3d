#if defined(ROVE_CL)
#pragma once
///
#include <random>
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#if defined(ROVE_MAC)
  #include <OpenCL/opencl.h>
  #include <OpenCL/cl_gl.h>
  #include <OpenCL/cl_gl_ext.h>
#elif defined(ROVE_LINUX)
  #include <CL/cl_gl.h>
  #include <CL/cl.h>
#elif defined(ROVE_WINDOWS)
  #include <CL/cl.h>
  #if defined(ROVE_GL)
    #include <CL/cl_gl.h>
  #elif defined(ROVE_D3D11)
    #include <CL/cl_d3d11.h>
    #include <CL/cl_d3d11_ext.h>
  #endif
#endif
#include "../compute.h"
///
#define CHECK_CL(F, H) \
{ \
  cl_int result = F; \
  if (result != CL_SUCCESS) { \
    ERR("CL error code: %i", result); \
    H \
  } \
}
///
#define CL_ERR COMPUTE->error
#define CL_GET_EXT clGetExtensionFunctionAddressForPlatform
///
struct Device {
  char* name;
  cl_device_id id;
  int32_t available;
  int32_t is_integrated;
  cl_context context;
  cl_command_queue queue;
  cl_command_queue io_queue;
  cl_ulong max_image_width;
  cl_ulong max_image_height;
  cl_ulong max_image_buffer_width;
  cl_program program;
  cl_kernel primary_kernel;
  cl_kernel secondary_kernel;
  cl_kernel nee_kernel;
  cl_kernel env_sky_kernel;
  cl_kernel env_map_kernel;
  cl_kernel emission_kernel;
  cl_kernel sample_kernel;
  cl_kernel accumulate_kernel;
  cl_kernel composite_two_kernel;
  cl_kernel composite_three_kernel;
  cl_kernel composite_four_kernel;
  cl_kernel composite_five_kernel;
  uint8_t* h_target;
  cl_mem target;
  cl_mem compositing_target;
  cl_mem ray_states;
  cl_mem albedo_atlas;
  cl_mem normal_atlas;
  cl_mem metallic_atlas;
  cl_mem emission_atlas;
  cl_mem environment_map;
  cl_mem sky_table;
};
///
struct Compute {
  cl_int error;
  int32_t nvidia_platform; 
  int32_t invert_image;
  uint32_t first_frame_skip;
  int32_t frame;
  int32_t reset_at_start;
  int32_t reset_at_end;
  cl_platform_id platform;
  uint32_t device_count;
  uint32_t compositing_target_count;
  std::default_random_engine random_generator;
  std::uniform_real_distribution<float> random_fdist;
  std::uniform_int_distribution<uint32_t> random_udist;
  cl_mem* compositing_targets;
  cl_mem target;
  Device* devices;
  Device* display_device;
  #if defined(ROVE_D3D11)
   clEnqueueAcquireD3D11ObjectsNV_fn clEnqueueAcquireD3D11ObjectsNV;
   clEnqueueReleaseD3D11ObjectsNV_fn clEnqueueReleaseD3D11ObjectsNV;
   clEnqueueAcquireD3D11ObjectsKHR_fn clEnqueueAcquireD3D11ObjectsKHR;
   clEnqueueReleaseD3D11ObjectsKHR_fn clEnqueueReleaseD3D11ObjectsKHR;
  #endif
};
#endif
