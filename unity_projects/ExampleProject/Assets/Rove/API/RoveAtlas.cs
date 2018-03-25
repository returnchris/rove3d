/* Special thanks to François Dujardin (www.judiva.com) */ 
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine.Serialization;
using RoveStatus = System.UInt32;
///
public class RoveAtlas
{
  public static uint TYPE_ALBEDO    = 0x0;
  public static uint TYPE_NORMAL    = 0x1;
  public static uint TYPE_METALLIC  = 0x2;
  public static uint TYPE_EMISSION  = 0x3;
  ///
  public uint m_type;
  public bool m_dirty;
  public int m_width;
  public int m_height;
  public List<Rect> m_available;
  public Texture2D m_texture;
  public Material m_atlas_material;
  ///
  public RoveAtlas(uint atlas_type)
  {
    m_type = atlas_type;
    m_dirty = true;
    Reset(4, 4);
    m_atlas_material = (Material)Resources.Load("RoveAtlasMaterial");
  }
  ///
  public class RectWidthAscending : IComparer<Rect>
  {
    public int Compare(Rect a,
                       Rect b)
    {
      if (a.width == b.width) {
        return 0;
      } else if (a.width < b.width) {
        return -1;
      }
      return 1;
    }
  }
  ///
  public static int PredictSize(int area)
  {
    int alpha = (int)Mathf.Log((float)area, 2) + 1;
    int predicted_width = 0x1 << (alpha >> 1); 
    int predicted_height = 0x1 << (alpha - (alpha >> 1));
    return Mathf.Max(predicted_width, predicted_height);
  }
  ///
  public void Reset(int width, int height)
  {
    if (m_available != null) {
      m_available.Clear();
    } else {
      m_available = new List<Rect>();
    }
    m_available.Add(new Rect(0, 0, width, height));
    m_width = width;
    m_height = height;
  }
  ///
  public void InsertAvailable(Rect rect)
  {
    int index = m_available.BinarySearch(rect, new RectWidthAscending());
    if (index < 0) {
      index = ~index;
    }
    if (index == m_available.Count) {
      m_available.Add(new Rect(rect));
    } else {
      m_available.Insert(index, new Rect(rect));
    }
  }
  ///
  public bool Pack(RoveTexture rove_texture)
  {
    int width = rove_texture.m_texture.width;
    int height = rove_texture.m_texture.height;
    rove_texture.m_atlas_rect.width = width;
    rove_texture.m_atlas_rect.height= height;
    int index = m_available.BinarySearch(rove_texture.m_atlas_rect,
                                         new RectWidthAscending());
    if (index < 0) {
      index = ~index;
    }
    if (index == m_available.Count) {
      return false;
    } else {
      while (index < m_available.Count) {
        Rect available = m_available[index];
        if (available.height > height) {
          rove_texture.m_atlas_rect.x = available.x;
          rove_texture.m_atlas_rect.y = available.y;
          m_available.RemoveAt(index);
          if (available.width == width) {
            InsertAvailable(new Rect(available.x, available.y + height,
                                     available.width, available.height - height));
          } else {
            InsertAvailable(new Rect(available.x, available.y + height,
                                     available.width, available.height - height));
            InsertAvailable(new Rect(available.x + width, available.y,
                                     available.width - width, height));
          }
          return true;
        } else if (available.height == height) {
          rove_texture.m_atlas_rect.x = available.x;
          rove_texture.m_atlas_rect.y = available.y;
          m_available.RemoveAt(index);
          if (available.width > width) {
            InsertAvailable(new Rect(available.x + width, available.y,
                                     available.width - width, available.height));
          }
          return true;
        }
        ++index;
      }
    }    
    return false;
  }
  ///
  public void Remove(RoveTexture rove_texture)
  {
    InsertAvailable(rove_texture.m_atlas_rect);
  }
  ///
  public bool ProcessAtlas()
  {
    if (!m_dirty) {
      return true;
    }
    RoveMaterials rove_materials = RoveSetup.GetAPIComponent<RoveMaterials>();
    if (!rove_materials) {
      Debug.LogError("Rove: Failed to begin to process atlas.");
      return false;
    }
    List<RoveMaterial> materials = rove_materials.m_materials;
    uint map_flag = 0x0;
    if (m_type == RoveAtlas.TYPE_ALBEDO) {
      map_flag = RoveMaterials.MAP_FLAG_ALBEDO;
    } else if (m_type == RoveAtlas.TYPE_NORMAL) {
      map_flag = RoveMaterials.MAP_FLAG_NORMAL;
    } else if (m_type == RoveAtlas.TYPE_METALLIC) {
      map_flag = RoveMaterials.MAP_FLAG_METALLIC;
    } else if (m_type == RoveAtlas.TYPE_EMISSION) {
      map_flag = RoveMaterials.MAP_FLAG_EMISSION;
    }
    List<int> dirty_textures = new List<int>();
    int area = 0;
    for (int m = 0; m < materials.Count; ++m) {
      RoveMaterial material = materials[m];
      RoveTexture rove_texture = material.m_textures[m_type];
      uint status = rove_texture.m_status;
      if ((status == RoveTexture.STATUS_EMPTY) ||
          (status == RoveTexture.STATUS_OK)) {
        continue;
      } else if (status == RoveTexture.STATUS_CHANGE) {
        Remove(rove_texture);
        if (rove_texture.m_texture == null) {
          rove_texture.m_status = RoveTexture.STATUS_EMPTY;
          material.m_map_flags &= ~map_flag;
          material.SetMapFlags(material.m_map_flags);
          continue;
        }
      }
      material.m_map_flags |= map_flag;
      material.SetMapFlags(material.m_map_flags);
      area += rove_texture.m_texture.width * rove_texture.m_texture.height;
      dirty_textures.Add(m);
    }
    int predicted_size = RoveAtlas.PredictSize(area);
    if (predicted_size > m_width) {
      if (predicted_size > 16384) {
        Debug.LogError("Rove: Predicted atlas exceeds 16384 x 16384, " + 
                       "cannot create atlas.");
        return false;
      }
      m_width = m_height = predicted_size;
      if (!Repack()) {
        return false;
      }
    } else {
      if (dirty_textures.Count > 1) {
        dirty_textures.Sort((a, b) =>
          {
            RoveTexture rove_texture_a = materials[a].m_textures[m_type];
            RoveTexture rove_texture_b = materials[b].m_textures[m_type];
            int a_size = rove_texture_a.m_texture.width * 
                         rove_texture_a.m_texture.height;
            int b_size = rove_texture_b.m_texture.width * 
                         rove_texture_b.m_texture.height;
            return b_size.CompareTo(a_size);
          });
      }
      for (int t = 0; t < dirty_textures.Count; ++t) {
        RoveMaterial material = materials[dirty_textures[t]];
        RoveTexture rove_texture = material.m_textures[m_type];
        if (!Pack(rove_texture)) {
          if (!Repack()) {
            return false;
          } else {
            break;
          }
        } else {
          material.SetAtlasRect(m_type, rove_texture.m_atlas_rect);
          rove_texture.m_status = RoveTexture.STATUS_OK;
        }
      }
    }
    RenderTexture temp_rt = RenderTexture.GetTemporary(m_width, m_height,
                                                       0, RenderTextureFormat.ARGB32);
    RenderTexture.active = temp_rt;
    GL.PushMatrix();
    GL.LoadPixelMatrix(0, m_width, m_height, 0);
    GL.Clear(false, true, new Color(0.0f, 0.0f, 0.0f, 1.0f));
    for (int m = 0; m < materials.Count; ++m) {
      Texture2D texture = materials[m].m_textures[m_type].m_texture;
      if (texture == null) {
        continue;
      }
      Rect bl_origin = new Rect(materials[m].m_textures[m_type].m_atlas_rect);
      bl_origin.y = (m_height - bl_origin.y) - bl_origin.height;
      m_atlas_material.SetTexture("target", materials[m].m_textures[m_type].m_texture);
      Graphics.DrawTexture(bl_origin, texture, m_atlas_material);
    }
    GL.PopMatrix();
    m_texture = new Texture2D(m_width, m_height, TextureFormat.ARGB32, false);
    m_texture.ReadPixels(new Rect(0, 0, m_width, m_height), 0, 0, false); 
    m_texture.Apply();
    RenderTexture.active = null;
    RenderTexture.ReleaseTemporary(temp_rt);
    UploadAtlas();
    m_dirty = false;
    return true;
  }
  ///
  public bool Repack()
  {
    RoveMaterials rove_materials = RoveSetup.GetAPIComponent<RoveMaterials>();
    if (!rove_materials) {
      Debug.LogError("Rove: Failed to begin to process atlas.");
      return false;
    }
    List<RoveMaterial> materials = rove_materials.m_materials;
    List<int> dirty_textures = new List<int>();
    int area = 0;
    for (int t = 0; t < materials.Count; ++t) {
      RoveTexture rove_texture = materials[t].m_textures[m_type];
      uint status = rove_texture.m_status;
      if ((rove_texture.m_texture == null) ||
          (status == RoveTexture.STATUS_EMPTY)) {
        continue;
      }
      dirty_textures.Add(t);
      area += rove_texture.m_texture.width * rove_texture.m_texture.height;
    }
    int predicted_size = RoveAtlas.PredictSize(area);
    if (predicted_size > m_width) {
      if (predicted_size > 16384) {
        Debug.LogError("Rove: Predicted atlas exceeds 16384 x 16384, " + 
                       "cannot create atlas.");
        return false;
      }
      Reset(predicted_size, predicted_size);
    } else {
      Reset(predicted_size, predicted_size);
    }
    if (dirty_textures.Count > 1) {
      dirty_textures.Sort((a, b) =>
        {
          RoveTexture rove_texture_a = materials[a].m_textures[m_type];
          RoveTexture rove_texture_b = materials[b].m_textures[m_type];
          int a_size = rove_texture_a.m_texture.width * 
                       rove_texture_a.m_texture.height;
          int b_size = rove_texture_b.m_texture.width * 
                       rove_texture_b.m_texture.height;
          return b_size.CompareTo(a_size);
        });
    }
    bool packed = false;
    while (!packed && (m_width <= 16384) && (m_height <= 16384)) {
      for (int t = 0; t < dirty_textures.Count; ++t) {
        RoveMaterial material = materials[dirty_textures[t]];
        RoveTexture rove_texture = material.m_textures[m_type];
        if (!Pack(rove_texture)) {
          Reset(m_width * 2, m_height * 2);
          break;
        } else {
          material.SetAtlasRect(m_type, rove_texture.m_atlas_rect);
          rove_texture.m_status = RoveTexture.STATUS_OK;
        }
        if (t == (dirty_textures.Count - 1)) {
          packed = true;
        } 
      }
    }
    if (!packed ){
      Debug.LogError("Rove: Failed to repack atlas: " + m_type + ".");
      return false;
    }
    return true;
  }
  ///
  public void UploadAtlas()
  {
    byte[] atlas_bytes = m_texture.GetRawTextureData();
    RoveStatus result = 0;
    uint width = (uint)m_width;
    uint height = (uint)m_height;
    if (m_type == RoveAtlas.TYPE_ALBEDO) {
      result = RoveInternal.SetAlbedoAtlas(atlas_bytes, width, height);
    } else if (m_type == RoveAtlas.TYPE_NORMAL) {
      result = RoveInternal.SetNormalAtlas(atlas_bytes, width, height);
    } else if (m_type == RoveAtlas.TYPE_METALLIC) {
      result = RoveInternal.SetMetallicAtlas(atlas_bytes, width, height);
    } else if (m_type == RoveAtlas.TYPE_EMISSION) {
      result = RoveInternal.SetEmissionAtlas(atlas_bytes, width, height);
    }
    if (result != RoveSetup.SUCCESS) {
      Debug.LogError("Rove: Failed to upload atlas: " + m_type + ".");
    }
  }
}
