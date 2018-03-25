#pragma once
///
#include <cstdint>
#include "vector.h"
///
template <typename T>
struct List
{
  ///
  void Setup()
  {
    m_items.Setup(0, 0);
    m_free_indices.Setup(0, 0);
  }
  ///
  void Shutdown()
  {
    m_items.Shutdown();
    m_free_indices.Shutdown();
  }
  ///
  T* Emplace()
  {
    uint32_t index;
    if (m_free_indices.m_count > 0) {
      return &m_items[m_free_indices.Pop()];
    } else {
      return m_items.Emplace(1);
    }
  }
  ///
  void Free(T* item)
  {
    uint32_t free_index = item - m_items.m_data;
    *m_free_indices.Emplace(1) = free_index;
  }
  ///
  void Free(uint32_t index)
  {
    *m_free_indices.Emplace(1) = index;
  }
  ///
  void Clear()
  {
    m_items.Clear();
    m_free_indices.Clear();
  }
  ///
  T& operator[](uint32_t index)
  {
    return m_items[index];
  }
  ///
  Vector<T> m_items;
  Vector<uint32_t> m_free_indices;
};
