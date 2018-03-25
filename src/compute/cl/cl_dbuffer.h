#if defined(ROVE_CL)
#pragma once
///
#include "../../core/vector.h"
#include "../../core/utility.h"
#include "cl_compute.h"
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
    m_flip_buffers = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    m_flop_buffers = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    m_flip_images = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    m_flop_images = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    m_dummy_buffers = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    m_dummy_images = (cl_mem*)calloc(COMPUTE->device_count, sizeof(cl_mem));
    LOG("Allocating dummy buffers/images.");
    for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
      Device* device = &COMPUTE->devices[d];
      if (!device->available) {
        continue;
      }
      m_dummy_buffers[d] = clCreateBuffer(device->context, 
                                          CL_MEM_READ_ONLY, 
                                          4, nullptr, &CL_ERR); 
      CHECK_CL(CL_ERR, {});
      cl_image_format image_format = { CL_R, CL_FLOAT };
      cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE1D_BUFFER, 1,
                                   1, 1, 1, 0, 0, 0, 0, m_dummy_buffers[d] };
      m_dummy_images[d] = clCreateImage(device->context, 
                                        CL_MEM_READ_ONLY, &image_format,
                                        &image_desc, nullptr, &CL_ERR);
      CHECK_CL(CL_ERR, {});
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
      if (m_dummy_buffers) {
        if (m_dummy_buffers[d]) {
          CHECK_CL(clReleaseMemObject(m_dummy_buffers[d]), {});
        }
      }
      if (m_dummy_images) {
        if (m_dummy_images[d]) {
          CHECK_CL(clReleaseMemObject(m_dummy_images[d]), {});
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
      if (m_h_data) {
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
        if (m_flip_buffers) {
          if (m_flip_buffers[d]) {
            LOG("Freeing flip DBuffer: %.2f MB on device: %u.",
                (sizeof(T) * m_flip_capacity) / 1048576.0f, d);
            CHECK_CL(clReleaseMemObject(m_flip_buffers[d]), {});
          }
        }
        if (m_image && m_flip_images) {
          if (m_flip_images[d]) {
            CHECK_CL(clReleaseMemObject(m_flip_images[d]), {});
          }
        }
      } 
    } else {
      if (m_flop_capacity == 0) {
        return;
      }
      for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
        Device* device = &COMPUTE->devices[d];
        if (m_flop_buffers) {
          if (m_flop_buffers[d]) {
            LOG("Freeing flop DBuffer: %.2f MB on device: %u.",
                (sizeof(T) * m_flop_capacity) / 1048576.0f, d);
            CHECK_CL(clReleaseMemObject(m_flop_buffers[d]), {});
          }
        }
        if (m_image && m_flop_images) {
          if (m_flop_images[d]) {
            CHECK_CL(clReleaseMemObject(m_flop_images[d]), {});
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
      m_pinned = (allocated_bytes < MAX_PINNED_BYTES);
      LOG("Allocating %s host DBuffer: %.2f MB.", 
          m_pinned ? "pinned" : "non-pinned", 
          allocated_bytes / 1048576.0f);
      T* new_data = (T*)_mm_malloc(allocated_bytes, 512);
      if (m_h_data) {
        memcpy(new_data, m_h_data, sizeof(T) * (m_h_count - count));
        _mm_free(m_h_data);
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
        cl_mem* buffer = m_flip ? &m_flip_buffers[d] : &m_flop_buffers[d];
        *buffer = clCreateBuffer(device->context, 
                                 CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY,
                                 allocated_bytes, nullptr, &CL_ERR); 
        CHECK_CL(CL_ERR,
                 {
                   ERR("Failed to allocate %s DBuffer.", 
                   m_flip ? "flip" : "flop");
                   return false;
                 });
        if (m_image) {
					uint32_t element_pixels = sizeof(T) / sizeof(float);
          cl_image_format image_format = { CL_R, CL_FLOAT };
          cl_image_desc image_desc = { CL_MEM_OBJECT_IMAGE1D_BUFFER,
                                       m_h_capacity * element_pixels,
                                       1, 1, 1, 0, 0, 0, 0, *buffer };
          LOG("DBuffer image has: %u pixels.", (m_h_capacity  * element_pixels));
          cl_mem* image_buffer = m_flip ? &m_flip_images[d] : &m_flop_images[d];
          *image_buffer = clCreateImage(device->context, 
                                        CL_MEM_READ_ONLY |
                                        CL_MEM_HOST_WRITE_ONLY, &image_format,
 																				&image_desc, nullptr, &CL_ERR);
					CHECK_CL(CL_ERR,
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
        CHECK_CL(clEnqueueWriteBuffer(device->io_queue, *buffer, CL_TRUE,
                                      0, allocated_bytes, m_h_data, 0, nullptr, 
                                      nullptr),
                 {
                   ERR("Failed to copy into %s device DBuffer.",
                        m_flip ? "flip" : "flop");
                   return false;
                 });
  	    CHECK_CL(clFlush(device->io_queue), {});
      }
    } else {
      if (count > 0) {
        for (uint32_t d = 0; d < COMPUTE->device_count; ++d) {
          Device* device = &COMPUTE->devices[d];
          if (!device->available) {
            continue;
          }
          cl_mem* buffer = m_flip ? &m_flip_buffers[d] : &m_flop_buffers[d];
          CHECK_CL(clEnqueueWriteBuffer(device->io_queue, *buffer, CL_TRUE,
                                        sizeof(T) * start, sizeof(T) * count, 
                                        m_h_data + start, 0, nullptr, nullptr),
                   {
                     ERR("Failed to copy into %s device DBuffer.",
                          m_flip ? "flip" : "flop");
                     return false;
                   });
          CHECK_CL(clFlush(device->io_queue), {});
        }
      }
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
  cl_mem* m_flip_buffers;
  cl_mem* m_flop_buffers;
  cl_mem* m_flip_images;
  cl_mem* m_flop_images;
  cl_mem* m_dummy_buffers;
  cl_mem* m_dummy_images;
  uint32_t m_flip_capacity;
  uint32_t m_flop_capacity;
};
#endif
