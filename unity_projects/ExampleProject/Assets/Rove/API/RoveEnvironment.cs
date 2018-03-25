using UnityEngine;
using System.Collections;
///
public class RoveEnvironment : MonoBehaviour
{
  public uint m_type;
  public float m_sky_intensity;
  public float m_sun_intensity;
  public Color m_sun_color;
  public Light m_sun_light;
  public Texture2D m_environment_map;
  public float m_map_intensity;
  public float m_map_rotation;
  public Material m_atlas_material;
  private RoveSetup m_rove_setup;
  ///
  public static uint TYPE_SKY = 0x0;
  public static uint TYPE_MAP = 0x1;
  ///
  void Start()
  {
	  m_rove_setup = RoveSetup.GetAPIComponent<RoveSetup>();
    if (!m_rove_setup || !m_rove_setup.enabled) {
      Debug.LogError("Rove: Could not find an active RoveSetup component.");
      Destroy(this);
      return;
    }
    m_sun_light = this.GetComponent<Light>();
    if (!m_sun_light) {
      Debug.LogError("Rove: Could not find a light component for sun.");
      Destroy(this);
      return;
    }
    m_atlas_material = (Material)Resources.Load("RoveAtlasMaterial");
    gameObject.transform.hasChanged = true;
  }
  ///
  void Update()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (gameObject.transform.hasChanged) {
      float[] sun_dir = new float[3];
      sun_dir[0] = gameObject.transform.forward.x;
      sun_dir[1] = gameObject.transform.forward.y;
      sun_dir[2] = gameObject.transform.forward.z;
      SetSunDirection(sun_dir);
      SetSkyIntensity(m_sky_intensity);
      SetSunIntensity(m_sun_intensity);
      SetSunColor(m_sun_color);
      gameObject.transform.hasChanged = false;
    }
  }
  /// API functions.
  ///
  public void SetType(uint type)
  {
    m_type = type;
    RoveInternal.SetEnvironmentType(type);
  }
  ///
  public void SetSunDirection(float[] direction)
  {
    RoveInternal.SetSunDirection(direction);
  }
  ///
  public void SetSkyIntensity(float intensity)
  {
    m_sky_intensity = intensity;
    RoveInternal.SetSkyIntensity(intensity);
  }
  ///
  public void SetSunIntensity(float intensity)
  {
    m_sun_intensity = intensity;
    RoveInternal.SetSunIntensity(intensity);
  }
  ///
  public void SetSunColor(Color color)
  {
    m_sun_color = color;
    float[] new_color = new float[4];
    new_color[0] = color.r;
    new_color[1] = color.g;
    new_color[2] = color.b;
    new_color[3] = 1.0f;
    RoveInternal.SetSunColor(new_color);
  }
  ///
  public void SetEnvironmentMap(Texture2D environment_map)
  {
    if (environment_map != null) {
      m_environment_map = environment_map;
      RenderTexture temp_rt = RenderTexture.GetTemporary(environment_map.width, 
                                                         environment_map.height,
                                                         0, 
                                                         RenderTextureFormat.ARGBHalf);
      RenderTexture.active = temp_rt;
      GL.PushMatrix();
      GL.LoadPixelMatrix(0, environment_map.width, environment_map.height, 0);
      m_atlas_material.SetTexture("target", environment_map);
      Graphics.DrawTexture(
        new Rect(0, 0, environment_map.width, environment_map.height),
        environment_map, m_atlas_material);
      GL.PopMatrix();
      Texture2D readable_map = new Texture2D(environment_map.width,
                                             environment_map.height,
                                             TextureFormat.RGBAHalf, false);
      readable_map.ReadPixels(new Rect(0, 0, environment_map.width, 
                                       environment_map.height), 0, 0, false); 
      readable_map.Apply();
      RenderTexture.active = null;
      RenderTexture.ReleaseTemporary(temp_rt);
      RoveInternal.SetEnvironmentMap(readable_map.GetRawTextureData(),
                                     (uint)environment_map.width,
                                     (uint)environment_map.height);
      DestroyImmediate(readable_map, true);
    } else {
      Color[] empty_color = new Color[1];
      empty_color[0].r = 1.0f;
      empty_color[0].g = 1.0f;
      empty_color[0].b = 1.0f;
      empty_color[0].a = 1.0f;
      Texture2D null_texture = new Texture2D(1, 1, TextureFormat.RGBAHalf, false);
      null_texture.SetPixels(empty_color);
      null_texture.Apply();
      RoveInternal.SetEnvironmentMap(null_texture.GetRawTextureData(), 1, 1);
      DestroyImmediate(null_texture, true);
    }
  }
  ///
  public void SetMapProperties(float intensity,
                               float rotation)
  {
    m_map_intensity = intensity;
    m_map_rotation = rotation;
    RoveInternal.SetEnvironmentProperties(intensity, rotation);
  }
}
