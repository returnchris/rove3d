#if defined(ROVE_CUDA)
#pragma once
///
#include "../../core/vector.h"
#include "../../core/utility.h"
#include "cuda_compute.h"
///
template <typename T>
struct DBuffer
{
  ///
  void Setup(bool allow_image)
  {
    m_allow_image = allow_image;
    m_image = allow_image;
    m_pinned = false;
    m_flip = true;
    m_h_data = nullptr;
    m_h_capacity = 2;
    m_h_count = 0;
    m_h_data = (T*)_mm_malloc(sizeof(T) * 2, 512);
    m_flip_buffers = (CUdeviceptr*)calloc(COMPUTE->device_count,
                                          sizeof(CUdeviceptr));
    m_flop_buffers = (CUdeviceptr*)calloc(COMPUTE->device_count,
                                          sizeof(CUdeviceptr));
    m_flip_images = (CUtexObject*)calloc(COMPUTE->device_count,
                                         sizeof(CUtexObject));
    m_flop_images = (CUtexObject*)calloc(COMPUTE->device_count,
                                         sizeof(CUtexObject));
    m_dummy_buffers = (CUdeviceptr*)calloc(COMPUTE->device_count,
                                           sizeof(CUdeviceptr));
    m_dummy_images = (CUtexObject*)calloc(COMPUTE->device_count,
                                          sizeof(CUtexObject));
    LOG("Allocating dummy buffers/images.");
    for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
      Device* device = &COMPUTE->devices[d];
      if(!device->available || 
         (cuCtxSetCurrent(device->context) != CUDA_SUCCESS)) {
        continue;
      }
      CHECK_CUDA(cuMemAlloc(&m_dummy_buffers[d], 4), {});
      CUDA_RESOURCE_DESC resource_desc;
      memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
      resource_desc.resType = CU_RESOURCE_TYPE_LINEAR; 
      resource_desc.res.linear.devPtr = m_dummy_buffers[d];
      resource_desc.res.linear.format = CU_AD_FORMAT_FLOAT;  
      resource_desc.res.linear.numChannels = 1;
      resource_desc.res.linear.sizeInBytes = 4;
      CUDA_TEXTURE_DESC tex_desc;
      memset(&tex_desc, 0, sizeof(CUDA_TEXTURE_DESC));
      CUDA_RESOURCE_VIEW_DESC resource_view_desc;
      memset(&resource_view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
      CHECK_CUDA(cuTexObjectCreate(&m_dummy_images[d], &resource_desc,
                                   &tex_desc, &resource_view_desc), {});
    }
    m_flip_capacity = 0;
    m_flop_capacity = 0;
  }
  ///
  void Shutdown()
  {
    LOG("Shutting down DBuffer.");
    FreeDeviceBuffers(true);
    FreeDeviceBuffers(false);
    if (m_flip_buffers) {
      free(m_flip_buffers);
    }
    if (m_flop_buffers) {
      free(m_flop_buffers);
    }
    if (m_flip_images) {
      free(m_flip_images);
    }
    if (m_flop_images) {
      free(m_flop_images);
    }
    LOG("Freeing dummy buffers/images.");
    for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
      Device* device = &COMPUTE->devices[d];
      if(cuCtxSetCurrent(device->context) != CUDA_SUCCESS) {
        continue;
      }
      if (m_dummy_buffers) {
        if (m_dummy_buffers[d]) {
          CHECK_CUDA(cuMemFree(m_dummy_buffers[d]), {});
        }
      }
      if (m_dummy_images) {
        if (m_dummy_images[d]) {
          CHECK_CUDA(cuTexObjectDestroy(m_dummy_images[d]), {});
        }
      }
    } 
    if (m_dummy_buffers) {
      free(m_dummy_buffers);
    }
    if (m_dummy_images) {
      free(m_dummy_images);
    }
    if (m_h_data) {
      LOG("Freeing host DBuffer: %.2f MB.",
          (sizeof(T) * m_h_capacity) / 1048576.0f);
      if (m_pinned) {
        CHECK_CUDA(cuCtxSetCurrent(DISPLAY_DEVICE->context), {});
        CHECK_CUDA(cuMemFreeHost(m_h_data), {});
      } else {
        _mm_free(m_h_data);
      }
    }
    memset(this, 0, sizeof(DBuffer));
  }
  ///
  void FlipBuffer()
  {
    m_flip = !m_flip;
  }
  ///
  void FreeDeviceBuffers(int32_t flip)
  {
    if (flip) {
      if (m_flip_capacity == 0) {
        return;
      }
      for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
        Device* device = &COMPUTE->devices[d];
        if(cuCtxSetCurrent(device->context) != CUDA_SUCCESS) {
          continue;
        }
        if (m_flip_buffers) {
          if (m_flip_buffers[d]) {
            LOG("Freeing flip DBuffer: %.2f MB on device: %u.",
                (sizeof(T) * m_flip_capacity) / 1048576.0f, d);
            CHECK_CUDA(cuMemFree(m_flip_buffers[d]), {});
          }
        }
        if (m_image && m_flip_images) {
          if (m_flip_images[d]) {
            CHECK_CUDA(cuTexObjectDestroy(m_flip_images[d]), {});
          }
        }
      } 
    } else {
      if (m_flop_capacity == 0) {
        return;
      }
      for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
        Device* device = &COMPUTE->devices[d];
        if(cuCtxSetCurrent(device->context) != CUDA_SUCCESS) {
          continue;
        }
        if (m_flop_buffers) {
          if (m_flop_buffers[d]) {
            LOG("Freeing flop DBuffer: %.2f MB on device: %u.",
                (sizeof(T) * m_flop_capacity) / 1048576.0f, d);
            CHECK_CUDA(cuMemFree(m_flop_buffers[d]), {});
          }
        }
        if (m_image && m_flop_images) {
          if (m_flop_images[d]) {
            CHECK_CUDA(cuTexObjectDestroy(m_flop_images[d]), {});
          }
        }
      } 
    }
  }
  ///
  T* Emplace(uint32_t count)
  {
    m_h_count += count;
    if (m_h_count > m_h_capacity) {
      NEXT_POWER_OF_TWO(m_h_count, m_h_capacity);
      uint32_t allocated_bytes = (sizeof(T) * m_h_capacity);
      int32_t was_pinned = m_pinned;
      m_pinned = (allocated_bytes < MAX_PINNED_BYTES);
      LOG("Allocating %s host DBuffer: %.2f MB.", 
          m_pinned ? "pinned" : "non-pinned", 
          allocated_bytes / 1048576.0f);
      T* new_data;
      if (m_pinned) {
        CHECK_CUDA(cuCtxSetCurrent(DISPLAY_DEVICE->context), 
                   {
                     ERR("Failed to set display device context current for "
                         "allocating pinned host DBuffer.");
                     return nullptr;
                   });
        CHECK_CUDA(cuMemHostAlloc((void**)&new_data, allocated_bytes,
                                  CU_MEMHOSTALLOC_PORTABLE |
                                  CU_MEMHOSTALLOC_DEVICEMAP |
                                  CU_MEMHOSTALLOC_WRITECOMBINED),
                   {
                     ERR("Failed to allocate pinned host DBuffer.");
                     return nullptr;
                   });
      } else {
        new_data = (T*)_mm_malloc(allocated_bytes, 512);
      }
      if (m_h_data) {
        memcpy(new_data, m_h_data, sizeof(T) * (m_h_count - count));
        if (was_pinned) {
          CHECK_CUDA(cuMemFreeHost(m_h_data), {});
        } else {
          _mm_free(m_h_data);
        }
      }
      m_h_data = new_data;
    } 
    return &m_h_data[m_h_count - count];
  }
  ///
  int32_t Upload(uint32_t start, uint32_t count)
  {
    uint32_t allocated_bytes = sizeof(T) * m_h_capacity;
    uint32_t capacity = m_flip ? m_flip_capacity : m_flop_capacity;
    if (m_h_capacity > capacity) {
      FreeDeviceBuffers(m_flip);
      uint32_t image_buffer_width = allocated_bytes / sizeof(float);
      m_image = m_allow_image;
      for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
        Device* device = &COMPUTE->devices[d];
        if (!device->available) {
          continue;
        }
        if ((device->is_integrated) ||
            (image_buffer_width > device->max_image_buffer_width)) {
          LOG("Unable to support buffer as image.");
          m_image = false;
          break;
        }
      }
      for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
        Device* device = &COMPUTE->devices[d];
        if (!device->available) {
          continue;
        }
        LOG("Allocating %s device %s DBuffer: %.2f MB on device: %u.", 
            m_image ? "image" : "non-image", 
            m_flip ? "flip" : "flop",
            allocated_bytes / 1048576.0f, d);
        CHECK_CUDA(cuCtxSetCurrent(device->context),
                   {
                     ERR("Failed to set device context as current.");
                     return false;
                   });
        CUdeviceptr* buffer = m_flip ? &m_flip_buffers[d] : &m_flop_buffers[d];
        CHECK_CUDA(cuMemAlloc(buffer, allocated_bytes),
                   {
                     ERR("Failed to allocate %s DBuffer.", 
                     m_flip ? "flip" : "flop");
                     return false;
                   });
        if (m_image) {
          CUDA_RESOURCE_DESC resource_desc;
          memset(&resource_desc, 0, sizeof(CUDA_RESOURCE_DESC));
          resource_desc.resType = CU_RESOURCE_TYPE_LINEAR; 
          resource_desc.res.linear.devPtr = *buffer;
          resource_desc.res.linear.format = CU_AD_FORMAT_FLOAT;  
          resource_desc.res.linear.numChannels = 1;
          resource_desc.res.linear.sizeInBytes = allocated_bytes;
          LOG("DBuffer image has: %u pixels.", 
              (uint32_t)(allocated_bytes / sizeof(float)));
          CUDA_TEXTURE_DESC tex_desc;
          memset(&tex_desc, 0, sizeof(CUDA_TEXTURE_DESC));
          CUDA_RESOURCE_VIEW_DESC resource_view_desc;
          memset(&resource_view_desc, 0, sizeof(CUDA_RESOURCE_VIEW_DESC));
          CUtexObject* image = m_flip ? &m_flip_images[d] : &m_flop_images[d];
          CHECK_CUDA(cuTexObjectCreate(image, &resource_desc,
                                       &tex_desc, &resource_view_desc),
                     {
                       ERR("Failed to create %s DBuffer image.",
                           m_flip ? "flip" : "flop");
                       return false;
                     });
        }
        if (m_flip) {
          m_flip_capacity = m_h_capacity;
        } else {
          m_flop_capacity = m_h_capacity;
        }
        CHECK_CUDA(cuMemcpyHtoD(*buffer, m_h_data,
                                sizeof(T) * m_h_count),
                   {
                     ERR("Failed to copy into %s device DBuffer.",
                          m_flip ? "flip" : "flop");
                     return false;
                   });
      }
    } else {
      //if (count > 0) {
        for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
          Device* device = &COMPUTE->devices[d];
          if (!device->available) {
            continue;
          }
          CHECK_CUDA(cuCtxSetCurrent(device->context),
                     {
                       ERR("Failed to set device context as current on "
                           "device: %u.", d);
                       return false;
                     });
          CUdeviceptr* buffer = m_flip ? &m_flip_buffers[d] : 
                                         &m_flop_buffers[d];
          CHECK_CUDA(cuMemcpyHtoD(*buffer + (start * sizeof(T)), 
                                  m_h_data + start,
                                  sizeof(T) * count),
                     {
                       ERR("Failed to copy into %s device DBuffer on "
                            "device: %u.", 
                            m_flip ? "flip" : "flop", d);
                       return false;
                     });
        }
      //}
    }    
    return true;   
  }
  ///
  void Clear()
  {
    m_h_count = 0;
  }
  ///
  T& operator[](uint32_t index)
  {
    return m_h_data[index];
  }
  ///
  int32_t m_allow_image;
  int32_t m_image;
  int32_t m_pinned;
  int32_t m_flip;
  T* m_h_data;
  uint32_t m_h_capacity;
  uint32_t m_h_count;
  CUdeviceptr* m_flip_buffers;
  CUdeviceptr* m_flop_buffers;
  CUtexObject* m_flip_images;
  CUtexObject* m_flop_images;
  CUdeviceptr* m_dummy_buffers;
  CUtexObject* m_dummy_images;
  uint32_t m_flip_capacity;
  uint32_t m_flop_capacity;
};
#endif
