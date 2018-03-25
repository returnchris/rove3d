#if defined(ROVE_CUDA)
#include <cstdint>
#include "../../rove.h"
#include "cuda_compute.h"
#include "../../scene/scene.h"
#include "../../core/utility.h"
#include "../../rove.h"
#include "../../core/encoder.h"
#include "../../scene/tree.h"
#include "kernels/module.fatbin.h"
///
int32_t SetupCompute()
{
  LOG("Compute API: CUDA.");
  memset(COMPUTE, 0, sizeof(Compute));
  CHECK_CUDA(cuInit(0), 
             {
               ERR("Failed to initialize CUDA.");
               return false;
             });
  int32_t device_count;
  CHECK_CUDA(cuDeviceGetCount(&device_count),
             {
               ERR("Failed to get device count.");
               return false;
             });
  COMPUTE->device_count = device_count;
  LOG("%u device(s) found.", COMPUTE->device_count);
  ROVE.use_multi_device &= (COMPUTE->device_count > 1);
  CUdevice cu_display_device;
  #if !defined(ROVE_MAC)
    #if defined(ROVE_GL)
      uint32_t gl_device_count;
      CHECK_CUDA(cuGLGetDevices(&gl_device_count, &cu_display_device,
                                1, CU_GL_DEVICE_LIST_ALL),
                 {
                   ERR("Failed to get GL display device.");	
                   return false;
                 });
      if (gl_device_count > 1) {
        ERR("Rove3D only supports one display device. If SLI is enabled, "
            "please disable it.");
        return false;        
      }
    #elif defined(ROVE_D3D11)
      if (rove_d3d_device == nullptr) {
        ERR("D3D device missing; please use RoveSetD3D11Device.");
        return ROVE_ERROR;
	    }
      uint32_t d3d_device_count;
      CHECK_CUDA(cuD3D11GetDevices(&d3d_device_count, &cu_display_device, 
                                   1, rove_d3d_device, 
                                   CU_D3D11_DEVICE_LIST_ALL),
                 {
                   ERR("Failed to get D3D display device.");	
                   return false;
                 });
      if (d3d_device_count > 1) {
        ERR("Rove3D only supports one display device. If SLI is enabled, "
            "please disable it.");
        return false;        
      }
    #endif
  #endif
  COMPUTE->devices = (Device*)calloc(COMPUTE->device_count,
                                     sizeof(Device));
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    Device* device = &COMPUTE->devices[d];  
    CHECK_CUDA(cuDeviceGet(&device->cu_device, d),
               {
                 ERR("Failed to get device: %u.", d);
                 LOG("Device will be disabled.");
                 device->available = false;
                 continue;
               });
    CHECK_CUDA(cuCtxCreate(&device->context, 
                           CU_CTX_SCHED_SPIN | CU_CTX_MAP_HOST,
                           device->cu_device),
               {
                 ERR("Failed to create device context.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 continue;
               });
    CHECK_CUDA(cuStreamCreate(&device->stream, CU_STREAM_NON_BLOCKING),
               {
                 ERR("Failed to create device stream.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 continue;
               });
    CHECK_CUDA(cuStreamCreate(&device->io_stream, CU_STREAM_NON_BLOCKING),
               {
                 ERR("Failed to create device i/o stream.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 continue;
               });
    if (!DISPLAY_DEVICE) {
  #if defined(ROVE_MAC)
      LOG("** Warning: Mac with CUDA will assume first available "
          "device\n** as GL display device. For maximum performance, "
          "please ensure\n** that your display is connected to the "
          "first device listed.");
      DISPLAY_DEVICE = device; 
  #else
      if (device->cu_device == cu_display_device) {
        DISPLAY_DEVICE = device;
        LOG("Display device index: %u.", d);
      }
  #endif
    }
  }
  if (!DISPLAY_DEVICE) {
    ERR("Failed to find CUDA-capable display device.");
    return false;
  }
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    Device* device = &COMPUTE->devices[d];  
    if (!ROVE.use_multi_device && (device != DISPLAY_DEVICE)) {
      device->available = false;
      continue;
    }
    device->available = SetupDevice(d);
    if (device->available) {
      device->available = SetupDeviceTargets(d);
    }
    LOG("Availability: %s.", device->available ? "AVAILABLE" : "NOT AVAILABLE");
    if ((device == DISPLAY_DEVICE) && (!device->available)) {
      ERR("Display device is unavailable.");
      return false;
    }
  }
  LOG("---");
  std::random_device rd;
  COMPUTE->random_generator = std::default_random_engine(rd());
  COMPUTE->random_fdist = std::uniform_real_distribution<float>(0.0f, 1.0f);
  COMPUTE->random_udist = std::uniform_int_distribution<uint32_t>(0, UINT32_MAX);
  #if defined(ROVE_D3D11)
    COMPUTE->invert_image = false;
  #else
    COMPUTE->invert_image = false;
  #endif
  COMPUTE->first_frame_skip = true;
  COMPUTE->frame = 1;
  return true;
}
///
void ShutdownCompute()
{
  if (COMPUTE->devices) {
    for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
      ShutdownDeviceTargets(d);
      ShutdownDevice(d);
    }
    free(COMPUTE->devices);
  }
  memset(COMPUTE, 0, sizeof(Compute));
}
///
int32_t SetupDevice(uint32_t index)
{
  Device* device = &COMPUTE->devices[index];
  if ((device != DISPLAY_DEVICE) &&
      (COMPUTE->compositing_target_count >= 4)) {
    ERR("Device is beyond the maximum supported device count (5).");
    LOG("Device will be disabled.");
    device->available = false;
    return false; 
  }
  device->id = index;
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  LOG("--- Device %s: %u.",
    (device == DISPLAY_DEVICE) ? "(Display)" : "", index);
  char name[256];
  CHECK_CUDA(cuDeviceGetName(name, 256, device->cu_device),
             {
               ERR("Failed to get device name.");
             });
  device->name = (char*)malloc(sizeof(char) * strlen(name));
  strcpy(device->name, name);
  LOG("Vendor: NVIDIA.");
  device->is_integrated = false;
  LOG("Name: %s.", device->name);
  int32_t compute_major;
  CHECK_CUDA(cuDeviceGetAttribute(&compute_major, 
                                  CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR,
                                  device->cu_device),
             {
               ERR("Failed to get major compute capability.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  int32_t compute_minor;
  CHECK_CUDA(cuDeviceGetAttribute(&compute_minor, 
                                  CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR,
                                  device->cu_device),
             {
               ERR("Failed to get minor compute capability.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  LOG("Compute capability: %i.%i.", compute_major, compute_minor);
  if (compute_major < 3) {
    ERR("Rove3D CUDA requires compute capability >= 3.0.");
    LOG("Device will be disabled.");
    device->available = false;
    return false;
  }
  CHECK_CUDA(cuCtxSetCacheConfig(CU_FUNC_CACHE_PREFER_L1), 
             {
               ERR("Failed to set cache configuration preference to L1.");
             });
  int32_t sm_count;
  CHECK_CUDA(cuDeviceGetAttribute(&sm_count, 
                                  CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT,
                                  device->cu_device),
             {
               ERR("Failed to get device SM count.");
             });
  LOG("SM Count: %u.", sm_count);
  size_t free_memory, total_memory;
  CHECK_CUDA(cuMemGetInfo(&free_memory, &total_memory),
             {
               ERR("Failed to get device global memory size.");
             });
  LOG("Global memory: %.2f GB.", total_memory / 1073741824.0f);
  if ((total_memory / 1073741824.0f) < 2.0f) {
    LOG("** Warning: Total global memory is below minimum requirement of 2GB.");
  }
  int32_t shared_memory;
  CHECK_CUDA(cuDeviceGetAttribute(&shared_memory, 
                           CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK,
                           device->cu_device),
             {
               ERR("Failed to get device shared memory size.");
             });
  LOG("Shared memory: %.2f KB.", shared_memory / 1024.0f); 
  CHECK_CUDA(cuDeviceGetAttribute(&device->max_image_width, 
                           CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_WIDTH,
                           device->cu_device),
             {
               ERR("Failed to get device maximum image width.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuDeviceGetAttribute(&device->max_image_height, 
                           CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE2D_HEIGHT,
                           device->cu_device),
             {
               ERR("Failed to get device maximum image height.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  LOG("Max image size: %u x %u.", device->max_image_width,
                                  device->max_image_height);
  CHECK_CUDA(cuDeviceGetAttribute(&device->max_image_buffer_width, 
                        CU_DEVICE_ATTRIBUTE_MAXIMUM_TEXTURE1D_LINEAR_WIDTH,
                        device->cu_device),
             {
               ERR("Failed to get device maximum image buffer size.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  device->max_image_buffer_width /= sizeof(float);
  LOG("Max image buffer width: %i.", device->max_image_buffer_width);
  char* s = (char*)module_fatbin;
  uint32_t check = 7;
  while (*s) {
    check = check * 101 + *s++;
  }
  /*
	if ((check != 4088264183)) {
		return false;
	}
  */
  LOG("Creating module.");
  CHECK_CUDA(cuModuleLoadFatBinary(&device->module, module_fatbin),
             {
               ERR("Failed to load module binary.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  LOG("Module loaded successfully, getting functions.");
  CHECK_CUDA(cuModuleGetFunction(&device->primary_function,
                                 device->module, "Primary"),
             {
               ERR("Failed to get primary kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->secondary_function,
                                 device->module, "Secondary"),
             {
               ERR("Failed to get secondary kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->nee_function,
                                 device->module, "NEE"),
             {
               ERR("Failed to get next-event estimation kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->env_sky_function,
                                 device->module, "EnvSky"),
             {
               ERR("Failed to get sky environment kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->env_map_function,
                                 device->module, "EnvMap"),
             {
               ERR("Failed to get environment map kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->emission_function,
                                 device->module, "Emission"),
             {
               ERR("Failed to get emission kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->sample_function,
                                 device->module, "Sample"),
             {
               ERR("Failed to get sample kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CHECK_CUDA(cuModuleGetFunction(&device->accumulate_function,
                                 device->module, "Accumulate"),
             {
               ERR("Failed to get accumulate kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  if (device == DISPLAY_DEVICE) {
    CHECK_CUDA(cuModuleGetFunction(&device->composite_two_function,
                                   device->module, "CompositeTwo"),
               {
                 ERR("Failed to get composite two kernel.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    CHECK_CUDA(cuModuleGetFunction(&device->composite_three_function,
                                   device->module, "CompositeThree"),
               {
                 ERR("Failed to get composite three kernel.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    CHECK_CUDA(cuModuleGetFunction(&device->composite_four_function,
                                   device->module, "CompositeFour"),
               {
                 ERR("Failed to get composite four kernel.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    CHECK_CUDA(cuModuleGetFunction(&device->composite_five_function,
                                   device->module, "CompositeFive"),
               {
                 ERR("Failed to get composite five kernel.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
  }
  LOG("Functions loaded successfully.");
  return true;
}
///
void ShutdownDevice(uint32_t index)
{
  LOG("Shutting down device: %u.", index);
  Device* device = &COMPUTE->devices[index];
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current.");
               return;
             });
  if (device->albedo_atlas_array) {
    CHECK_CUDA(cuArrayDestroy(device->albedo_atlas_array), {});
  }
  if (device->albedo_atlas) {
    CHECK_CUDA(cuTexObjectDestroy(device->albedo_atlas), {});
  }
  if (device->normal_atlas_array) {
    CHECK_CUDA(cuArrayDestroy(device->normal_atlas_array), {});
  }
  if (device->normal_atlas) {
    CHECK_CUDA(cuTexObjectDestroy(device->normal_atlas), {});
  }
  if (device->metallic_atlas_array) {
    CHECK_CUDA(cuArrayDestroy(device->metallic_atlas_array), {});
  }
  if (device->metallic_atlas) {
    CHECK_CUDA(cuTexObjectDestroy(device->metallic_atlas), {});
  }
  if (device->emission_atlas_array) {
    CHECK_CUDA(cuArrayDestroy(device->emission_atlas_array), {});
  }
  if (device->emission_atlas) {
    CHECK_CUDA(cuTexObjectDestroy(device->emission_atlas), {});
  }
  if (device->environment_map_array) {
    CHECK_CUDA(cuArrayDestroy(device->environment_map_array), {});
  }
  if (device->environment_map) {
    CHECK_CUDA(cuTexObjectDestroy(device->environment_map), {});
  }
  if (device->module) {
    CHECK_CUDA(cuModuleUnload(device->module), {});
  }
  if (device->stream) {
    CHECK_CUDA(cuStreamDestroy(device->stream), {});
  }
  if (device->io_stream) {
    CHECK_CUDA(cuStreamDestroy(device->io_stream), {});
  }
  if (device->context) {
    CHECK_CUDA(cuCtxDestroy(device->context), {});
  }
  if (device->name) {
    free(device->name);
  }
  memset(device, 0, sizeof(Device));
}
///
int32_t SetupDeviceTargets(uint32_t index)
{
  Device* device = &COMPUTE->devices[index];
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  if (device == DISPLAY_DEVICE) {
    #if defined(ROVE_GL)
      LOG("Creating target on display DISPLAY_DEVICE from GL texture.");
      CHECK_CUDA(cuGraphicsGLRegisterImage(&COMPUTE->interop_resource, 
                                           (GLuint)((uint64_t)ROVE.target),
                                           GL_TEXTURE_2D, 
                               CU_GRAPHICS_REGISTER_FLAGS_WRITE_DISCARD |
                               CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST),
                 {
                   ERR("Failed to create interop resource from GL texture.");
                   LOG("Device will be disabled.");
                   DISPLAY_DEVICE->available = false;
                   return false;
                 });
    #elif defined(ROVE_D3D11)
      LOG("Creating target on display DISPLAY_DEVICE from D3D11 texture.");
      
      CHECK_CUDA(cuGraphicsD3D11RegisterResource(&COMPUTE->interop_resource, 
                                          (ID3D11Texture2D*)ROVE.target,
                               CU_GRAPHICS_REGISTER_FLAGS_SURFACE_LDST),
                 {
                   ERR("Failed to create interop resource from D3D11 texture.");
                   LOG("Device will be disabled.");
                   DISPLAY_DEVICE->available = false;
                   return false;
                 });
    #endif
    CHECK_CUDA(cuGraphicsMapResources(1, &COMPUTE->interop_resource, 
                                      DISPLAY_DEVICE->stream),
               {
                 ERR("Failed to map interop resource.");
                 LOG("Device will be disabled.");
                 DISPLAY_DEVICE->available = false;
                 return false;
               });
    CHECK_CUDA(cuGraphicsSubResourceGetMappedArray(&COMPUTE->target_array, 
                                                   COMPUTE->interop_resource,
                                                   0, 0),
               {
                 ERR("Failed to get array from interop resource.");
                 LOG("Device will be disabled.");
                 DISPLAY_DEVICE->available = false;
                 return false;
               });
    CUDA_RESOURCE_DESC resource_desc;
    memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
    resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
    resource_desc.res.array.hArray = COMPUTE->target_array;
    CHECK_CUDA(cuSurfObjectCreate(&COMPUTE->target, &resource_desc),
               {
                 ERR("Failed to create interop target surface.");
                 LOG("Device will be disabled.");
                 DISPLAY_DEVICE->available = false;
                 return false;
               });
    CHECK_CUDA(cuGraphicsUnmapResources(1, &COMPUTE->interop_resource, 
                                        DISPLAY_DEVICE->stream),
               {
                 ERR("Failed to unmap interop resource.");
                 LOG("Device will be disabled.");
                 DISPLAY_DEVICE->available = false;
                 return false;
               });
    DISPLAY_DEVICE->composite_two_params[2] = &COMPUTE->target;
    DISPLAY_DEVICE->composite_two_params[3] = &ROVE.target_width;
    DISPLAY_DEVICE->composite_two_params[4] = &ROVE.target_height;
    DISPLAY_DEVICE->composite_three_params[3] = &COMPUTE->target;
    DISPLAY_DEVICE->composite_three_params[4] = &ROVE.target_width;
    DISPLAY_DEVICE->composite_three_params[5] = &ROVE.target_height;
    DISPLAY_DEVICE->composite_four_params[4] = &COMPUTE->target;
    DISPLAY_DEVICE->composite_four_params[5] = &ROVE.target_width;
    DISPLAY_DEVICE->composite_four_params[6] = &ROVE.target_height;
    DISPLAY_DEVICE->composite_five_params[5] = &COMPUTE->target;
    DISPLAY_DEVICE->composite_five_params[6] = &ROVE.target_width;
    DISPLAY_DEVICE->composite_five_params[7] = &ROVE.target_height;
  }
  LOG("Creating device target.");
  CUDA_ARRAY_DESCRIPTOR array_desc;
  memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
  array_desc.Width = ROVE.target_width;
  array_desc.Height = ROVE.target_height;
  array_desc.Format = CU_AD_FORMAT_UNSIGNED_INT8;
  array_desc.NumChannels = 4;
  CHECK_CUDA(cuArrayCreate(&device->target_array, &array_desc),
             {
               ERR("Failed to allocate device target array.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  CUDA_RESOURCE_DESC resource_desc;
  memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
  resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
  resource_desc.res.array.hArray = device->target_array;
  CHECK_CUDA(cuSurfObjectCreate(&device->target, &resource_desc),
             {
               ERR("Failed to create device target surface.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  if (device == DISPLAY_DEVICE) {
    LOG("Setting display device target as root compositing target.");
    DISPLAY_DEVICE->composite_two_params[0] = &DISPLAY_DEVICE->target;
    DISPLAY_DEVICE->composite_three_params[0] = &DISPLAY_DEVICE->target;
    DISPLAY_DEVICE->composite_four_params[0] = &DISPLAY_DEVICE->target;
    DISPLAY_DEVICE->composite_five_params[0] = &DISPLAY_DEVICE->target;
  } else {
    LOG("Creating device compositing target on display device.");
    CHECK_CUDA(cuCtxSetCurrent(DISPLAY_DEVICE->context),
               {
                 ERR("Failed to set display device context as current.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    CHECK_CUDA(cuArrayCreate(&device->compositing_target_array, &array_desc),
               {
                 ERR("Failed to allocate device compositing target array.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
	  memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
    resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
    resource_desc.res.array.hArray = device->compositing_target_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_CLAMP; 
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_CLAMP; 
    texture_desc.filterMode = CU_TR_FILTER_MODE_POINT;
    texture_desc.flags = CU_TRSF_READ_AS_INTEGER;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = ROVE.target_width;
    view_desc.height = ROVE.target_height;
    view_desc.format = CU_RES_VIEW_FORMAT_UINT_4X8;
    CHECK_CUDA(cuTexObjectCreate(&device->compositing_target, &resource_desc,
      													 &texture_desc, &view_desc),
               {
                 ERR("Failed to create device compositing target texture.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
	  CHECK_CUDA(cuCtxSetCurrent(device->context),
               {
                 ERR("Failed to set device context as current.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    ++COMPUTE->compositing_target_count;
    LOG("Compositing target index: %u.", COMPUTE->compositing_target_count);
    if (COMPUTE->compositing_target_count == 1) {
      DISPLAY_DEVICE->composite_two_params[1] = &device->compositing_target;
      DISPLAY_DEVICE->composite_three_params[1] = &device->compositing_target;
      DISPLAY_DEVICE->composite_four_params[1] = &device->compositing_target;
      DISPLAY_DEVICE->composite_five_params[1] = &device->compositing_target;
    } else if (COMPUTE->compositing_target_count == 2) {
      DISPLAY_DEVICE->composite_three_params[2] = &device->compositing_target;
      DISPLAY_DEVICE->composite_four_params[2] = &device->compositing_target;
      DISPLAY_DEVICE->composite_five_params[2] = &device->compositing_target;
    } else if (COMPUTE->compositing_target_count == 3) {
      DISPLAY_DEVICE->composite_four_params[3] = &device->compositing_target;
      DISPLAY_DEVICE->composite_five_params[3] = &device->compositing_target;
    } else if (COMPUTE->compositing_target_count == 4) {
      DISPLAY_DEVICE->composite_five_params[4] = &device->compositing_target;
    }
  }
  uint32_t ray_count = ROVE.target_width * ROVE.target_height;
  float ray_count_mb = (ray_count * sizeof(RayState)) / 1048576.0f;
  LOG("Allocating device ray states: %.2f MB.", ray_count_mb);
  CHECK_CUDA(cuMemAlloc(&device->ray_states, sizeof(RayState) * ray_count),
             {
               ERR("Failed to allocate device ray states.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  device->primary_params[0] = &device->ray_states;
  device->primary_params[13] = &ROVE.target_width;
  device->primary_params[14] = &ROVE.target_height;
  device->primary_params[15] = &COMPUTE->invert_image;
  device->secondary_params[0] = &device->ray_states;
  device->secondary_params[12] = &ROVE.target_width;
  device->secondary_params[13] = &ROVE.target_height;
  device->nee_params[0] = &device->ray_states;
  device->nee_params[12] = &ROVE.target_width;
  device->nee_params[13] = &ROVE.target_height;
  device->env_sky_params[0] = &device->ray_states;
  device->env_sky_params[3] = &ROVE.target_width;
  device->env_sky_params[4] = &ROVE.target_height;
  device->env_map_params[0] = &device->ray_states;
  device->env_map_params[3] = &ROVE.target_width;
  device->env_map_params[4] = &ROVE.target_height;
  device->emission_params[0] = &device->ray_states;
  device->emission_params[7] = &ROVE.target_width;
  device->emission_params[8] = &ROVE.target_height;
  device->sample_params[0] = &device->ray_states;
  device->sample_params[21] = &ROVE.target_width;
  device->sample_params[22] = &ROVE.target_height;
  device->accumulate_params[0] = &device->ray_states;
  device->accumulate_params[1] = &device->target;
  device->accumulate_params[3] = &ROVE.target_width;
  device->accumulate_params[4] = &ROVE.target_height;
  return true;
}
///
void ShutdownDeviceTargets(uint32_t index)
{
  LOG("Shutting down device targets for device: %u.", index);
  Device* device = &COMPUTE->devices[index];
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current.");
               return;
             });
  if (device->target) {
    CHECK_CUDA(cuSurfObjectDestroy(device->target), {});
  }
  if (device->target_array) {
    CHECK_CUDA(cuArrayDestroy(device->target_array), {});
  }
  if (device == DISPLAY_DEVICE) {
    if (COMPUTE->interop_resource) {
      CHECK_CUDA(cuGraphicsUnregisterResource(COMPUTE->interop_resource),
                 {});
    }
    if (COMPUTE->target) {
      CHECK_CUDA(cuSurfObjectDestroy(COMPUTE->target), {});
    }
  }
  if (device->compositing_target) {
    CHECK_CUDA(cuSurfObjectDestroy(device->compositing_target), {});
    --COMPUTE->compositing_target_count;
  }
  if (device->compositing_target_array) {
    CHECK_CUDA(cuArrayDestroy(device->compositing_target_array), {});
  }
  if (device->ray_states) {
    CHECK_CUDA(cuMemFree(device->ray_states), {});
  }
}
///
int32_t ResizeTargets()
{
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    Device* device = &COMPUTE->devices[d];
    if (!device->available) {
      continue;
    }
    ShutdownDeviceTargets(d);
    if (!SetupDeviceTargets(d)) {
      ERR("Failed to resize targets on device: %u.", d);
      return false;
    }
  }
  LOG("---");
  return true;
}
///
int32_t SetupSky()
{
  LOG("Setting up physical sky.");
  std::string sky_path = std::string(ROVE.data_path) + "/sky.data";
  FILE* table_file = fopen(sky_path.c_str(), "rb");
  if (!table_file) {
    ERR("Failed to open sky data file.");
    return false;
  }
  uint32_t elevation = 3;
  if (fseek(table_file, 256 * 128 * 4 * elevation * sizeof(uint16_t), 0)) {
    ERR("Failed to seek sky data file.");
    return false;
  }
  uint16_t* table = (uint16_t*)malloc(sizeof(uint16_t) * 256 * 128 * 4);
  size_t length = fread(table, sizeof(uint16_t), 256 * 128 * 4, table_file);
  if (length != 256 * 128 * 4) {
    ERR("Failed to read sky data.");
    free(table);
    fclose(table_file);
    return false;
  }
  fclose(table_file);
  for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
    Device* device = &COMPUTE->devices[d];
    if (!device->available) {
      continue;
    }
    CHECK_CUDA(cuCtxSetCurrent(device->context),
               {
                 ERR("Failed to set device context as current.");
                 return false;
               });
    LOG("Allocating physical sky data on device: %u.", d);
    CUDA_ARRAY_DESCRIPTOR array_desc;
    memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
    array_desc.Width = 256;
    array_desc.Height = 128;
    array_desc.Format = CU_AD_FORMAT_HALF;
    array_desc.NumChannels = 4;
    CHECK_CUDA(cuArrayCreate(&device->sky_table_array, &array_desc),
               {
                 ERR("Failed to allocate sky table array.");
                 return false;
               });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = table;
    host_to_device.srcPitch = sizeof(uint16_t) * 4 * 256;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->sky_table_array;
    host_to_device.Height = 128;
    host_to_device.WidthInBytes = sizeof(uint16_t) * 4 * 256;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy sky table to device.");
                 return false;
               });
    CUDA_RESOURCE_DESC resource_desc;
    memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
    resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
    resource_desc.res.array.hArray = device->sky_table_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = 256;
    view_desc.height = 128;
    view_desc.format = CU_RES_VIEW_FORMAT_FLOAT_4X16;
    CHECK_CUDA(cuTexObjectCreate(&device->sky_table, &resource_desc,
                                 &texture_desc, &view_desc),
               {
                 ERR("Failed to create sky table texture object.");
                 return false;
               });
    device->env_sky_params[1] = &device->sky_table;
    device->env_sky_params[2] = &SCENE->sun;
    device->sample_params[9] = &device->sky_table;
    device->sample_params[10] = &SCENE->sun;
  }
  free(table);
  return true;
}
///
void ResetFieldOfView()
{
  LOG("Resetting the field of view.");
	float w = (float)ROVE.target_width;
  float h = (float)ROVE.target_height;
  float near_plane = 0.0001f;
  float far_plane = 100000.0f;
  float f = 1.0f / tanf(ROVE.field_of_view * PI / 360.0f);
  float aspect = w / h;
  float d = 1.0f / (near_plane - far_plane);
  float a = (near_plane + far_plane) * d;
  float b = 2.0f * near_plane * far_plane * d;
  float pv[16];
  pv[0] = f / aspect; pv[4] = 0.0f; pv[8] = -0.0f;  pv[12] = 0.0f;
  pv[1] = 0.0f;       pv[5] = f;    pv[9] = -0.0f;  pv[13] = 0.0f;
  pv[2] = 0.0f;       pv[6] = 0.0f; pv[10] = -a;   pv[14] = b;
  pv[3] = 0.0f;       pv[7] = 0.0f; pv[11] = 1.0f; pv[15] = 0.0f;
  float to_world[16];
  INVERT_MATRIX(pv, to_world);
  float move[16], scale[16];
  move[0] = 1.0f; move[4] = 0.0f; move[8] = 0.0f;   move[12] = -1.0f;
  move[1] = 0.0f; move[5] = 1.0f; move[9] = 0.0f;   move[13] = -1.0f;
  move[2] = 0.0f; move[6] = 0.0f; move[10] = 1.0f;  move[14] = 0.0f;
  move[3] = 0.0f; move[7] = 0.0f; move[11] = 0.0f;  move[15] = 1.0f;
  float sx = 2.0f / w;
  float sy = 2.0f / h;
  scale[0] = sx;   scale[4] = 0.0f; scale[8] = 0.0f;   scale[12] = 0.0f;
  scale[1] = 0.0f; scale[5] = sy;   scale[9] = 0.0f;   scale[13] = 0.0f;
  scale[2] = 0.0f; scale[6] = 0.0f; scale[10] = 0.0f;  scale[14] = 0.0f;
  scale[3] = 0.0f; scale[7] = 0.0f; scale[11] = 0.0f;  scale[15] = 1.0f;
  float temp[16];
  MULTIPLY_MATRIX(to_world, move, temp);
  MULTIPLY_MATRIX(temp, scale, to_world);
  float root_x, root_y, offset_x, offset_y;
  root_x = to_world[12];
  root_y = to_world[13];
  offset_x = (to_world[0] + to_world[4] + to_world[12]);
  offset_y = (to_world[1] + to_world[5] + to_world[13]);
  SCENE->camera.root_pixel[0] = root_x;
  SCENE->camera.root_pixel[1] = root_y;
  SCENE->camera.pixel_offset[0] = (offset_x - root_x);
  SCENE->camera.pixel_offset[1] = (offset_y - root_y);
  SCENE->camera.image_plane_distance =
    w / (2.0f * tan(ROVE.field_of_view * PI / 360.0f));
}
#endif
