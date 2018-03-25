//#define ROVE_FORCE_GL
using UnityEngine;
using System;
using System.IO;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
#if UNITY_EDITOR
  using UnityEditor;
#endif
///
public class RoveMaterials : MonoBehaviour
{
  public static uint ATLAS_COUNT = 4;
  public static uint MAP_FLAG_ALBEDO   = 0x1;
  public static uint MAP_FLAG_NORMAL   = 0x2;
  public static uint MAP_FLAG_METALLIC = 0x4;
  public static uint MAP_FLAG_EMISSION = 0x8;
  ///
  public List<RoveMaterial> m_materials;
  public RoveAtlas[] m_atlases; 
  ///
  public void Start()
  {
    m_materials = new List<RoveMaterial>();
    m_atlases = new RoveAtlas[ATLAS_COUNT];
    for (uint i = 0; i < ATLAS_COUNT; ++i) {
      m_atlases[i] = new RoveAtlas(i);
    }
  }
  ///
  public uint AddMaterial(RoveMaterial material)
  {
    int index = 0;
    for (int m = 0; m < m_materials.Count; ++m) {
      if (material == m_materials[m]) {
        index = m + 1;
      }
    }
    if (index == 0) {
      index = m_materials.Count + 1;
      float[] albedo = new float[4];
      albedo[0] = material.m_albedo.r;
      albedo[1] = material.m_albedo.g;
      albedo[2] = material.m_albedo.b;
      albedo[3] = material.m_albedo.a;
      // Map flags and bounds will be set during atlas processing.
      material.m_map_flags = 0x0;
      float[] albedo_bounds = new float[4];
      float[] normal_bounds = new float[4];
      float[] metallic_bounds = new float[4];
      float[] emission_bounds = new float[4];
      for (uint m = 0; m < ATLAS_COUNT; ++m) {
        if (material.m_textures[m].m_texture == null) {
          material.m_textures[m].m_status = RoveTexture.STATUS_EMPTY;
        } else {
          material.m_textures[m].m_status = RoveTexture.STATUS_NEW;
        }
      }
      uint mat_id = RoveInternal.AddMaterial(material.m_map_flags,
                                             material.m_double_sided,
                                             albedo,
                                             material.m_metallic,
                                             material.m_smoothness,
                                             material.m_glass,
                                             material.m_emission,
                                             albedo_bounds,
                                             normal_bounds,
                                             metallic_bounds,
                                             emission_bounds);
      if ((mat_id == RoveSetup.ERROR) || (mat_id != index)) {
        Debug.LogError("Rove: Failed to add material; check 'Rove/Data/rove.log' " +
                       "for details");
        return RoveSetup.ERROR;
      }
      material.m_id = mat_id;
      m_materials.Add(material);
      for (int a = 0; a < ATLAS_COUNT; ++a) {
        m_atlases[a].m_dirty = true;
      }
    }
    return (uint)index;
  }
}
