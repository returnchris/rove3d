#if defined(ROVE_CL)
#include "../../rove.h"
#include "../../core/utility.h"
#include "../../scene/tree.h"
#include "../../scene/scene.h"
#include "cl_compute.h"
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
  size_t global2d[2] = { ((uint32_t)ROVE.target_width + 7) & ~7, 
                         ((uint32_t)ROVE.target_height + 7) & ~7 };
  size_t local2d[2] = { 8, 8 };
  size_t global1d = (((uint32_t)(ROVE.target_width * ROVE.target_height)) + 
                     63) & ~63;
  size_t local1d = 64; 
  if (TREE->triangles.m_image) {
    if (TREE->triangles.m_flip) {
      clSetKernelArg(device->primary_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flop_images[index]);
      clSetKernelArg(device->secondary_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flop_images[index]);
      clSetKernelArg(device->emission_kernel, 3, sizeof(cl_mem),
                     &TREE->triangles.m_flop_images[index]);
      clSetKernelArg(device->sample_kernel, 3, sizeof(cl_mem),
                     &TREE->triangles.m_flop_images[index]);
      clSetKernelArg(device->nee_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flop_images[index]);
    } else {
      clSetKernelArg(device->primary_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flip_images[index]);
      clSetKernelArg(device->secondary_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flip_images[index]);
      clSetKernelArg(device->emission_kernel, 3, sizeof(cl_mem),
                     &TREE->triangles.m_flip_images[index]);
      clSetKernelArg(device->sample_kernel, 3, sizeof(cl_mem),
                     &TREE->triangles.m_flip_images[index]);
      clSetKernelArg(device->nee_kernel, 7, sizeof(cl_mem),
                     &TREE->triangles.m_flip_images[index]);
    }
    clSetKernelArg(device->primary_kernel, 8, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_buffers[index]);
    clSetKernelArg(device->secondary_kernel, 8, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_buffers[index]);
    clSetKernelArg(device->emission_kernel, 4, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_buffers[index]);
    clSetKernelArg(device->sample_kernel, 4, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_buffers[index]);
    clSetKernelArg(device->nee_kernel, 8, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_buffers[index]);
  } else {
    if (TREE->triangles.m_flip) {
      clSetKernelArg(device->primary_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flop_buffers[index]);
      clSetKernelArg(device->secondary_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flop_buffers[index]);
      clSetKernelArg(device->emission_kernel, 4, sizeof(cl_mem),
                     &TREE->triangles.m_flop_buffers[index]);
      clSetKernelArg(device->sample_kernel, 4, sizeof(cl_mem),
                     &TREE->triangles.m_flop_buffers[index]);
      clSetKernelArg(device->nee_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flop_buffers[index]);
    } else {
      clSetKernelArg(device->primary_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flip_buffers[index]);
      clSetKernelArg(device->secondary_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flip_buffers[index]);
      clSetKernelArg(device->emission_kernel, 4, sizeof(cl_mem),
                     &TREE->triangles.m_flip_buffers[index]);
      clSetKernelArg(device->sample_kernel, 4, sizeof(cl_mem),
                     &TREE->triangles.m_flip_buffers[index]);
      clSetKernelArg(device->nee_kernel, 8, sizeof(cl_mem),
                     &TREE->triangles.m_flip_buffers[index]);
    }
    clSetKernelArg(device->primary_kernel, 7, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_images[index]);
    clSetKernelArg(device->secondary_kernel, 7, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_images[index]);
    clSetKernelArg(device->emission_kernel, 3, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_images[index]);
    clSetKernelArg(device->sample_kernel, 3, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_images[index]);
    clSetKernelArg(device->nee_kernel, 7, sizeof(cl_mem),
                   &TREE->triangles.m_dummy_images[index]);
  }
  if (TREE->nodes.m_image) {
    if (TREE->nodes.m_flip) {
      clSetKernelArg(device->primary_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flop_images[index]);
      clSetKernelArg(device->secondary_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flop_images[index]);
      clSetKernelArg(device->nee_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flop_images[index]);
    } else {
      clSetKernelArg(device->primary_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flip_images[index]);
      clSetKernelArg(device->secondary_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flip_images[index]);
      clSetKernelArg(device->nee_kernel, 3, sizeof(cl_mem),
                     &TREE->nodes.m_flip_images[index]);
    }
    clSetKernelArg(device->primary_kernel, 4, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_buffers[index]);
    clSetKernelArg(device->secondary_kernel, 4, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_buffers[index]);
    clSetKernelArg(device->nee_kernel, 4, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_buffers[index]);
  } else {
    if (TREE->nodes.m_flip) {
      clSetKernelArg(device->primary_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flop_buffers[index]);
      clSetKernelArg(device->secondary_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flop_buffers[index]);
      clSetKernelArg(device->nee_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flop_buffers[index]);
    } else {
      clSetKernelArg(device->primary_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flip_buffers[index]);
      clSetKernelArg(device->secondary_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flip_buffers[index]);
      clSetKernelArg(device->nee_kernel, 4, sizeof(cl_mem),
                     &TREE->nodes.m_flip_buffers[index]);
    }
    clSetKernelArg(device->primary_kernel, 3, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_images[index]);
    clSetKernelArg(device->secondary_kernel, 3, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_images[index]);
    clSetKernelArg(device->nee_kernel, 3, sizeof(cl_mem),
                   &TREE->nodes.m_dummy_images[index]);
  }
  if (TREE->materials.m_flip) {
    clSetKernelArg(device->primary_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flop_buffers[index]);
    clSetKernelArg(device->secondary_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flop_buffers[index]);
    clSetKernelArg(device->emission_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flop_buffers[index]);
    clSetKernelArg(device->sample_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flop_buffers[index]);
    clSetKernelArg(device->nee_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flop_buffers[index]);
  } else {
    clSetKernelArg(device->primary_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flip_buffers[index]);
    clSetKernelArg(device->secondary_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flip_buffers[index]);
    clSetKernelArg(device->emission_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flip_buffers[index]);
    clSetKernelArg(device->sample_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flip_buffers[index]);
    clSetKernelArg(device->nee_kernel, 1, sizeof(cl_mem),
                   &TREE->materials.m_flip_buffers[index]);
  }
  if (TREE->emitter_indices.m_flip) {
    clSetKernelArg(device->sample_kernel, 13, sizeof(cl_mem),
                   &TREE->emitter_indices.m_flop_buffers[index]);
  } else {
    clSetKernelArg(device->sample_kernel, 13, sizeof(cl_mem),
                   &TREE->emitter_indices.m_flip_buffers[index]);
  }
  clSetKernelArg(device->emission_kernel, 6, sizeof(uint32_t), &emitter_count);
  clSetKernelArg(device->sample_kernel, 14, sizeof(uint32_t), &emitter_count);
  if (TREE->portal_indices.m_flip) {
    clSetKernelArg(device->sample_kernel, 15, sizeof(cl_mem), 
                   &TREE->portal_indices.m_flop_buffers[index]);
  } else {
    clSetKernelArg(device->sample_kernel, 15, sizeof(cl_mem), 
                   &TREE->portal_indices.m_flip_buffers[index]);
  }
  clSetKernelArg(device->sample_kernel, 16, sizeof(uint32_t), &portal_count);
  ///
  clSetKernelArg(device->primary_kernel, 2, sizeof(int32_t), &TREE->nodes.m_image);
  clSetKernelArg(device->primary_kernel, 5, sizeof(int32_t), &TREE->top_tree_offset);
  clSetKernelArg(device->primary_kernel, 6, sizeof(int32_t), 
                 &TREE->triangles.m_image);
  clSetKernelArg(device->primary_kernel, 11, sizeof(Camera), &SCENE->camera);
  clSetKernelArg(device->secondary_kernel, 2, sizeof(int32_t), &TREE->nodes.m_image);
  clSetKernelArg(device->secondary_kernel, 5, sizeof(uint32_t), 
                 &TREE->top_tree_offset);
  clSetKernelArg(device->secondary_kernel, 6, sizeof(int32_t), 
                 &TREE->triangles.m_image);
  clSetKernelArg(device->env_map_kernel, 2, sizeof(EnvironmentProperties), 
                 &SCENE->environment_properties);
  clSetKernelArg(device->emission_kernel, 2, sizeof(int32_t), 
                 &TREE->triangles.m_image);
  clSetKernelArg(device->sample_kernel, 2, sizeof(int32_t), 
                 &TREE->triangles.m_image);
  clSetKernelArg(device->sample_kernel, 12, sizeof(EnvironmentProperties), 
                 &SCENE->environment_properties);
  int32_t sample_sun = (SCENE->environment_type == ENVIRONMENT_SKY) &&
                       (SCENE->sun.sun_intensity > 0.0f);
  clSetKernelArg(device->sample_kernel, 23, sizeof(int32_t), &sample_sun);
  clSetKernelArg(device->nee_kernel, 2, sizeof(int32_t), &TREE->nodes.m_image);
  clSetKernelArg(device->nee_kernel, 5, sizeof(int32_t), &TREE->top_tree_offset);
  clSetKernelArg(device->nee_kernel, 6, sizeof(int32_t), &TREE->triangles.m_image);
  uint32_t sample = (COMPUTE->frame - 1) * ROVE.samples_per_frame;
  for (uint32_t spf = 0; spf < ROVE.samples_per_frame; ++spf) {
    clSetKernelArg(device->primary_kernel, 16, sizeof(uint32_t), &sample);
    for (int32_t bounce = 0; bounce <= ROVE.max_bounces; ++bounce) {
      uint32_t b_seeds[36];
      for (uint32_t s = 0; s < 36; ++s) {
        b_seeds[s] = RANDOM_U();
      }
      clSetKernelArg(device->primary_kernel, 12, sizeof(cl_uint4), &b_seeds[0]);
      clSetKernelArg(device->secondary_kernel, 11, sizeof(cl_uint4), &b_seeds[4]);
      clSetKernelArg(device->sample_kernel, 17, sizeof(cl_uint4), &b_seeds[8]);
      clSetKernelArg(device->sample_kernel, 18, sizeof(cl_uint4), &b_seeds[12]);
      clSetKernelArg(device->sample_kernel, 19, sizeof(cl_uint4), &b_seeds[16]);
      clSetKernelArg(device->sample_kernel, 20, sizeof(cl_uint4), &b_seeds[20]);
      clSetKernelArg(device->nee_kernel, 11, sizeof(cl_uint4), &b_seeds[24]);
      if (bounce == 0) {
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->primary_kernel, 
                                        2, nullptr, global2d, local2d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch primary kernel.");
                   return false;
                 });
      } else {
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->secondary_kernel, 
                                        2, nullptr, global2d, local2d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch secondary kernel.");
                   return false;
                 });
      }
      if (SCENE->environment_type == ENVIRONMENT_SKY) {
        clSetKernelArg(device->env_sky_kernel, 5, sizeof(int32_t), &bounce);
        clSetKernelArg(device->env_sky_kernel, 2, sizeof(Sun), &SCENE->sun);
        clSetKernelArg(device->sample_kernel, 10, sizeof(Sun), &SCENE->sun);
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->env_sky_kernel, 
                                        1, nullptr, &global1d, &local1d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch sky environment kernel.");
                   return false;
                 });
      } else if (SCENE->environment_type == ENVIRONMENT_MAP) {
        clSetKernelArg(device->env_map_kernel, 5, sizeof(int32_t), &bounce);
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->env_map_kernel, 
                                        1, nullptr, &global1d, &local1d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch environment map kernel.");
                   return false;
                 });
      }
      clSetKernelArg(device->emission_kernel, 9, sizeof(int32_t), &bounce);
      CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->emission_kernel, 
                                      1, nullptr, &global1d, &local1d, 0, nullptr,
                                      nullptr), 
               {
                 ERR("Failed to launch emission kernel.");
                 return false;
               });
      if (bounce < ROVE.max_bounces) {
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->sample_kernel, 
                                        1, nullptr, &global1d, &local1d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch sample kernel.");
                   return false;
                 });
        CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->nee_kernel, 
                                        1, nullptr, &global1d, &local1d, 0, nullptr,
                                        nullptr), 
                 {
                   ERR("Failed to launch next-event estimation kernel.");
                   return false; 
                 });
      }
    }
    CHECK_CL(clFlush(device->queue), {});
    ++sample;
  }
  if (COMPUTE->compositing_target_count > 0) {
    float inv_sample_count = 1.0f /
                             (float)(COMPUTE->frame * ROVE.samples_per_frame);
    clSetKernelArg(device->accumulate_kernel, 2, sizeof(ImageProperties), 
                   &SCENE->image_properties);
    clSetKernelArg(device->accumulate_kernel, 5, sizeof(float), &inv_sample_count);
    CHECK_CL(clEnqueueNDRangeKernel(device->queue, device->accumulate_kernel, 
                                    2, nullptr, global2d, local2d, 0, nullptr,
                                    nullptr), 
             {
               ERR("Failed to launch accumulate kernel.");
               return false;
             });
    if (device != DISPLAY_DEVICE) {
      size_t origin[3] = {0};
      size_t region[3] = { (uint32_t)ROVE.target_width, 
                           (uint32_t)ROVE.target_height, 1};
      CHECK_CL(clEnqueueReadImage(device->queue, device->target, CL_FALSE, 
                                  origin, region, 0, 0, device->h_target,
                                  0, nullptr, nullptr), {});
    }
  }
  CHECK_CL(clFlush(device->queue), {});
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
  CHECK_CL(clFlush(device->queue), {});
  CHECK_CL(clFinish(device->queue), {});
  if (device != DISPLAY_DEVICE) {
    size_t origin[3] = {0};
    size_t region[3] = { (uint32_t)ROVE.target_width, 
                         (uint32_t)ROVE.target_height, 1};
    CHECK_CL(clEnqueueWriteImage(DISPLAY_DEVICE->queue, 
                                 device->compositing_target, 
                                 CL_FALSE, origin, region, 0, 0, 
                                 device->h_target, 0, nullptr, nullptr), {});
  }
  CHECK_CL(clFlush(device->io_queue), {});
  CHECK_CL(clFinish(device->io_queue), {});
}
///
void CompositeFinalImage()
{
  if (COMPUTE->first_frame_skip) {
    COMPUTE->first_frame_skip = false;
    return;
  }
  size_t global2d[2] = { ((uint32_t)ROVE.target_width + 7) & ~7, 
                         ((uint32_t)ROVE.target_height + 7) & ~7 };
  size_t local2d[2] = { 8, 8 };
  #if defined(ROVE_GL)
    CHECK_CL(clEnqueueAcquireGLObjects(DISPLAY_DEVICE->queue, 1, &COMPUTE->target, 
                                       0, nullptr, nullptr), {});
  #elif defined(ROVE_D3D11)
    if (COMPUTE->nvidia_platform) {
      CHECK_CL(COMPUTE->clEnqueueAcquireD3D11ObjectsNV(DISPLAY_DEVICE->queue, 1, 
                                                       &COMPUTE->target, 0,
                                                       nullptr, nullptr), {});
    } else {
      CHECK_CL(COMPUTE->clEnqueueAcquireD3D11ObjectsKHR(DISPLAY_DEVICE->queue, 1, 
                                                        &COMPUTE->target, 0, 
                                                        nullptr, nullptr), {});
    }
  #endif
  if (COMPUTE->compositing_target_count == 0) {
    float inv_sample_count = 1.0f /
                             (float)(COMPUTE->frame * ROVE.samples_per_frame);
    clSetKernelArg(DISPLAY_DEVICE->accumulate_kernel, 1, sizeof(cl_mem), 
                            &COMPUTE->target);
    clSetKernelArg(DISPLAY_DEVICE->accumulate_kernel, 2, sizeof(ImageProperties), 
                   &SCENE->image_properties);
    clSetKernelArg(DISPLAY_DEVICE->accumulate_kernel, 5, sizeof(float), 
                   &inv_sample_count);
    CHECK_CL(clEnqueueNDRangeKernel(DISPLAY_DEVICE->queue, 
                                    DISPLAY_DEVICE->accumulate_kernel, 
                                    2, nullptr, global2d, local2d, 0, nullptr,
                                    nullptr), {});
  } else if (COMPUTE->compositing_target_count == 1) {
    CHECK_CL(clEnqueueNDRangeKernel(DISPLAY_DEVICE->queue, 
						         						    DISPLAY_DEVICE->composite_two_kernel, 
												            2, nullptr, global2d, local2d, 0, 
												            nullptr, nullptr), {});
  } else if (COMPUTE->compositing_target_count == 2) {
    CHECK_CL(clEnqueueNDRangeKernel(DISPLAY_DEVICE->queue, 
						         						    DISPLAY_DEVICE->composite_three_kernel, 
												            2, nullptr, global2d, local2d, 0, 
												            nullptr, nullptr), {});
  } else if (COMPUTE->compositing_target_count == 3) {
    CHECK_CL(clEnqueueNDRangeKernel(DISPLAY_DEVICE->queue, 
						         						    DISPLAY_DEVICE->composite_four_kernel, 
												            2, nullptr, global2d, local2d, 0, 
												            nullptr, nullptr), {});
  } else if (COMPUTE->compositing_target_count == 4) {
    CHECK_CL(clEnqueueNDRangeKernel(DISPLAY_DEVICE->queue, 
						         						    DISPLAY_DEVICE->composite_five_kernel, 
												            2, nullptr, global2d, local2d, 0, 
												            nullptr, nullptr), {});
  }
  #if defined(ROVE_GL)
    CHECK_CL(clEnqueueReleaseGLObjects(DISPLAY_DEVICE->queue, 1, &COMPUTE->target, 
                                       0, nullptr, nullptr), {});
  #elif defined(ROVE_D3D11)
    if (COMPUTE->nvidia_platform) {
      CHECK_CL(COMPUTE->clEnqueueReleaseD3D11ObjectsNV(DISPLAY_DEVICE->queue, 1, 
                                                       &COMPUTE->target, 0, 
                                                       nullptr, nullptr), {});
    } else {
      CHECK_CL(COMPUTE->clEnqueueReleaseD3D11ObjectsKHR(DISPLAY_DEVICE->queue, 1, 
                                                        &COMPUTE->target, 0, 
                                                        nullptr, nullptr), {});
    }
  #endif
  CHECK_CL(clFlush(DISPLAY_DEVICE->queue), {});
  CHECK_CL(clFinish(DISPLAY_DEVICE->queue), {});
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
    if (device->albedo_atlas) {
      clReleaseMemObject(device->albedo_atlas);
    }
    cl_image_format image_format = { CL_ARGB, CL_UNORM_INT8 };
    cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE2D,
                                 width, height,
                                 1, 1, 0, 0, 0, 0, nullptr };
    LOG("Allocating albedo atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Albedo atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    device->albedo_atlas = clCreateImage(device->context,
                                         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                         &image_format, &image_desc, atlas,
                                         &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create albedo atlas image.")
               return false;
             });
    clSetKernelArg(device->primary_kernel, 9, sizeof(cl_mem), &device->albedo_atlas);
    clSetKernelArg(device->secondary_kernel, 9, sizeof(cl_mem), &device->albedo_atlas);
    clSetKernelArg(device->sample_kernel, 5, sizeof(cl_mem), &device->albedo_atlas);
    clSetKernelArg(device->nee_kernel, 9, sizeof(cl_mem), &device->albedo_atlas);
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
    if (device->normal_atlas) {
      clReleaseMemObject(device->normal_atlas);
    }
    cl_image_format image_format = { CL_ARGB, CL_UNORM_INT8 };
    cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE2D,
                                 width, height,
                                 1, 1, 0, 0, 0, 0, nullptr };
    LOG("Allocating normal atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Normal atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    device->normal_atlas = clCreateImage(device->context,
                                         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                         &image_format, &image_desc, atlas,
                                         &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create normal atlas image.")
               return false;
             });
    clSetKernelArg(device->sample_kernel, 6, sizeof(cl_mem), &device->normal_atlas);
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
    if (device->metallic_atlas) {
      clReleaseMemObject(device->metallic_atlas);
    }
    cl_image_format image_format = { CL_ARGB, CL_UNORM_INT8 };
    cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE2D,
                                 width, height,
                                 1, 1, 0, 0, 0, 0, nullptr };
    LOG("Allocating metallic atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Metallic atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    device->metallic_atlas = clCreateImage(device->context,
                                         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                         &image_format, &image_desc, atlas,
                                         &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create metallic atlas image.")
               return false;
             });
    clSetKernelArg(device->sample_kernel, 7, sizeof(cl_mem), &device->metallic_atlas);
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
    if (device->emission_atlas) {
      clReleaseMemObject(device->emission_atlas);
    }
    cl_image_format image_format = { CL_ARGB, CL_UNORM_INT8 };
    cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE2D,
                                 width, height,
                                 1, 1, 0, 0, 0, 0, nullptr };
    LOG("Allocating emission atlas on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Emission atlas dimensions exceed max dimensions allowed by device.");
      return false;
    }
    device->emission_atlas = clCreateImage(device->context,
                                         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                         &image_format, &image_desc, atlas,
                                         &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create emission atlas image.")
               return false;
             });
    clSetKernelArg(device->primary_kernel, 10, sizeof(cl_mem), &device->emission_atlas);
    clSetKernelArg(device->secondary_kernel, 10, sizeof(cl_mem), &device->emission_atlas);
    clSetKernelArg(device->emission_kernel, 5, sizeof(cl_mem), &device->emission_atlas);
    clSetKernelArg(device->sample_kernel, 8, sizeof(cl_mem), &device->emission_atlas);
    clSetKernelArg(device->nee_kernel, 10, sizeof(cl_mem), &device->emission_atlas);
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
    if (device->environment_map) {
      clReleaseMemObject(device->environment_map);
    }
    cl_image_format image_format = { CL_RGBA, CL_HALF_FLOAT };
    cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE2D,
                                 width, height,
                                 1, 1, 0, 0, 0, 0, nullptr };
    LOG("Allocating environment map on device: %u, with size: %u x %u.",
        d, width, height);
    if ((width > device->max_image_width) ||
        (height > device->max_image_height)) {
      ERR("Environment map dimensions exceed max dimensions allowed by device.");
      return false;
    }
    device->environment_map = clCreateImage(device->context,
                                         CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                         &image_format, &image_desc, map,
                                         &CL_ERR);
    CHECK_CL(CL_ERR, 
             {
               ERR("Failed to create normal atlas image.")
               return false;
             });
    clSetKernelArg(device->env_map_kernel, 1, sizeof(cl_mem), &device->environment_map);
    clSetKernelArg(device->sample_kernel, 11, sizeof(cl_mem), &device->environment_map);
  }
  return true;
}
#endif
