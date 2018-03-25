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
using RoveStatus = System.UInt32;
using RoveLock = System.UInt32;
using RoveComputeAPI = System.UInt32;
///
public class RoveSetup : MonoBehaviour 
{
  public static uint ERROR     = 0xFFFFFFFF;
  public static uint SUCCESS   = 0x1;
  ///
  public static uint UNLOCKED  = 0x0;
  public static uint LOCKED    = 0x1;
  ///
  public static uint CL        = 0x0;
  public static uint CUDA      = 0x1;
  ///
  public static uint OPTIMIZATION_SPEED    = 0x0;
  public static uint OPTIMIZATION_BALANCED = 0x1;
  public static uint OPTIMIZATION_MEMORY   = 0x2;
  //
  public struct SystemInfo {
    public uint m_thread_count;
    public uint m_compute_api;
    public uint m_device_count;
    public string[] m_device_names;
	}
  ///
  public SystemInfo m_system_info;
  public bool m_force_opencl;
  public bool m_setup_success;
  public uint m_optimization_level;
  public bool m_use_multi_device;
  public bool m_use_integrated_device;
  public int m_samples_per_frame;
  public int m_max_bounces;
  public Material m_target_material;
  public Texture2D m_target;
  public int m_target_width;
  public int m_target_height;
  public int m_target_downsample;
  public Vector2 m_last_screen_size; 
  public bool m_pending_resize_success;
  private RoveCamera m_rove_camera;
  private RoveEnvironment m_rove_environment;
  ///
  public static T GetAPIComponent<T>() where T : class
  {
    T[] found_components = FindObjectsOfType(typeof(T)) as T[];
    if (found_components.Length == 0 || found_components.Length > 1) {
      Debug.LogError("Rove: There must exactly one " + typeof(T) + 
                     " component in scene.");
      return null;
    }
    return found_components[0];
  }
  ///
  IEnumerator SetupRove()
  {
    yield return new WaitForEndOfFrame();
    m_last_screen_size = new Vector2(Screen.width, Screen.height);
    string data_path = Application.dataPath + "/Rove/Data";
    string log_path = data_path + "/rove.log";
    bool valid_log_file = false;
    try {
      File.WriteAllText(log_path, "Test write to log.");
      valid_log_file = true;
    } catch {
      valid_log_file = false;
    }
    if (!valid_log_file) {
      Debug.LogError("Rove: Unable to write to log file at: " + log_path + ".");
      Destroy(this);
      yield return null;
    }
    m_target_width = Screen.width >> m_target_downsample;
    m_target_height = Screen.height >> m_target_downsample;
    #if (UNITY_EDITOR)
      if (PlayerSettings.colorSpace != ColorSpace.Gamma) {
        Debug.LogWarning("Rove: Gamma color space is recommended.");
      }
    #endif
    m_target = new Texture2D(m_target_width, m_target_height, 
                             TextureFormat.ARGB32, false, true);
    m_target.filterMode = FilterMode.Bilinear;
    IntPtr native_tex = m_target.GetNativeTexturePtr();
    m_target_material = (Material)Resources.Load("RoveTargetMaterial");
    m_target_material.SetTexture("target", m_target);
    #if ((!ROVE_FORCE_GL) && (UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN))
      IntPtr d3d11_device = RoveInternal.RoveUnityGetD3D11Device();
      if (d3d11_device == IntPtr.Zero) {
        Debug.LogError("Failed to get D3D11 device.");
        Destroy(this);
        yield return null;
      }
      RoveInternal.SetD3D11Device(d3d11_device);
    #endif
    GL.IssuePluginEvent(RoveInternal.Setup(data_path, m_use_multi_device,
                                           m_use_integrated_device,
                                           m_optimization_level,
                                           native_tex), 0);
    yield return new WaitForEndOfFrame();
    if (RoveInternal.GetLock() == LOCKED) {
      Debug.LogError("Rove: Failed setup; check 'Rove/Data/rove.log' " + 
                     "for details.");
      Destroy(this);
      yield return null;
    } else {
      // Additional resize fixes bug on some Linux drivers.
      m_target.Resize(m_target_width, m_target_height);
      m_target.Apply();
      native_tex = m_target.GetNativeTexturePtr();
      GL.IssuePluginEvent(RoveInternal.Resize(native_tex), 0);
      yield return new WaitForEndOfFrame();
      if (RoveInternal.GetLock() == LOCKED) {
        Debug.LogError("Rove: Failed to resize; check 'Rove/Data/rove.log' " +
                       "for details.");
        Destroy(this);
        yield return null;
      } 
      Color[] black_pixels = new Color[m_target_width * m_target_height];
      for (uint p = 0; p < m_target_width * m_target_height; ++p) {
        black_pixels[p] = Color.black;
      }
      m_target.SetPixels(black_pixels);
      m_target.Apply();
      m_system_info.m_thread_count = RoveInternal.GetThreadCount();
      m_system_info.m_compute_api = RoveInternal.GetComputeAPI();
      m_system_info.m_device_count = RoveInternal.GetDeviceCount();
      m_system_info.m_device_names = new string[m_system_info.m_device_count];
      for (uint d = 0; d < m_system_info.m_device_count; ++d) {
        m_system_info.m_device_names[d] =
          Marshal.PtrToStringAnsi(RoveInternal.GetDeviceName(d));
      }
    }
    m_rove_camera = GetAPIComponent<RoveCamera>();
    if (!m_rove_camera) {
      Debug.LogError("Rove: Could not find an active RoveCamera component.");
      Destroy(this);
      yield return null;
    }
    m_rove_environment = GetAPIComponent<RoveEnvironment>();
    if (!m_rove_environment) {
      Debug.LogError("Rove: Could not find an active RoveEnvironment component.");
      Destroy(this);
      yield return null;
    }
    SetMaxBounces(m_max_bounces);
    SetSamplesPerFrame(m_samples_per_frame);
    m_rove_camera.SetCameraApertureSize(m_rove_camera.m_aperture_size);
    m_rove_camera.SetCameraFocalDepth(m_rove_camera.m_focal_depth);
    m_rove_camera.SetImageProperties(m_rove_camera.m_tonemap, 
                                     m_rove_camera.m_gamma,
                                     m_rove_camera.m_exposure, 
                                     m_rove_camera.m_contrast,
                                     m_rove_camera.m_saturation, 
                                     m_rove_camera.m_brightness);
    m_rove_environment.SetType(m_rove_environment.m_type);
    if (m_rove_environment.m_type == RoveEnvironment.TYPE_MAP) {
      m_rove_environment.SetEnvironmentMap(m_rove_environment.m_environment_map);
      m_rove_environment.SetMapProperties(m_rove_environment.m_map_intensity,
                                          m_rove_environment.m_map_rotation);
    }
    m_setup_success = true;
  }
  ///
  void Start() 
  {
    QualitySettings.vSyncCount = 0;
    m_setup_success = false;
		m_pending_resize_success = false;
    if (IntPtr.Size != 8) {
      Debug.LogError("Rove: Rove3D currently requires 64-bit editor/build.");
      return;
    }
    if (!RoveInternal.LoadPlugin(m_force_opencl)) {
      Debug.LogError("Rove: Failed to load plugin.");
      return;
    }
    StartCoroutine(SetupRove());
  }
  ///
  void OnDestroy() 
  {
    m_setup_success = false;
    RoveInternal.Shutdown();
    RoveInternal.UnloadPlugin();
  }
  /// API functions.
  ///
  public static void EnableRove()
  {
    GetAPIComponent<RoveSetup>().enabled = true;
    GetAPIComponent<RoveCamera>().enabled = true;
    GetAPIComponent<RoveEnvironment>().enabled = true;
    GetAPIComponent<RoveCapture>().enabled = true;
    GetAPIComponent<RoveMaterials>().enabled = true;
    RoveObject[] rove_objects = FindObjectsOfType(typeof(RoveObject)) as  
                                RoveObject[];
    foreach(RoveObject o in rove_objects) {
      if (Application.isPlaying) {
        MeshRenderer mr = o.gameObject.GetComponent<MeshRenderer>();
        mr.enabled = false;
      }
      o.enabled = true;
    }
  }
  ///
  public static void DisableRove()
  {
    GetAPIComponent<RoveSetup>().enabled = false;
    GetAPIComponent<RoveCamera>().enabled = false;
    GetAPIComponent<RoveEnvironment>().enabled = false;
    GetAPIComponent<RoveCapture>().enabled = false;
    GetAPIComponent<RoveMaterials>().enabled = false;
    RoveObject[] rove_objects = FindObjectsOfType(typeof(RoveObject)) as  
                                RoveObject[];
    foreach(RoveObject o in rove_objects) {
      MeshRenderer mr = o.gameObject.GetComponent<MeshRenderer>();
      mr.enabled = true;
      o.enabled = false;
    }
  }
  ///
  public void ResizeTarget(int width,
                           int height)
  {
    m_setup_success = false;
    m_pending_resize_success = true;
    m_last_screen_size.x = Screen.width;
    m_last_screen_size.y = Screen.height;
    m_target_width = width >> m_target_downsample;
    m_target_height = height >> m_target_downsample;
		m_target.Resize(m_target_width, m_target_height);
    m_target.Apply();
    IntPtr native_tex = m_target.GetNativeTexturePtr();
    GL.IssuePluginEvent(RoveInternal.Resize(native_tex), 0);
  }
  ///
  public void SetTargetDownsample(int downsample)
  {
    m_target_downsample = downsample;
    m_last_screen_size = new Vector2(0.0f, 0.0f);
  }
  ///
  public void SetMaxBounces(int max_bounces)
  {
    m_max_bounces = max_bounces;
    RoveInternal.SetMaxBounces((uint)m_max_bounces);
  }
  ///
  public void SetSamplesPerFrame(int samples_per_frame)
  {
    m_samples_per_frame = samples_per_frame;
    RoveInternal.SetSamplesPerFrame((uint)m_samples_per_frame);
  }
}
