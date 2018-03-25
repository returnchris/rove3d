using UnityEngine;
using System.Collections;
#if UNITY_EDITOR
  using UnityEditor;
#endif
///
[System.Serializable]
public class RoveTexture
{
  public static uint STATUS_EMPTY  = 0x0;
  public static uint STATUS_OK     = 0x1;
  public static uint STATUS_NEW    = 0x2;
  public static uint STATUS_CHANGE = 0x3;
  ///
  public uint m_status;
  public Texture2D m_texture;
  public Rect m_atlas_rect;
  ///
  public RoveTexture()
  {
    m_status = STATUS_EMPTY;
    m_texture = null;
    m_atlas_rect = new Rect(0, 0, 0, 0);
  }
}
///
[CreateAssetMenuAttribute(menuName = "Rove3D/Material", 
                          fileName = "NewRoveMaterial.asset",
                          order = 1)]
public class RoveMaterial : ScriptableObject
{
  public uint m_id;
  public bool m_folded_out;
  public uint m_map_flags;
  public bool m_double_sided;
  public Color  m_albedo;
  public float m_metallic;
  public float m_smoothness;
  public float m_glass;
  public float m_emission;
  public RoveTexture[] m_textures;
  ///
  public RoveMaterial()
  {
    m_id = 0;
    m_folded_out = false;
    m_map_flags = 0x0;
    m_double_sided = false;
    m_albedo = new Color(1.0f, 1.0f, 1.0f, 1.0f);
    m_metallic = 0.0f;
    m_smoothness = 0.0f;
    m_glass = 0.0f;
    m_emission = 0.0f;
    m_textures = new RoveTexture[4];
    m_textures[RoveAtlas.TYPE_ALBEDO] = new RoveTexture();
    m_textures[RoveAtlas.TYPE_NORMAL] = new RoveTexture();
    m_textures[RoveAtlas.TYPE_METALLIC] = new RoveTexture();
    m_textures[RoveAtlas.TYPE_EMISSION] = new RoveTexture();
  }
  /// API functions.
  ///
  public void SetMapFlags(uint map_flags)
  {
    m_map_flags = map_flags;
    RoveInternal.SetMaterialMapFlags(m_id, map_flags);
  }
  ///
  public void SetDoubleSided(bool double_sided)
  {
    m_double_sided = double_sided;
    RoveInternal.SetMaterialDoubleSided(m_id, double_sided);
  }
  ///
  public void SetAlbedo(Color albedo)
  {
    m_albedo = albedo; 
    float[] new_albedo = new float[4];
    new_albedo[0] = albedo.r;
    new_albedo[1] = albedo.g;
    new_albedo[2] = albedo.b;
    new_albedo[3] = albedo.a;
    RoveInternal.SetMaterialAlbedo(m_id, new_albedo);
  }
  ///
  public void SetMetallic(float metallic)
  {
    m_metallic = metallic;
    RoveInternal.SetMaterialMetallic(m_id, metallic);
  }
  ///
  public void SetSmoothness(float smoothness)
  {
    m_smoothness = smoothness;
    RoveInternal.SetMaterialSmoothness(m_id, smoothness);
  }
  ///
  public void SetGlass(float glass)
  {
    m_glass = glass;
    RoveInternal.SetMaterialGlass(m_id, glass);
  }
  ///
  public void SetEmission(float emission)
  {
    m_emission = emission;
    RoveInternal.SetMaterialEmission(m_id, emission);
  }
  ///
  public void SetTexture(uint atlas_type, Texture2D texture)
  {
    RoveMaterials rove_materials = RoveSetup.GetAPIComponent<RoveMaterials>();
    if (!rove_materials) {
      Debug.LogError("Failed set material texture.");
      return;
    }
    RoveTexture rove_texture = m_textures[atlas_type];
    if (texture == null) {
      if (rove_texture.m_status != RoveTexture.STATUS_EMPTY) {
        rove_texture.m_status = RoveTexture.STATUS_CHANGE;
      }
    } else {
      if (rove_texture.m_status != RoveTexture.STATUS_OK) {
        rove_texture.m_status = RoveTexture.STATUS_NEW; 
      } else {
        rove_texture.m_status = RoveTexture.STATUS_CHANGE;
      }
    }
    rove_texture.m_texture = texture;
    rove_materials.m_atlases[atlas_type].m_dirty = true;
  }
  ///
  public void SetAtlasRect(uint atlas_type, Rect bounds)
  {
    m_textures[atlas_type].m_atlas_rect = bounds;
    float[] new_bounds = new float[4];
    new_bounds[0] = bounds.min.x;
    new_bounds[1] = bounds.max.x;
    new_bounds[2] = bounds.min.y;
    new_bounds[3] = bounds.max.y;
    if (atlas_type == RoveAtlas.TYPE_ALBEDO) {
      RoveInternal.SetMaterialAlbedoBounds(m_id, new_bounds);
    } else if (atlas_type == RoveAtlas.TYPE_NORMAL) {
      RoveInternal.SetMaterialNormalBounds(m_id, new_bounds);
    } else if (atlas_type == RoveAtlas.TYPE_METALLIC) {
      RoveInternal.SetMaterialMetallicBounds(m_id, new_bounds);
    } else if (atlas_type == RoveAtlas.TYPE_EMISSION) {
      RoveInternal.SetMaterialEmissionBounds(m_id, new_bounds);
    }
  }
}
