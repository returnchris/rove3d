using UnityEngine;
using UnityEngine.Rendering;
using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections;
using System.Collections.Generic;
#if UNITY_EDITOR
  using UnityEditor;
#endif
///
public class RoveCamera : MonoBehaviour 
{
  ///
  public float m_field_of_view;
  public float m_aperture_size; 
  public float m_focal_depth;
  public bool m_tonemap;
  public float m_gamma;
  public float m_contrast;
  public float m_saturation;
  public float m_brightness;
  public float m_exposure;
  private RoveSetup m_rove_setup;
  private RoveMaterials m_rove_materials;
  ///
  void Start()
  {
    m_rove_setup = RoveSetup.GetAPIComponent<RoveSetup>();
    if (!m_rove_setup || (!m_rove_setup.enabled)) {
      Debug.LogError("Rove: Could not find an active RoveSetup component.");
      Destroy(this);
      return;
    }
    m_rove_materials = RoveSetup.GetAPIComponent<RoveMaterials>();
    if (!m_rove_materials || (!m_rove_materials.enabled)) {
      Debug.LogError("Rove: Could not find an active RoveMaterials component.");
      Destroy(this);
      return;
    }
  }
  ///
  void Update()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (gameObject.transform.hasChanged) {
      SetCameraTransform(gameObject.transform);
      gameObject.transform.hasChanged = false;
    }
  }
  ///
  void OnPreRender()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if ((Screen.width != m_rove_setup.m_last_screen_size.x) || 
        (Screen.height != m_rove_setup.m_last_screen_size.y)) {
      m_rove_setup.ResizeTarget(Screen.width, Screen.height);
    } else if (!m_rove_setup.m_pending_resize_success) {
      if (!m_rove_materials.m_atlases[RoveAtlas.TYPE_ALBEDO].ProcessAtlas() ||
          !m_rove_materials.m_atlases[RoveAtlas.TYPE_NORMAL].ProcessAtlas() ||
          !m_rove_materials.m_atlases[RoveAtlas.TYPE_METALLIC].ProcessAtlas() ||
          !m_rove_materials.m_atlases[RoveAtlas.TYPE_EMISSION].ProcessAtlas()) {
        Debug.LogError("Rove: Failed to build texture atlases.");
        m_rove_setup.m_setup_success = false;
        Destroy(m_rove_setup);
        return;
      }
      if (RoveInternal.StartUpdate() != RoveSetup.SUCCESS) {
        Debug.LogError("Rove: StartUpdate() failed; " +
                       "check 'Rove/Data/rove.log' for details.");
        m_rove_setup.m_setup_success = false;
        Destroy(m_rove_setup);
        return;
      }
      GL.IssuePluginEvent(RoveInternal.FinishUpdate(), 0);
    }
  }
  ///
  void OnRenderImage(RenderTexture source, 
                     RenderTexture destination) 
  {
    if (!m_rove_setup.m_setup_success) {
      RenderTexture temp_rt = RenderTexture.GetTemporary(Screen.width, Screen.height,
                                                         0,
                                                         RenderTextureFormat.ARGB32);
      RenderTexture.active = temp_rt;
      GL.Clear(false, true, new Color(0.0f, 0.0f, 0.0f, 0.0f));
      Graphics.Blit(temp_rt, destination);
      RenderTexture.active = null;
      RenderTexture.ReleaseTemporary(temp_rt);
      if (m_rove_setup.m_pending_resize_success) {
        if (RoveInternal.GetLock() == RoveSetup.UNLOCKED) {
          m_rove_setup.m_setup_success = true;
          m_rove_setup.m_pending_resize_success = false;
        }
      }
    } else {
      Graphics.Blit(source, destination, m_rove_setup.m_target_material);
    }
  }
  /// API functions.
  ///
  public void SetCameraTransform(Transform new_transform)
  {
    gameObject.transform.position = new_transform.position;
    gameObject.transform.rotation = new_transform.rotation;
    float[] camera_transform = new float[16];
    Matrix4x4 camera_mat = new_transform.localToWorldMatrix;
    for (int e = 0; e < 16; ++e) {
      camera_transform[e] = camera_mat[e];
    }
    RoveInternal.SetCameraTransform(camera_transform);
  }
  ///
  public void SetFieldOfView(float field_of_view)
  {
    m_field_of_view = field_of_view;
    RoveInternal.SetFieldOfView(field_of_view);
  }
  ///
  public void SetCameraApertureSize(float aperture_size)
  {
    m_aperture_size = aperture_size;
    RoveInternal.SetCameraApertureSize(aperture_size);
  }
  ///
  public void SetCameraFocalDepth(float focal_depth)
  {
    m_focal_depth = focal_depth;
    RoveInternal.SetCameraFocalDepth(focal_depth);
  }
  ///
  public void SetImageProperties(bool tonemap,
                                 float gamma,
                                 float exposure,
                                 float contrast,
                                 float saturation,
                                 float brightness)
  {
    m_tonemap = tonemap;
    m_gamma = gamma;
    m_exposure = exposure;
    m_contrast = contrast;
    m_saturation = saturation;
    m_brightness = brightness;
    RoveInternal.SetImageProperties(tonemap, gamma, exposure, contrast, saturation, 
                                    brightness);
  }
}
