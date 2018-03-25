#pragma once
///
#include <cstdint>
///
#define MAX_PINNED_BYTES 500000
///
struct RayState {
  int32_t state;
  float origin[3];
  float direction[3];
  float uv[2];
  float hit_distance;
  int32_t triangle_index;
  float last_brdf_pdf;
  float throughput[3];
  float color[3];
  float sun_d[3];
  float sun_color[3];
  float light_d[3];
  float light_t;
  float light_color[3];
  float portal_d[3];
  float portal_color[3];
};
///
int32_t SetupCompute();
void ShutdownCompute();
int32_t SetupDevice(uint32_t index);
void ShutdownDevice(uint32_t index);
int32_t SetupDeviceTargets(uint32_t index);
void ShutdownDeviceTargets(uint32_t index);
int32_t ResizeTargets();
int32_t SetupSky();
void ResetFieldOfView();
void ResetRenderAtStart();
void ResetRenderAtEnd();
int32_t StartDeviceRender(uint32_t index,
                          uint32_t light_count,
                          uint32_t portal_count);
void FinishDeviceRender(uint32_t index);
void CompositeFinalImage();
int32_t SetAlbedoAtlas(uint8_t* atlas,
                       uint32_t width,
                       uint32_t height);
int32_t SetNormalAtlas(uint8_t* atlas,
                       uint32_t width,
                       uint32_t height);
int32_t SetMetallicAtlas(uint8_t* atlas,
                         uint32_t width,
                         uint32_t height);
int32_t SetEmissionAtlas(uint8_t* atlas,
                         uint32_t width,
                         uint32_t height);
int32_t SetEnvironmentMap(uint8_t* map,
                          uint32_t width,
                          uint32_t height);
