#pragma once
///
#include <future>
#include "../core/vector.h"
#include "../core/list.h"
#include "mesh.h"
///
#define ENVIRONMENT_SKY 0x0
#define ENVIRONMENT_MAP 0x1
///
typedef struct Camera
{
  float aperture_size;
  float focal_depth;
  float transform[16];
  float root_pixel[2];
  float pixel_offset[2];
  float image_plane_distance;
  float padding;
} Camera;
///
struct ImageProperties
{
  int32_t tonemap;
  float gamma;
  float exposure;
  float contrast;
  float saturation;
  float brightness;
};
///
struct Sun
{
  float direction[4];
  float sun_intensity;
  float sky_intensity;
  float color[4];
};
///
struct EnvironmentProperties
{
  float intensity;
  float rotation;
};
///
struct Scene
{
  int32_t free_flag;
  int32_t update_flag;
  int32_t update_materials_flag;
  int32_t update_emitters_flag;
  int32_t update_portals_flag;
  uint32_t environment_type;
  Camera camera;
	Sun sun;
  EnvironmentProperties environment_properties;
  ImageProperties image_properties;
  Bounds bounds;
  List<Mesh> meshes;
  Vector<uint32_t> mesh_indices;
  Vector<uint64_t> lsh_buffers[2];
  Vector<F4> vertex_buffer;
};
///
int32_t SetupScene();
void ShutdownScene();
void UpdateScene(RoveStatus* scene_status);
int32_t UploadScene();
void SetCameraApertureSize(float aperture_size);
void SetCameraFocalDepth(float focal_depth);
void SetCameraTransform(float* transform);
void SetEnvironmentType(uint32_t type);
void SetSunDirection(float* direction);
void SetSkyIntensity(float intensity);
void SetSunIntensity(float intensity);
void SetSunColor(float* color);
void SetEnvironmentProperties(float intensity,
                              float rotation);
void SetImageProperties(int32_t tonemap, 
                        float gamma, 
                        float exposure, 
                        float contrast, 
                        float saturation, 
                        float brightness);
