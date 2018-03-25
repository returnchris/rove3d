#if defined(ROVE_CUDA)
#include "../../rove.h"
#include "../../core/utility.h"
#include "../../scene/tree.h"
#include "../../scene/scene.h"
#include "cuda_compute.h"
///
void ResetRenderAtStart()
{
  COMPUTE->reset_at_start = true;
}
///
void ResetRenderAtEnd()
{
  COMPUTE->reset_at_end = true;
}
///
int32_t StartDeviceRender(uint32_t index, 
                          uint32_t emitter_count, 
                          uint32_t portal_count)
{
  if (COMPUTE->first_frame_skip) {
    return true;
  }
  Device* device = &COMPUTE->devices[index];
  if (!device->available) {
    return true;
  }
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current on device: %u.",
                   index);
               return false;
             });
  uint32_t block2D_x = 8;
  uint32_t block2D_y = 8;
  uint32_t grid2D_x = ((ROVE.target_width + 7) & ~7) / block2D_x; 
  uint32_t grid2D_y = ((ROVE.target_height + 7) & ~7) / block2D_y;
  uint32_t block1D_x = 64;
  uint32_t block1D_y = 1;
  uint32_t grid1D_x = ((ROVE.target_width * ROVE.target_height + 63) & ~63) / 
                      block1D_x; 
  uint32_t grid1D_y = 1; 
  if (TREE->triangles.m_image) {
    if (TREE->triangles.m_flip) {
      device->primary_params[7] = &TREE->triangles.m_flop_images[index];
      device->secondary_params[7] = &TREE->triangles.m_flop_images[index];
      device->emission_params[3] = &TREE->triangles.m_flop_images[index];
      device->sample_params[3] = &TREE->triangles.m_flop_images[index];
      device->nee_params[7] = &TREE->triangles.m_flop_images[index];
    } else {
      device->primary_params[7] = &TREE->triangles.m_flip_images[index];
      device->secondary_params[7] = &TREE->triangles.m_flip_images[index];
      device->emission_params[3] = &TREE->triangles.m_flip_images[index];
      device->sample_params[3] = &TREE->triangles.m_flip_images[index];
      device->nee_params[7] = &TREE->triangles.m_flip_images[index];
    }
    device->primary_params[8] = &TREE->triangles.m_dummy_buffers[index];
    device->secondary_params[8] = &TREE->triangles.m_dummy_buffers[index];
    device->emission_params[4] = &TREE->triangles.m_dummy_buffers[index];
    device->sample_params[4] = &TREE->triangles.m_dummy_buffers[index];
    device->nee_params[8] = &TREE->triangles.m_dummy_buffers[index];
  } else {
    if (TREE->triangles.m_flip) {
      device->primary_params[8] = &TREE->triangles.m_flop_buffers[index];
      device->secondary_params[8] = &TREE->triangles.m_flop_buffers[index];
      device->emission_params[4] = &TREE->triangles.m_flop_buffers[index];
      device->sample_params[4] = &TREE->triangles.m_flop_buffers[index];
      device->nee_params[8] = &TREE->triangles.m_flop_buffers[index];
    } else {
      device->primary_params[8] = &TREE->triangles.m_flip_buffers[index];
      device->secondary_params[8] = &TREE->triangles.m_flip_buffers[index];
      device->emission_params[4] = &TREE->triangles.m_flip_buffers[index];
      device->sample_params[4] = &TREE->triangles.m_flip_buffers[index];
      device->nee_params[8] = &TREE->triangles.m_flip_buffers[index];
    }
    device->primary_params[7] = &TREE->triangles.m_dummy_images[index];
    device->secondary_params[7] = &TREE->triangles.m_dummy_images[index];
    device->emission_params[3] = &TREE->triangles.m_dummy_images[index];
    device->sample_params[3] = &TREE->triangles.m_dummy_images[index];
    device->nee_params[7] = &TREE->triangles.m_dummy_images[index];
  }
  if (TREE->nodes.m_image) {
    if (TREE->nodes.m_flip) {
      device->primary_params[3] = &TREE->nodes.m_flop_images[index];
      device->secondary_params[3] = &TREE->nodes.m_flop_images[index];
      device->nee_params[3] = &TREE->nodes.m_flop_images[index];
    } else {
      device->primary_params[3] = &TREE->nodes.m_flip_images[index];
      device->secondary_params[3] = &TREE->nodes.m_flip_images[index];
      device->nee_params[3] = &TREE->nodes.m_flip_images[index];
    }
    device->primary_params[4] = &TREE->nodes.m_dummy_buffers[index];
    device->secondary_params[4] = &TREE->nodes.m_dummy_buffers[index];
    device->nee_params[4] = &TREE->nodes.m_dummy_buffers[index];
  } else {
    if (TREE->nodes.m_flip) {
      device->primary_params[4] = &TREE->nodes.m_flop_buffers[index];
      device->secondary_params[4] = &TREE->nodes.m_flop_buffers[index];
      device->nee_params[4] = &TREE->nodes.m_flop_buffers[index];
    } else {
      device->primary_params[4] = &TREE->nodes.m_flip_buffers[index];
      device->secondary_params[4] = &TREE->nodes.m_flip_buffers[index];
      device->nee_params[4] = &TREE->nodes.m_flip_buffers[index];
    }
    device->primary_params[3] = &TREE->nodes.m_dummy_images[index];
    device->secondary_params[3] = &TREE->nodes.m_dummy_images[index];
    device->nee_params[3] = &TREE->nodes.m_dummy_images[index];
  }
  if (TREE->materials.m_flip) {
    device->primary_params[1] = &TREE->materials.m_flop_buffers[index];
    device->secondary_params[1] = &TREE->materials.m_flop_buffers[index];
    device->emission_params[1] = &TREE->materials.m_flop_buffers[index];
    device->sample_params[1] = &TREE->materials.m_flop_buffers[index];
    device->nee_params[1] = &TREE->materials.m_flop_buffers[index];
  } else {
    device->primary_params[1] = &TREE->materials.m_flip_buffers[index];
    device->secondary_params[1] = &TREE->materials.m_flip_buffers[index];
    device->emission_params[1] = &TREE->materials.m_flip_buffers[index];
    device->sample_params[1] = &TREE->materials.m_flip_buffers[index];
    device->nee_params[1] = &TREE->materials.m_flip_buffers[index];
  }
  if (TREE->emitter_indices.m_flip) {
    device->sample_params[13] = &TREE->emitter_indices.m_flop_buffers[index];
  } else {
    device->sample_params[13] = &TREE->emitter_indices.m_flip_buffers[index];
  }
  device->emission_params[6] = &emitter_count;
  device->sample_params[14] = &emitter_count;
  if (TREE->portal_indices.m_flip) {
    device->sample_params[15] = &TREE->portal_indices.m_flop_buffers[index];
  } else {
    device->sample_params[15] = &TREE->portal_indices.m_flip_buffers[index];
  }
  device->sample_params[16] = &portal_count;
  ///
  device->primary_params[2] = &TREE->nodes.m_image;
  device->primary_params[5] = &TREE->top_tree_offset;
  device->primary_params[6] = &TREE->triangles.m_image;
  device->primary_params[11] = &SCENE->camera;
  device->secondary_params[2] = &TREE->nodes.m_image;
  device->secondary_params[5] = &TREE->top_tree_offset;
  device->secondary_params[6] = &TREE->triangles.m_image;
  device->env_map_params[2] = &SCENE->environment_properties;
  device->emission_params[2] = &TREE->triangles.m_image;
  device->sample_params[2] = &TREE->triangles.m_image;
  device->sample_params[12] = &SCENE->environment_properties;
  int32_t sample_sun = (SCENE->environment_type == ENVIRONMENT_SKY) &&
                       (SCENE->sun.sun_intensity > 0.0f);
  device->sample_params[23] = &sample_sun; 
  device->nee_params[2] = &TREE->nodes.m_image;
  device->nee_params[5] = &TREE->top_tree_offset;
  device->nee_params[6] = &TREE->triangles.m_image;
  uint32_t sample = (COMPUTE->frame - 1) * ROVE.samples_per_frame;
  for (uint32_t spf = 0; spf < ROVE.samples_per_frame; ++spf) {
    device->primary_params[16] = &sample;
    for (int32_t bounce = 0; bounce <= ROVE.max_bounces; ++bounce) {
      uint32_t b_seeds[36];
      for (uint32_t s = 0; s < 36; ++s) {
        b_seeds[s] = RANDOM_U();
      }
      device->primary_params[12] = &b_seeds[0];
      device->secondary_params[11] = &b_seeds[4];
      device->sample_params[17] = &b_seeds[8];
      device->sample_params[18] = &b_seeds[12];
      device->sample_params[19] = &b_seeds[16];
      device->sample_params[20] = &b_seeds[20];
      device->nee_params[11] = &b_seeds[24];
      if (bounce == 0) {
        CHECK_CUDA(cuLaunchKernel(device->primary_function, grid2D_x, grid2D_y, 
                                  1, block2D_x, block2D_y, 1, 0, device->stream, 
                                  device->primary_params, nullptr), 
                   {
                     ERR("Failed to launch primary kernel.");
                     return false;
                   });
      } else {
        CHECK_CUDA(cuLaunchKernel(device->secondary_function, grid2D_x, grid2D_y, 
                                  1, block2D_x, block2D_y, 1, 0, device->stream, 
                                  device->secondary_params, nullptr), 
                   {
                     ERR("Failed to launch secondary kernel.");
                     return false;
                   });
      }
      if (SCENE->environment_type == ENVIRONMENT_SKY) {
        device->env_sky_params[5] = &bounce;
        CHECK_CUDA(cuLaunchKernel(device->env_sky_function, grid1D_x, grid1D_y, 
                                  1, block1D_x, block1D_y, 1, 0, device->stream, 
                                  device->env_sky_params, nullptr), 
                   {
                     ERR("Failed to launch sky environment kernel.");
                     return false;
                   });
      } else if (SCENE->environment_type == ENVIRONMENT_MAP) {
        device->env_map_params[5] = &bounce;
        CHECK_CUDA(cuLaunchKernel(device->env_map_function, grid1D_x, grid1D_y, 
                                  1, block1D_x, block1D_y, 1, 0, device->stream, 
                                  device->env_map_params, nullptr), 
                   {
                     ERR("Failed to launch environment map kernel.");
                     return false;
                   });
      }
      device->emission_params[9] = &bounce;
      CHECK_CUDA(cuLaunchKernel(device->emission_function, grid1D_x, grid1D_y, 
                                1, block1D_x, block1D_y, 1, 0, device->stream, 
                                device->emission_params, nullptr), 
                 {
                   ERR("Failed to launch emission kernel.");
                   return false;
                 });
      if (bounce < ROVE.max_bounces) {
        CHECK_CUDA(cuLaunchKernel(device->sample_function, grid1D_x, grid1D_y, 
                                  1, block1D_x, block1D_y, 1, 0, device->stream, 
                                  device->sample_params, nullptr), 
                   {
                     ERR("Failed to launch sample kernel.");
                     return false;
                   });
        CHECK_CUDA(cuLaunchKernel(device->nee_function, grid1D_x, grid1D_y, 
                                  1, block1D_x, block1D_y, 1, 0, device->stream, 
                                  device->nee_params, nullptr), 
                   {
                     ERR("Failed to launch next-event estimation kernel.");
                     return false;
                   });
      }
    }
    ++sample;
  }
  if (COMPUTE->compositing_target_count > 0) {
    float inv_sample_count = 1.0f / 
                             (float)(COMPUTE->frame * ROVE.samples_per_frame);
    device->accumulate_params[2] = &SCENE->image_properties;
    device->accumulate_params[5] = &inv_sample_count;
    CHECK_CUDA(cuLaunchKernel(device->accumulate_function, grid2D_x, grid2D_y, 
                              1, block2D_x, block2D_y, 1, 0, device->stream, 
                              device->accumulate_params, nullptr), 
               {
                 ERR("Failed to launch accumulate kernel on device: %u.",
                     index);
                 return false;
               });
    if (device != DISPLAY_DEVICE) {
	    CUDA_MEMCPY2D device_to_host;
      memset(&device_to_host, 0, sizeof(CUDA_MEMCPY2D));
      device_to_host.srcMemoryType = CU_MEMORYTYPE_ARRAY;
      device_to_host.srcArray = device->target_array;
      device_to_host.dstMemoryType = CU_MEMORYTYPE_ARRAY;
      device_to_host.dstArray = device->compositing_target_array;
      device_to_host.Height = ROVE.target_height;
      device_to_host.WidthInBytes = ROVE.target_width * sizeof(uint8_t) * 4;
      CHECK_CUDA(cuMemcpy2DAsync(&device_to_host, device->stream), 
                 {
                   ERR("Failed to initiate compositing target transfer on "
                       "device: %u", index);
                   return false;
                 });
    }
  }
  return true;
}
///
void FinishDeviceRender(uint32_t index)
{
  if (COMPUTE->first_frame_skip) {
    return;
  }
  Device* device = &COMPUTE->devices[index];
  if (!device->available) {
    return;
  }
  CHECK_CUDA(cuCtxSetCurrent(device->context),
             {
               ERR("Failed to set device context as current.");
               return;
             });
  CHECK_CUDA(cuStreamSynchronize(device->stream), {});
  CHECK_CUDA(cuStreamSynchronize(device->io_stream), {});
}
///
void CompositeFinalImage()
{
  if (COMPUTE->first_frame_skip) {
    COMPUTE->first_frame_skip = false;
    return;
  }
  CHECK_CUDA(cuCtxSetCurrent(DISPLAY_DEVICE->context),
             {
               ERR("Failed to set display device context as current to "
                   "composite final image.");
               return;
             });
  uint32_t block2D_x = 8;
  uint32_t block2D_y = 8;
  uint32_t grid2D_x = ((ROVE.target_width + 7) & ~7) / block2D_x; 
  uint32_t grid2D_y = ((ROVE.target_height + 7) & ~7) / block2D_y;
  CHECK_CUDA(cuGraphicsMapResources(1, &COMPUTE->interop_resource, 
                                    DISPLAY_DEVICE->stream), {});
  if (COMPUTE->compositing_target_count == 0) {
    float inv_sample_count = 1.0f / 
                             (float)(COMPUTE->frame * ROVE.samples_per_frame);
    DISPLAY_DEVICE->accumulate_params[1] = &COMPUTE->target;
    DISPLAY_DEVICE->accumulate_params[2] = &SCENE->image_properties;
    DISPLAY_DEVICE->accumulate_params[5] = &inv_sample_count;
    CHECK_CUDA(cuLaunchKernel(DISPLAY_DEVICE->accumulate_function, 
                              grid2D_x, grid2D_y, 1, block2D_x, block2D_y, 
                              1, 0, DISPLAY_DEVICE->stream, 
                              DISPLAY_DEVICE->accumulate_params, nullptr), 
                              {});
  } else if (COMPUTE->compositing_target_count == 1) {
    CHECK_CUDA(cuLaunchKernel(DISPLAY_DEVICE->composite_two_function, grid2D_x, 
                              grid2D_y, 1, block2D_x, block2D_y, 1, 0, 
                              DISPLAY_DEVICE->stream,
                              DISPLAY_DEVICE->composite_two_params,
                              nullptr), {});
  } else if (COMPUTE->compositing_target_count == 2) {
    CHECK_CUDA(cuLaunchKernel(DISPLAY_DEVICE->composite_three_function, grid2D_x, 
                              grid2D_y, 1, block2D_x, block2D_y, 1, 0, 
                              DISPLAY_DEVICE->stream,
                              DISPLAY_DEVICE->composite_three_params,
                              nullptr), {});
  } else if (COMPUTE->compositing_target_count == 3) {
    CHECK_CUDA(cuLaunchKernel(DISPLAY_DEVICE->composite_four_function, grid2D_x, 
                              grid2D_y, 1, block2D_x, block2D_y, 1, 0, 
                              DISPLAY_DEVICE->stream,
                              DISPLAY_DEVICE->composite_four_params,
                              nullptr), {});
  } else if (COMPUTE->compositing_target_count == 4) {
    CHECK_CUDA(cuLaunchKernel(DISPLAY_DEVICE->composite_five_function, grid2D_x, 
                              grid2D_y, 1, block2D_x, block2D_y, 1, 0, 
                              DISPLAY_DEVICE->stream,
                              DISPLAY_DEVICE->composite_five_params,
                              nullptr), {});
  }
  CHECK_CUDA(cuGraphicsUnmapResources(1, &COMPUTE->interop_resource, 
                                      DISPLAY_DEVICE->stream), {});
  CHECK_CUDA(cuStreamSynchronize(DISPLAY_DEVICE->stream), {});
  ++COMPUTE->frame;
}
///
int32_t SetAlbedoAtlas(uint8_t* atlas,
                       uint32_t width,
                       uint32_t height)
{
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
    if (device->albedo_atlas_array) {
      CHECK_CUDA(cuArrayDestroy(device->albedo_atlas_array), {});
    }
    if (device->albedo_atlas) {
      CHECK_CUDA(cuTexObjectDestroy(device->albedo_atlas), {});
    }
    LOG("Allocating albedo atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Albedo atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    CUDA_ARRAY_DESCRIPTOR array_desc;
		memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
		array_desc.Width = width;
		array_desc.Height = height;
		array_desc.Format = CU_AD_FORMAT_UNSIGNED_INT8;
		array_desc.NumChannels = 4;
		CHECK_CUDA(cuArrayCreate(&device->albedo_atlas_array, &array_desc),
							 {
								 ERR("Failed to allocate albedo atlas array.");
								 return false;
							 });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = atlas;
    host_to_device.srcPitch = sizeof(uint8_t) * 4 * width;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->albedo_atlas_array;
    host_to_device.Height = height;
    host_to_device.WidthInBytes = sizeof(uint8_t) * 4 * width;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy albedo atlas to device.");
                 return false;
               });
		CUDA_RESOURCE_DESC resource_desc;
		memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
		resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
		resource_desc.res.array.hArray = device->albedo_atlas_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    // texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = width;
    view_desc.height = height;
    view_desc.format = CU_RES_VIEW_FORMAT_UINT_4X8;
		CHECK_CUDA(cuTexObjectCreate(&device->albedo_atlas, &resource_desc,
        												 &texture_desc, &view_desc),
							 {
								 ERR("Failed to create albedo atlas texture object.");
								 return false;
							 });
    device->primary_params[9] = &device->albedo_atlas;
    device->secondary_params[9] = &device->albedo_atlas;
    device->sample_params[5] = &device->albedo_atlas;
    device->nee_params[9] = &device->albedo_atlas;
  }
  return true;
}
///
int32_t SetNormalAtlas(uint8_t* atlas,
                       uint32_t width,
                       uint32_t height)
{
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
    if (device->normal_atlas_array) {
      CHECK_CUDA(cuArrayDestroy(device->normal_atlas_array), {});
    }
    if (device->normal_atlas) {
      CHECK_CUDA(cuTexObjectDestroy(device->normal_atlas), {});
    }
    LOG("Allocating normal atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Normal atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    CUDA_ARRAY_DESCRIPTOR array_desc;
		memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
		array_desc.Width = width;
		array_desc.Height = height;
		array_desc.Format = CU_AD_FORMAT_UNSIGNED_INT8;
		array_desc.NumChannels = 4;
		CHECK_CUDA(cuArrayCreate(&device->normal_atlas_array, &array_desc),
							 {
								 ERR("Failed to allocate normal atlas array.");
								 return false;
							 });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = atlas;
    host_to_device.srcPitch = sizeof(uint8_t) * 4 * width;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->normal_atlas_array;
    host_to_device.Height = height;
    host_to_device.WidthInBytes = sizeof(uint8_t) * 4 * width;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy normal atlas to device.");
                 return false;
               });
		CUDA_RESOURCE_DESC resource_desc;
		memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
		resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
		resource_desc.res.array.hArray = device->normal_atlas_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    // texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = width;
    view_desc.height = height;
    view_desc.format = CU_RES_VIEW_FORMAT_UINT_4X8;
		CHECK_CUDA(cuTexObjectCreate(&device->normal_atlas, &resource_desc,
        												 &texture_desc, &view_desc),
							 {
								 ERR("Failed to create normal atlas texture object.");
								 return false;
							 });
    device->sample_params[6] = &device->normal_atlas;
  }
  return true;
}
///
int32_t SetMetallicAtlas(uint8_t* atlas,
                         uint32_t width,
                         uint32_t height)
{
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
    if (device->metallic_atlas_array) {
      CHECK_CUDA(cuArrayDestroy(device->metallic_atlas_array), {});
    }
    if (device->metallic_atlas) {
      CHECK_CUDA(cuTexObjectDestroy(device->metallic_atlas), {});
    }
    LOG("Allocating metallic atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Metallic atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    CUDA_ARRAY_DESCRIPTOR array_desc;
		memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
		array_desc.Width = width;
		array_desc.Height = height;
		array_desc.Format = CU_AD_FORMAT_UNSIGNED_INT8;
		array_desc.NumChannels = 4;
		CHECK_CUDA(cuArrayCreate(&device->metallic_atlas_array, &array_desc),
							 {
								 ERR("Failed to allocate metallic atlas array.");
								 return false;
							 });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = atlas;
    host_to_device.srcPitch = sizeof(uint8_t) * 4 * width;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->metallic_atlas_array;
    host_to_device.Height = height;
    host_to_device.WidthInBytes = sizeof(uint8_t) * 4 * width;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy metallic atlas to device.");
                 return false;
               });
		CUDA_RESOURCE_DESC resource_desc;
		memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
		resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
		resource_desc.res.array.hArray = device->metallic_atlas_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    // texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = width;
    view_desc.height = height;
    view_desc.format = CU_RES_VIEW_FORMAT_UINT_4X8;
		CHECK_CUDA(cuTexObjectCreate(&device->metallic_atlas, &resource_desc,
        												 &texture_desc, &view_desc),
							 {
								 ERR("Failed to create metallic atlas texture object.");
								 return false;
							 });
    device->sample_params[7] = &device->metallic_atlas;
  }
  return true;
}
///
int32_t SetEmissionAtlas(uint8_t* atlas,
                         uint32_t width,
                         uint32_t height)
{
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
    if (device->emission_atlas_array) {
      CHECK_CUDA(cuArrayDestroy(device->emission_atlas_array), {});
    }
    if (device->emission_atlas) {
      CHECK_CUDA(cuTexObjectDestroy(device->emission_atlas), {});
    }
    LOG("Allocating emission atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Emission atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    CUDA_ARRAY_DESCRIPTOR array_desc;
		memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
		array_desc.Width = width;
		array_desc.Height = height;
		array_desc.Format = CU_AD_FORMAT_UNSIGNED_INT8;
		array_desc.NumChannels = 4;
		CHECK_CUDA(cuArrayCreate(&device->emission_atlas_array, &array_desc),
							 {
								 ERR("Failed to allocate emission atlas array.");
								 return false;
							 });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = atlas;
    host_to_device.srcPitch = sizeof(uint8_t) * 4 * width;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->emission_atlas_array;
    host_to_device.Height = height;
    host_to_device.WidthInBytes = sizeof(uint8_t) * 4 * width;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy emission atlas to device.");
                 return false;
               });
		CUDA_RESOURCE_DESC resource_desc;
		memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
		resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
		resource_desc.res.array.hArray = device->emission_atlas_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    // texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = width;
    view_desc.height = height;
    view_desc.format = CU_RES_VIEW_FORMAT_UINT_4X8;
		CHECK_CUDA(cuTexObjectCreate(&device->emission_atlas, &resource_desc,
        												 &texture_desc, &view_desc),
							 {
								 ERR("Failed to create emission atlas texture object.");
								 return false;
							 });
    device->primary_params[10] = &device->emission_atlas;
    device->secondary_params[10] = &device->emission_atlas;
    device->emission_params[5] = &device->emission_atlas;
    device->sample_params[8] = &device->emission_atlas;
    device->nee_params[10] = &device->emission_atlas;
  }
  return true;
}
///
int32_t SetEnvironmentMap(uint8_t* map,
                          uint32_t width,
                          uint32_t height)
{
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
    if (device->environment_map_array) {
      CHECK_CUDA(cuArrayDestroy(device->environment_map_array), {});
    }
    if (device->environment_map) {
      CHECK_CUDA(cuTexObjectDestroy(device->environment_map), {});
    }
    LOG("Allocating environment map on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Environment map dimensions exceed max dimensions allowed by device.");
      return false;
    }
    CUDA_ARRAY_DESCRIPTOR array_desc;
		memset(&array_desc, 0, sizeof(CUDA_ARRAY_DESCRIPTOR));
		array_desc.Width = width;
		array_desc.Height = height;
		array_desc.Format = CU_AD_FORMAT_HALF;
		array_desc.NumChannels = 4;
		CHECK_CUDA(cuArrayCreate(&device->environment_map_array, &array_desc),
							 {
								 ERR("Failed to allocate environment map array.");
								 return false;
							 });
    CUDA_MEMCPY2D host_to_device;
    memset(&host_to_device, 0, sizeof(CUDA_MEMCPY2D));
    host_to_device.srcMemoryType = CU_MEMORYTYPE_HOST;
    host_to_device.srcHost = map;
    host_to_device.srcPitch = sizeof(uint16_t) * 4 * width;
    host_to_device.dstMemoryType = CU_MEMORYTYPE_ARRAY;
    host_to_device.dstArray = device->environment_map_array;
    host_to_device.Height = height;
    host_to_device.WidthInBytes = sizeof(uint16_t) * 4 * width;
    CHECK_CUDA(cuMemcpy2D(&host_to_device), 
               {
                 ERR("Failed to copy environment map to device.");
                 return false;
               });
		CUDA_RESOURCE_DESC resource_desc;
		memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
		resource_desc.resType = CU_RESOURCE_TYPE_ARRAY;
		resource_desc.res.array.hArray = device->environment_map_array;
    CUDA_TEXTURE_DESC texture_desc;
    memset(&texture_desc, 0, sizeof(CUDA_TEXTURE_DESC));
    texture_desc.addressMode[0] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.addressMode[1] = CU_TR_ADDRESS_MODE_WRAP;
    texture_desc.filterMode = CU_TR_FILTER_MODE_LINEAR;
    texture_desc.flags = CU_TRSF_NORMALIZED_COORDINATES;
    CUDA_RESOURCE_VIEW_DESC view_desc;
    memset(&view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
    view_desc.width = width;
    view_desc.height = height;
    view_desc.format = CU_RES_VIEW_FORMAT_FLOAT_4X16;
		CHECK_CUDA(cuTexObjectCreate(&device->environment_map, &resource_desc,
        												 &texture_desc, &view_desc),
							 {
								 ERR("Failed to create environment map texture object.");
								 return false;
							 });
    device->env_map_params[1] = &device->environment_map;
    device->sample_params[11] = &device->environment_map;
  }
  return true;
}
#endif
