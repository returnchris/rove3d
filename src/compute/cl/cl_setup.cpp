#if defined(ROVE_CL)
#include <string>
#include <vector>
#include <sstream>
#include "../../rove.h"
#include "../../scene/scene.h"
#include "../../core/utility.h"
#include "../../core/encoder.h"
#include "kernels/program.cl.h"
#include "cl_compute.h"
///
int32_t SetupCompute()
{
  LOG("Compute API: OpenCL.");
  memset(COMPUTE, 0, sizeof(Compute));
  cl_uint platform_count = 0;
  CHECK_CL(clGetPlatformIDs(0, nullptr, &platform_count),
           {
             ERR("Failed to get system platforms.");
             return false;
           });
  LOG("%u platform(s) found.", platform_count);
  cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) *
                                                      platform_count);
  cl_device_id* device_ids = nullptr;
  CHECK_CL(clGetPlatformIDs(platform_count, platforms, nullptr),
           {
             ERR("Failed to get system platforms.");
             free(platforms);
             return false;
           });
  int32_t found_platform = false;
  for (cl_uint p = 0; p < platform_count; ++p) {
    LOG("--- Platform: %u.", p);
		char vendor[256], name[256], version[256];
    CL_ERR = clGetPlatformInfo(platforms[p], CL_PLATFORM_VENDOR, 256, vendor, 
       										     nullptr);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to get vendor for platform.");
             });
    LOG("Vendor: %s.", vendor);
    CL_ERR = clGetPlatformInfo(platforms[p], CL_PLATFORM_NAME, 256, name, 
                               nullptr);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to get name for platform.");
             });
    LOG("Name: %s.", name);
    CL_ERR = clGetPlatformInfo(platforms[p], CL_PLATFORM_VERSION, 256, version, 
        											 nullptr);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to get OpenCL version for platform.");
               continue;
             });
    std::vector<std::string> tokens;
    std::string version_string(version);
    std::stringstream ss;
    ss.str(version_string);
    std::string item;
    while (std::getline(ss, item, ' ')) {
      tokens.push_back(item);
    }
    float version_float = std::stof(tokens[1]);
    LOG("OpenCL Version: %f (%s).", version_float, version);
    if (version_float < 1.2f) {
      ERR("Rove3D requires OpenCL 1.2 or greater; if a successful platform is "
          "not found, try updating your GPU driver.");
      continue;
    }
    if (strstr(vendor, "NVIDIA") != nullptr) {
      COMPUTE->nvidia_platform = true;
      LOG("NVIDIA platform detected.");
    } else {
      COMPUTE->nvidia_platform = false;
      LOG("KHR platform detected.");
    }
    cl_uint device_count;
    CL_ERR = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_GPU, 0, nullptr,
                            &device_count);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to get GPU devices for platform.");
               continue;
             });
    if (device_ids) {
      free(device_ids);
    }
    device_ids = (cl_device_id*)malloc(sizeof(cl_device_id) * device_count);
    CL_ERR = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_GPU, device_count,
                            device_ids, nullptr);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to get GPU devices for platform.");
               continue;
             });
    COMPUTE->device_count = device_count;
    LOG("%u device(s) found.", COMPUTE->device_count);
    if (COMPUTE->devices) {
      free(COMPUTE->devices);
    }
	  COMPUTE->devices = (Device*)calloc(COMPUTE->device_count, sizeof(Device));
    #if defined(ROVE_GL)
      #if defined(ROVE_LINUX)
        cl_context_properties props[] = {
          CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[p], 
          CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
          CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
          0 };
      #elif defined(ROVE_WINDOWS)
        cl_context_properties props[] = {
          CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[p], 
          CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
          CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
          0 };
      #elif defined(ROVE_MAC)
        CGLContextObj gl_context = CGLGetCurrentContext();
        CGLShareGroupObj sharegroup= CGLGetShareGroup(gl_context);
        cl_context_properties props[] = {
          CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[p], 
          CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
          (cl_context_properties)sharegroup,
          0 };
      #endif
      #if defined(ROVE_MAC)
        cl_device_id display_device_id = nullptr;
        cl_context temp_context = clCreateContext(props, 0, nullptr, 0, 0, 
                                                  &CL_ERR);
        CHECK_CL(CL_ERR,
                 {
                   ERR("Failed to create temporary context to find GL context.");
                   continue;
                 });
        CL_ERR = clGetGLContextInfoAPPLE(temp_context, gl_context, 
                                  CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE, 
                                  sizeof(cl_device_id), &display_device_id, 
                                  nullptr);
        CHECK_CL(CL_ERR,
                 {
                   ERR("Failed to get display device or active GL context.");
                   clReleaseContext(temp_context); 
                   continue;
                 });
        if (!display_device_id) {
          ERR("Failed to get display device or active GL context.");
          clReleaseContext(temp_context); 
          continue;
        }
        clReleaseContext(temp_context); 
      #else
        cl_device_id display_device_id = nullptr;
        clGetGLContextInfoKHR_fn GetGLContextInfo =
         (clGetGLContextInfoKHR_fn)CL_GET_EXT(platforms[p], 
                                              "clGetGLContextInfoKHR");
        if (!GetGLContextInfo) {
          ERR("Could not get clGetCLContextInfoKHR function address.");
          continue;
        }
        CL_ERR = GetGLContextInfo(props, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, 
                                  sizeof(cl_device_id), &display_device_id, 
                                  nullptr);
        CHECK_CL(CL_ERR,
                 {
                   ERR("Failed to get display device or active GL context.");
                   continue;
                 });
        if (!display_device_id) {
          ERR("Failed to get display device or active GL context.");
          continue;
        }
      #endif
      for (cl_uint d = 0; d < COMPUTE->device_count; ++d) {
        COMPUTE->devices[d].id = device_ids[d];
        if (device_ids[d] == display_device_id) {
          DISPLAY_DEVICE = &COMPUTE->devices[d];
          LOG("Display device index: %u.", d);
        }
      }
      if (!DISPLAY_DEVICE) {
        ERR("Failed to match display device with platform.");
        continue;
      }
      LOG("Creating GL sharing display device context.");
      DISPLAY_DEVICE->context = clCreateContext(props, 1, &display_device_id, 
                                                nullptr, nullptr, &CL_ERR);
      CHECK_CL(CL_ERR,
               {
                 ERR("Failed to create display device context.");
                 continue;
               });
    #elif defined(ROVE_D3D11)
      cl_device_id display_device_id = nullptr;
      clGetDeviceIDsFromD3D11KHR_fn GetD3DContextInfo;
      cl_context_properties props[5];
      props[0] = CL_CONTEXT_PLATFORM; 
      props[1] = (cl_context_properties)platforms[p];
	  if (rove_d3d_device == nullptr) {
        ERR("D3D device missing; please use RoveSetD3D11Device.");
        return ROVE_ERROR;
	  }
      props[3] = (cl_context_properties)rove_d3d_device;
      props[4] = 0;
      if (COMPUTE->nvidia_platform) {
        LOG("Using NV-specific D3D11 extensions.");
        GetD3DContextInfo =
          (clGetDeviceIDsFromD3D11NV_fn)CL_GET_EXT(platforms[p], 
                                                   "clGetDeviceIDsFromD3D11NV");
        props[2] = CL_CONTEXT_D3D11_DEVICE_NV;
        COMPUTE->clEnqueueAcquireD3D11ObjectsNV =
          (clEnqueueAcquireD3D11ObjectsNV_fn)CL_GET_EXT(platforms[p], 
                                               "clEnqueueAcquireD3D11ObjectsNV");
        COMPUTE->clEnqueueReleaseD3D11ObjectsNV = 
          (clEnqueueReleaseD3D11ObjectsNV_fn)CL_GET_EXT(platforms[p], 
                                               "clEnqueueReleaseD3D11ObjectsNV");
        if (!COMPUTE->clEnqueueAcquireD3D11ObjectsNV) {
          ERR("Failed to get clEnqueueAcquireD3D11Objects function address.");
          continue;
        }
        if (!COMPUTE->clEnqueueReleaseD3D11ObjectsNV) {
          ERR("Failed to get clEnqueueReleaseD3D11Objects function address.");
          continue;
        }
      } else {
        LOG("Using KHR-specific D3D11 extensions.");
        GetD3DContextInfo = 
          (clGetDeviceIDsFromD3D11KHR_fn)CL_GET_EXT(platforms[p], 
                                "clGetDeviceIDsFromD3D11KHR");
        props[2] = CL_CONTEXT_D3D11_DEVICE_KHR;
        COMPUTE->clEnqueueAcquireD3D11ObjectsKHR = 
          (clEnqueueAcquireD3D11ObjectsKHR_fn)CL_GET_EXT(platforms[p], 
                              "clEnqueueAcquireD3D11ObjectsKHR");
        COMPUTE->clEnqueueReleaseD3D11ObjectsKHR = 
          (clEnqueueReleaseD3D11ObjectsKHR_fn)CL_GET_EXT(platforms[p], 
                              "clEnqueueReleaseD3D11ObjectsKHR");
        if (!COMPUTE->clEnqueueAcquireD3D11ObjectsKHR) {
          ERR("Failed to get clEnqueueAcquireD3D11Objects function address.");
          continue;
        }
        if (!COMPUTE->clEnqueueReleaseD3D11ObjectsKHR) {
          ERR("Failed to get clEnqueueReleaseD3D11Objects function address.");
          continue;
        }
      }
      if (!GetD3DContextInfo) {
        ERR("Failed to get clGetDeviceIDsFromD3D11 function address.");
        continue;
      }
      CHECK_CL(GetD3DContextInfo(platforms[p],
                                 CL_D3D11_DEVICE_NV, rove_d3d_device,
                                 CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, 
                                 &display_device_id, nullptr),
               {
                 ERR("Failed to get display device or active D3D11 device.");
                 continue;
               });
      for (cl_uint d = 0; d < COMPUTE->device_count; ++d) {
        COMPUTE->devices[d].id = device_ids[d];
        if (device_ids[d] == display_device_id) {
          DISPLAY_DEVICE = &COMPUTE->devices[d];
          LOG("Display device index: %u.", d);
        }
      }
      LOG("Creating D3D sharing display device context.");
      DISPLAY_DEVICE->context = clCreateContext(props, 1, &display_device_id, 
                                                nullptr, nullptr, &CL_ERR);
      CHECK_CL(CL_ERR, 
		       {
                 ERR("Failed to create context.");
                 continue;
	           });
    #endif
    COMPUTE->platform = platforms[p];
    found_platform = true;
    break;
  }
  LOG("---");
  if (device_ids) {
    free(device_ids);
  }
  if (platforms) {
    free(platforms);
  }
  if (!found_platform) {
    ERR("Failed to find a supported platform.");
    return false;
  }
  ROVE.use_multi_device &= (COMPUTE->device_count > 1);
  for (cl_uint d = 0; d < COMPUTE->device_count; ++d) {
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
    for (cl_uint d = 0; d < COMPUTE->device_count; ++d) {
      Device* device = &COMPUTE->devices[d];
      if (!ROVE.use_multi_device && (device != DISPLAY_DEVICE)) { 
        continue;
      }
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
      COMPUTE->compositing_target_count >= 4) {
    ERR("Device is beyond the maximum supported device count (5)."); 
    LOG("Device will be disabled.");
    device->available = false;
    return false;
  }
  LOG("--- Device %s: %u.", 
      (device == DISPLAY_DEVICE) ? "(Display)" : "", index);
  char vendor[256], name[256], version[256], ext[1024];
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_VENDOR, 256, vendor, nullptr),
           {
             ERR("Failed to get device vendor.");
           });
  LOG("Vendor: %s.", vendor);
  if (strstr(vendor, "Intel") != nullptr) {
    LOG("Integrated GPU detected.");
    device->is_integrated = true;
    if(!ROVE.use_integrated_device) {
      LOG("Integrated device detected, but is optionally disabled.");
      device->available = false;
      return false;
    }
  } else {
    LOG("Discrete GPU detected.");
    device->is_integrated = false;
  }
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_NAME, 256, name, nullptr),
           {
             ERR("Failed to get device name.");
           });
  size_t name_length = strlen(name);
  device->name = (char*)malloc(sizeof(char) * name_length + 1);
  strncpy(device->name, name, name_length);
  device->name[name_length] = '\0';
  LOG("Name: %s.", device->name);
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_VERSION, 256, version, nullptr),
           {
             ERR("Failed to get device version.");
           });
	std::vector<std::string> tokens;
	std::string version_string(version);
	std::stringstream ss;
	ss.str(version_string);
	std::string item;
	while (std::getline(ss, item, ' ')) {
		tokens.push_back(item);
	}
	float version_float = std::stof(tokens[1]);
	LOG("OpenCL Version: %f (%s).", version_float, version);
  if (version_float < 1.2f) {
    ERR("Rove3D requires OpenCL 1.2 or greater on device.");
    LOG("Device will be disabled.");
    device->available = false;
    return false;
  }
  cl_uint compute_units;
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_MAX_COMPUTE_UNITS, 
                           sizeof(cl_uint), &compute_units, nullptr),
           {
             ERR("Failed to get device compute units.");
           });
  LOG("Compute units: %u.", compute_units);
  cl_ulong global_memory;
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_GLOBAL_MEM_SIZE, 
                           sizeof(cl_ulong), &global_memory, nullptr),
           {
             ERR("Failed to get device global memory size.");
           });
  LOG("Global memory: %.2f GB.", global_memory / 1073741824.0f);
  if ((global_memory / 1073741824.0f) < 2.0f) {
    LOG("** Warning: Total global memory is below minimum requirement of 2GB.");
  }
  cl_ulong local_memory;
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_LOCAL_MEM_SIZE, 
                           sizeof(cl_ulong), &local_memory, nullptr),
           {
             ERR("Failed to get device local memory size.");
           });
  LOG("Local memory: %.2f KB.", local_memory / 1024.0f);
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_EXTENSIONS, 1024, 
                           &ext, nullptr),
           {
             ERR("Failed to get device extensions.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_IMAGE2D_MAX_WIDTH, 
                           sizeof(cl_ulong), &device->max_image_width, nullptr),
           {
             ERR("Failed to get device maximum image width.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_IMAGE2D_MAX_HEIGHT, 
                           sizeof(cl_ulong), &device->max_image_height, nullptr),
           {
             ERR("Failed to get device maximum image height.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  LOG("Max image size: %llu x %llu.", 
    (unsigned long long)device->max_image_width, 
    (unsigned long long)device->max_image_height);
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, 
                           sizeof(cl_ulong), &device->max_image_buffer_width, 
                           nullptr),
           {
             ERR("Failed to get device maximum image buffer size.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->max_image_buffer_width /= sizeof(float);
  LOG("Max image buffer width: %u.", (uint32_t)device->max_image_buffer_width);
  CHECK_CL(clGetDeviceInfo(device->id, CL_DEVICE_AVAILABLE, sizeof(int32_t), 
                           &device->available, nullptr),
           {
             ERR("Failed to get device availability.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  if (!device->available) {
    LOG("Device is not available and will be disabled.");
    device->available = false;
    return false;
  }
  if (device == DISPLAY_DEVICE) {
    #if defined(ROVE_GL)
      if((strstr((const char*)ext, "cl_khr_gl_sharing") == nullptr) &&
         (strstr((const char*)ext, "cl_APPLE_gl_sharing") == nullptr)) {
        ERR("Display device does not support CL/GL sharing.");
        LOG("Extensions: %s", ext);
        return false; 
      }
    #elif defined(ROVE_D3D11)
      if((strstr((const char*)ext, "cl_nv_d3d11_sharing")) == nullptr &&
         (strstr((const char*)ext, "cl_khr_d3d11_sharing") == nullptr)) {
        ERR("Display device does not support CL/D3D11 sharing.");
        LOG("Extensions: %s", ext);
        return false; 
      }
    #endif
    device->queue = clCreateCommandQueue(device->context, device->id, 
                                         0, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create command queue.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->io_queue = clCreateCommandQueue(device->context, device->id, 
                                            0, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create I/O command queue.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  } else {
    cl_context_properties props[3] =
      { CL_CONTEXT_PLATFORM, (cl_context_properties)COMPUTE->platform,  0 };
    device->context = clCreateContext(props, 1, &device->id, nullptr, 
                                        nullptr, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create context.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->queue = clCreateCommandQueue(device->context, device->id, 
                                           0, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create command queue.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->io_queue = clCreateCommandQueue(device->context, device->id, 
                                              0, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create I/O command queue.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  }
  const char* s = (char*)program.c_str();
  /*
  uint32_t check = 7;
  while (*s) {
    check = check * 101 + *s++;
  }*/
  LOG("Creating program.");
  /*
  std::string decoded;
  Encoder::Decode(&program, &decoded);
  size_t length = decoded.size();
  const char* string = &decoded[0];*/
  size_t length = program.size();
  device->program = clCreateProgramWithSource(device->context, 1, 
                                              &s, &length, &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create program.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  LOG("Building program.");
  CL_ERR = clBuildProgram(device->program, 1, &device->id, 
                          "-cl-fast-relaxed-math", nullptr, nullptr);
  CHECK_CL(CL_ERR,
           {
             size_t log_size;
             CHECK_CL(clGetProgramBuildInfo(device->program, device->id, 
                                            CL_PROGRAM_BUILD_LOG, 0, nullptr, 
                                            &log_size),
                      {
                        ERR("Failed to get build log size.");
                        LOG("Device will be disabled.");
                        device->available = false;
                        return false;
                      });
             char build_log[65536];
             //char* build_log = (char*)calloc(log_size, sizeof(char));
             clGetProgramBuildInfo(device->program, device->id, 
                                   CL_PROGRAM_BUILD_LOG, 
                                   log_size, build_log, nullptr);
             ERR("Failed to build program.");
             ERR("%s", build_log);
             //free(build_log);
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  LOG("Build was successful, creating kernels.");
  device->primary_kernel = clCreateKernel(device->program, "Primary", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create primary kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->secondary_kernel = clCreateKernel(device->program, "Secondary", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create secondary kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->nee_kernel = clCreateKernel(device->program, "NEE", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create next-event estimation kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->env_sky_kernel = clCreateKernel(device->program, "EnvSky", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create sky environment kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->env_map_kernel = clCreateKernel(device->program, "EnvMap", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create environment map kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->emission_kernel = clCreateKernel(device->program, "Emission", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create emission kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->sample_kernel = clCreateKernel(device->program, "Sample", &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create sample kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  device->accumulate_kernel = clCreateKernel(device->program, "Accumulate", 
                                             &CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to create accumulate kernel.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  if (device == DISPLAY_DEVICE) {
    device->composite_two_kernel = clCreateKernel(device->program, 
                                                  "CompositeTwo", &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create composite two kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->composite_three_kernel = clCreateKernel(device->program, 
                                                    "CompositeThree", &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create composite three kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->composite_four_kernel = clCreateKernel(device->program, 
                                                   "CompositeFour", &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create composite four kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    device->composite_five_kernel = clCreateKernel(device->program, 
                                                   "CompositeFive", &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create composite five kernel.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  }
  LOG("Kernels created successfully.");
  return true; 
}
///
void ShutdownDevice(uint32_t index)
{
  LOG("Shutting down device: %u.", index);
  Device* device = &COMPUTE->devices[index];
  if (device->albedo_atlas) {
    clReleaseMemObject(device->albedo_atlas);
  }
  if (device->normal_atlas) {
    clReleaseMemObject(device->normal_atlas);
  }
  if (device->metallic_atlas) {
    clReleaseMemObject(device->metallic_atlas);
  }
  if (device->emission_atlas) {
    clReleaseMemObject(device->emission_atlas);
  }
  if (device->environment_map) {
    clReleaseMemObject(device->environment_map);
  }
  if (device->sky_table) {
    clReleaseMemObject(device->sky_table);
  }
  if (device->primary_kernel) {
    CHECK_CL(clReleaseKernel(device->primary_kernel), {});
  }
  if (device->secondary_kernel) {
    CHECK_CL(clReleaseKernel(device->secondary_kernel), {});
  }
  if (device->nee_kernel) {
    CHECK_CL(clReleaseKernel(device->nee_kernel), {});
  }
  if (device->env_sky_kernel) {
    CHECK_CL(clReleaseKernel(device->env_sky_kernel), {});
  }
  if (device->env_map_kernel) {
    CHECK_CL(clReleaseKernel(device->env_map_kernel), {});
  }
  if (device->emission_kernel) {
    CHECK_CL(clReleaseKernel(device->emission_kernel), {});
  }
  if (device->sample_kernel) {
    CHECK_CL(clReleaseKernel(device->sample_kernel), {});
  }
  if (device->accumulate_kernel) {
    CHECK_CL(clReleaseKernel(device->accumulate_kernel), {});
  }
  if (device->composite_two_kernel) {
    CHECK_CL(clReleaseKernel(device->composite_two_kernel), {});
  }
  if (device->composite_three_kernel) {
    CHECK_CL(clReleaseKernel(device->composite_three_kernel), {});
  }
  if (device->composite_four_kernel) {
    CHECK_CL(clReleaseKernel(device->composite_four_kernel), {});
  }
  if (device->composite_five_kernel) {
    CHECK_CL(clReleaseKernel(device->composite_five_kernel), {});
  }
  if (device->program) {
    CHECK_CL(clReleaseProgram(device->program), {});
  }
  if (device->queue) {
    CHECK_CL(clReleaseCommandQueue(device->queue), {});
  }
  if (device->io_queue) {
    CHECK_CL(clReleaseCommandQueue(device->io_queue), {});
  }
  if (device->context) {
    CHECK_CL(clReleaseContext(device->context), {});
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
  cl_image_format format = { CL_RGBA, CL_UNORM_INT8 };
  cl_image_desc desc = { CL_MEM_OBJECT_IMAGE2D, 
                         (uint32_t)ROVE.target_width, 
                         (uint32_t)ROVE.target_height };
  if (device == DISPLAY_DEVICE) {
    #if defined(ROVE_GL)
      LOG("Creating target on display device from GL Texture.");
      COMPUTE->target = clCreateFromGLTexture(DISPLAY_DEVICE->context, 
                                              CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 
                                              0, (GLuint)((uint64_t)ROVE.target), 
                                              &CL_ERR);
      CHECK_CL(CL_ERR, 
               {
                 ERR("Failed to create target from GL texture.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    #elif defined(ROVE_D3D11)
      LOG("Creating output target on display device from D3D11 Texture.");
      clCreateFromD3D11Texture2DKHR_fn CreateFromD3D11Texture;
	    if (COMPUTE->nvidia_platform) {
        CreateFromD3D11Texture = 
          (clCreateFromD3D11Texture2DKHR_fn)CL_GET_EXT(COMPUTE->platform,
                                                 "clCreateFromD3D11Texture2DNV");
      } else {
        CreateFromD3D11Texture = 
          (clCreateFromD3D11Texture2DKHR_fn)CL_GET_EXT(COMPUTE->platform,
                                                 "clCreateFromD3D11Texture2DKHR");
      }
      if (!CreateFromD3D11Texture) {
        ERR("Failed to get clCreateFromD3D11Texture2D function address.");
        LOG("Device will be disabled.");
        device->available = false;
        return false;
      }
      COMPUTE->target = CreateFromD3D11Texture(DISPLAY_DEVICE->context, 
                                               CL_MEM_WRITE_ONLY, 
                                               (ID3D11Texture2D*)ROVE.target,
                                               0, &CL_ERR);
      CHECK_CL(CL_ERR, 
               {
                 ERR("Failed to create target from D3D11 texture.");
                 LOG("Device will be disabled.");
                 device->available = false;
                 return false;
               });
    #endif
    clSetKernelArg(DISPLAY_DEVICE->composite_two_kernel, 2, sizeof(cl_mem), 
                   &COMPUTE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_two_kernel, 3, sizeof(int32_t), 
                   &ROVE.target_width);
    clSetKernelArg(DISPLAY_DEVICE->composite_two_kernel, 4, sizeof(int32_t), 
                   &ROVE.target_height);
    clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 3, sizeof(cl_mem), 
                   &COMPUTE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 4, sizeof(int32_t), 
                   &ROVE.target_width);
    clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 5, sizeof(int32_t), 
                   &ROVE.target_height);
    clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 4, sizeof(cl_mem), 
                   &COMPUTE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 5, sizeof(int32_t), 
                   &ROVE.target_width);
    clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 6, sizeof(int32_t), 
                   &ROVE.target_height);
    clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 5, sizeof(cl_mem), 
                   &COMPUTE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 6, sizeof(int32_t), 
                   &ROVE.target_width);
    clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 7, sizeof(int32_t), 
                   &ROVE.target_height);
  }
  LOG("Creating device target.");
  if (device == DISPLAY_DEVICE) {
    device->target = clCreateImage(device->context, CL_MEM_READ_WRITE,
                                   &format, &desc, nullptr, &CL_ERR);
    LOG("Setting display device target as root compositing target.");
    clSetKernelArg(DISPLAY_DEVICE->composite_two_kernel, 0, sizeof(cl_mem), 
                   &DISPLAY_DEVICE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 0, sizeof(cl_mem), 
                   &DISPLAY_DEVICE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 0, sizeof(cl_mem), 
                   &DISPLAY_DEVICE->target);
    clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 0, sizeof(cl_mem), 
                   &DISPLAY_DEVICE->target);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create target for device.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
  } else {
    device->target = clCreateImage(device->context, 
                                   CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR,
                                   &format, &desc, nullptr, &CL_ERR);
    size_t origin[3] = {0};
    size_t region[3] = { (uint32_t)ROVE.target_width, 
                         (uint32_t)ROVE.target_height, 1};
    size_t pitch;
    device->h_target = (uint8_t*)clEnqueueMapImage(device->queue, 
                                   device->target, CL_TRUE, 
                                   CL_MAP_READ, 
                                   origin, region, &pitch, 
                                   nullptr, 0, nullptr, nullptr, &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to map pinned device target.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    LOG("Creating device compositing target on display device.");
    device->compositing_target = clCreateImage(DISPLAY_DEVICE->context,
                                         CL_MEM_READ_ONLY,
                                         &format, &desc, nullptr,
                                         &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create compositing target for device.");
               LOG("Device will be disabled.");
               device->available = false;
               return false;
             });
    ++COMPUTE->compositing_target_count;
    LOG("Compositing target index: %u.", COMPUTE->compositing_target_count);
    if (COMPUTE->compositing_target_count == 1) {
      clSetKernelArg(DISPLAY_DEVICE->composite_two_kernel, 1, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 1, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 1, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 1, sizeof(cl_mem), 
                     &device->compositing_target);
    } else if (COMPUTE->compositing_target_count == 2) {
      clSetKernelArg(DISPLAY_DEVICE->composite_three_kernel, 2, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 2, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 2, sizeof(cl_mem), 
                     &device->compositing_target);
    } else if (COMPUTE->compositing_target_count == 3) {
      clSetKernelArg(DISPLAY_DEVICE->composite_four_kernel, 3, sizeof(cl_mem), 
                     &device->compositing_target);
      clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 3, sizeof(cl_mem), 
                     &device->compositing_target);
    } else if (COMPUTE->compositing_target_count == 4) {
      clSetKernelArg(DISPLAY_DEVICE->composite_five_kernel, 4, sizeof(cl_mem), 
                     &device->compositing_target);
    }
  }
	uint32_t ray_count = ROVE.target_width * ROVE.target_height;
  float ray_count_mb = (ray_count * sizeof(RayState)) / 1048576.0f; 
  LOG("Allocating device ray states: %.2f MB.", ray_count_mb);
	device->ray_states = clCreateBuffer(device->context, CL_MEM_READ_WRITE,
									  									sizeof(RayState) * ray_count, nullptr, 
									  									&CL_ERR);
  CHECK_CL(CL_ERR,
           {
             ERR("Failed to allocate device ray states.");
             LOG("Device will be disabled.");
             device->available = false;
             return false;
           });
  clSetKernelArg(device->primary_kernel, 0, sizeof(cl_mem), 
                 &device->ray_states);
  clSetKernelArg(device->primary_kernel, 13, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->primary_kernel, 14, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->primary_kernel, 15, sizeof(int32_t),
                 &COMPUTE->invert_image);
  clSetKernelArg(device->secondary_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->secondary_kernel, 12, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->secondary_kernel, 13, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->nee_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->nee_kernel, 12, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->nee_kernel, 13, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->env_sky_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->env_sky_kernel, 3, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->env_sky_kernel, 4, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->env_map_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->env_map_kernel, 3, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->env_map_kernel, 4, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->emission_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->emission_kernel, 7, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->emission_kernel, 8, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->sample_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
  clSetKernelArg(device->sample_kernel, 21, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->sample_kernel, 22, sizeof(int32_t),
                 &ROVE.target_height);
  clSetKernelArg(device->accumulate_kernel, 0, sizeof(cl_mem),
                 &device->ray_states);
	clSetKernelArg(device->accumulate_kernel, 1, sizeof(cl_mem),
								 &device->target);
  clSetKernelArg(device->accumulate_kernel, 3, sizeof(int32_t),
                 &ROVE.target_width);
  clSetKernelArg(device->accumulate_kernel, 4, sizeof(int32_t),
                 &ROVE.target_height);
  return true;
}
///
void ShutdownDeviceTargets(uint32_t index)
{
  LOG("Shutting down targets for device: %u.", index);
  Device* device = &COMPUTE->devices[index];
  if (device->h_target) {
    clEnqueueUnmapMemObject(device->queue, device->target, 
                            device->h_target, 0, nullptr, nullptr);
    CHECK_CL(clFinish(device->queue), {});
  }
  if (device->target) {
    CHECK_CL(clReleaseMemObject(device->target), {});
  }
  if (device == DISPLAY_DEVICE) {
    if (COMPUTE->target) {
      CHECK_CL(clReleaseMemObject(COMPUTE->target), {});
    }
  }
  if (device->compositing_target) {
    CHECK_CL(clReleaseMemObject(device->compositing_target), {});
    --COMPUTE->compositing_target_count;
  }
  if (device->ray_states) {
    CHECK_CL(clReleaseMemObject(device->ray_states), {});
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
	cl_image_format format = { CL_RGBA, CL_HALF_FLOAT };
	cl_image_desc s_desc = { CL_MEM_OBJECT_IMAGE2D, 256, 128, 1, 0, 0, 0, 0, 0 };
	for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
		Device* device = &COMPUTE->devices[d];
		if (!device->available) {
			continue;
		}
		device->sky_table = clCreateImage(device->context,
																		  CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
																			&format, &s_desc, table, &CL_ERR);
    CHECK_CL(CL_ERR,
             {
               ERR("Failed to create sky table texture object.");
               return false;
             });
    clSetKernelArg(device->env_sky_kernel, 1, sizeof(cl_mem), &device->sky_table);
    clSetKernelArg(device->env_sky_kernel, 2, sizeof(Sun), &SCENE->sun);
    clSetKernelArg(device->sample_kernel, 9, sizeof(cl_mem), &device->sky_table);
    clSetKernelArg(device->sample_kernel, 10, sizeof(Sun), &SCENE->sun);
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
