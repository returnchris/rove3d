#pragma once
#include <atomic>
#include <thread>
#include "rove_api.h"
#include "memory.h"
#if defined(ROVE_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/CGLDevice.h>
  #include <OpenGL/CGLCurrent.h>
#elif defined(ROVE_LINUX)
  #include <GL/gl.h>
  #include <GL/glx.h>
#elif defined(ROVE_WINDOWS)
  #include <Windows.h>
  #if defined(ROVE_GL)
    #include <GL/gl.h>
  #elif defined(ROVE_D3D11)
    #include <d3d11.h>
    extern ID3D11Device* rove_d3d_device;
  #endif
#endif
///
struct Rove
{
  std::atomic<RoveLock> lock;
  char* data_path;
  FILE* log;
  uint32_t thread_count;
  int32_t use_multi_device;
  int32_t use_integrated_device;
  uint32_t optimization_level;
  float field_of_view;
  uint32_t max_bounces;
  uint32_t samples_per_frame;
  int32_t target_width;
  int32_t target_height;
  void* target;
  std::thread scene_thread;
  struct ThreadPool* thread_pool;
  struct Compute* compute;
  struct Scene* scene;
  struct Tree* tree;
};
extern Rove rove;
