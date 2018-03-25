#pragma once
///
#include <string>
#include <cstdint>
#include "utility.h"
///
template <typename T>
struct Vector 
{
  ///
  void Setup(uint32_t alignment, uint32_t reserve)
  {
    m_count = 0;
    m_capacity = reserve;
    m_aligned = (alignment != 0);
    m_alignment = alignment;
    if (reserve > 0) {
      if (m_aligned) {
        m_data = (T*)_mm_malloc(sizeof(T) * m_capacity, m_alignment);
      } else {
        m_data = (T*)malloc(sizeof(T) * m_capacity);
      }
    } else {
      m_data = nullptr;
    }
  }
  ///
  void Shutdown()
  {
    if (m_data) {
      if (m_aligned) {
        _mm_free(m_data);
      } else {
        free(m_data);
      }
    }
    m_count = 0;
    m_capacity = 0;
    m_aligned = false;
    m_alignment = 0;
  }
  ///
  T* Emplace(uint32_t count)
  {
    m_count += count;
    if (m_count > m_capacity) {
      NEXT_POWER_OF_TWO(m_count, m_capacity);
      T* new_data;
      if (m_aligned) {
        new_data = (T*)_mm_malloc(sizeof(T) * m_capacity, m_alignment);
        if (m_data) {
          memcpy(new_data, m_data, sizeof(T) * (m_count - count));
          _mm_free(m_data);
        }
      } else {
        new_data = (T*)malloc(sizeof(T) * m_capacity);
        if (m_data) {
          memcpy(new_data, m_data, sizeof(T) * (m_count - count));
          free(m_data);
        }
      }
      m_data = new_data;
    }
    return &m_data[m_count - count];
  }
  ///
  void Free(uint32_t start, uint32_t free_count) 
  {
    uint32_t copy_count = m_count - start - free_count;
    memcpy(&m_data[start], &m_data[start + free_count], sizeof(T) * copy_count);
    m_count -= free_count;
  }
  ///
  T Pop()
  {
    return m_data[--m_count];
  }
  ///
  void Clear()
  {
    m_count = 0;
  }
  ///
  T& operator[](uint32_t index)
  {
    return m_data[index];
  }
  ///
  uint32_t m_count;
  uint32_t m_capacity;
  int32_t m_aligned;
  uint32_t m_alignment;
  T* m_data;
};
