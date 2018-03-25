using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveSetup))]
public class RoveSetupEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private Texture2D m_bg;
  private Texture2D m_gbg;
  private SerializedProperty mp_target_downsample;
  private SerializedProperty mp_force_opencl;
  private SerializedProperty mp_use_integrated_device;
  private SerializedProperty mp_samples_per_frame;
  private SerializedProperty mp_max_bounces;
  private SerializedProperty mp_optimization_level;
  private static string[] m_available_optimizations = { "Speed (< 1M tris)",
                                                        "Balanced",
                                                        "Memory (> 1M tris)" };
  ///
  void OnEnable()
  {
    m_logo = (Texture2D)Resources.Load("RoveLogo");
    Color[] bgp = new Color[1];
    bgp[0].r = 0.15f;
    bgp[0].g = 0.15f;
    bgp[0].b = 0.15f;
    bgp[0].a = 1.0f;
    m_bg = new Texture2D(1, 1);
    m_bg.SetPixels(bgp);
    m_bg.wrapMode = TextureWrapMode.Repeat;
    m_bg.Apply();
    Color[] gbgp = new Color[1];
    gbgp[0].r = 0.8f;
    gbgp[0].g = 0.6f;
    gbgp[0].b = 0.18f;
    gbgp[0].a = 1.0f;
    m_gbg = new Texture2D(1, 1);
    m_gbg.SetPixels(gbgp);
    m_gbg.wrapMode = TextureWrapMode.Repeat;
    m_gbg.Apply();
    m_dark_style = new GUIStyle();
    m_dark_style.normal.textColor = Color.white;
    m_dark_style.normal.background = m_bg;
    m_dark_style.fontSize = 12;
    m_dark_style.alignment = TextAnchor.MiddleLeft;
    mp_target_downsample = serializedObject.FindProperty("m_target_downsample");
    mp_force_opencl = serializedObject.FindProperty("m_force_opencl");
    mp_use_integrated_device = serializedObject.FindProperty("m_use_integrated_device");
    mp_samples_per_frame = serializedObject.FindProperty("m_samples_per_frame");
    mp_max_bounces = serializedObject.FindProperty("m_max_bounces");
    mp_optimization_level = serializedObject.FindProperty("m_optimization_level");
    serializedObject.Update();
    serializedObject.ApplyModifiedProperties();
  }
  ///
  public override void OnInspectorGUI()
  {
    serializedObject.Update();
    RoveSetup rove_setup = (RoveSetup)target;
    EditorGUILayout.BeginVertical("box");
    {
      EditorGUILayout.BeginVertical(m_dark_style);
      {
        GUILayout.Space(10);
        EditorGUILayout.BeginHorizontal();
        {
          int logo_width = 256;
          int logo_height = 64;
          GUILayout.Space((EditorGUIUtility.currentViewWidth / 2) -
                          (logo_width / 2) - 10);
          EditorGUILayout.LabelField(new GUIContent(m_logo),
                                     GUILayout.Width(logo_width),
                                     GUILayout.Height(logo_height));
          EditorGUILayout.EndHorizontal();
        }
        GUILayout.Space(10);
        m_dark_style.normal.background = m_gbg;
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Setup", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          EditorGUILayout.EndVertical();
        }
        m_dark_style.normal.background = m_bg;
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    EditorGUILayout.BeginHorizontal(m_dark_style);
    {
      GUILayout.Space(15);
      if (!Application.isPlaying && GUILayout.Button("Setup Scene")) {
        MeshRenderer[] found_renderers =
          FindObjectsOfType(typeof(MeshRenderer)) as MeshRenderer[];
        for (uint r = 0; r < found_renderers.Length; ++r) {
          GameObject o = found_renderers[r].gameObject;
          if (!o.GetComponent(typeof(RoveObject))) {
            o.AddComponent(typeof(RoveObject));
          }
        }
      } 
      if (GUILayout.Button("Enable Rove")) {
        RoveSetup.EnableRove();
      } 
      if (GUILayout.Button("Disable Rove")) {
        RoveSetup.DisableRove();
      } 
      GUILayout.Space(15);
      EditorGUILayout.EndHorizontal();
    }
    EditorGUILayout.BeginVertical("box");
    {
      EditorGUILayout.BeginVertical(m_dark_style);
      {
        EditorGUILayout.Space();
        EditorGUILayout.Space();
        if (!Application.isPlaying) {
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUILayout.LabelField("Optimization Level: ", m_dark_style,
                                       GUILayout.MaxWidth(150.0f));
            GUILayout.FlexibleSpace();
            mp_optimization_level.intValue = 
              EditorGUILayout.Popup(mp_optimization_level.intValue, 
                                    m_available_optimizations);
            EditorGUILayout.EndHorizontal();
          }
          EditorGUILayout.Space();
        }
        EditorGUILayout.BeginHorizontal(m_dark_style);
        {
          EditorGUILayout.LabelField("Downsample: ", m_dark_style,
                                     GUILayout.MaxWidth(150.0f));
          GUILayout.FlexibleSpace();
          EditorGUI.BeginChangeCheck();
          {
            mp_target_downsample.intValue =
              EditorGUILayout.IntField(mp_target_downsample.intValue);
            if (mp_target_downsample.intValue < 0) {
              mp_target_downsample.intValue = 0;
            }
            if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
              serializedObject.ApplyModifiedProperties();
              rove_setup.SetTargetDownsample(mp_target_downsample.intValue);
            }
          }
          EditorGUILayout.EndHorizontal();
        }
        EditorGUILayout.Space();
        EditorGUILayout.BeginHorizontal(m_dark_style);
        {
          EditorGUILayout.LabelField("Max bounces: ", m_dark_style,
                                     GUILayout.MaxWidth(150.0f));
          GUILayout.FlexibleSpace();
          EditorGUI.BeginChangeCheck();
          {
            mp_max_bounces.intValue =
              EditorGUILayout.IntField(mp_max_bounces.intValue);
            if (mp_max_bounces.intValue <= 0) {
              mp_max_bounces.intValue = 1;
            }
            if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
              rove_setup.SetMaxBounces(mp_max_bounces.intValue);
            }
          }
          EditorGUILayout.EndHorizontal();
        }
        EditorGUILayout.Space();
        EditorGUILayout.BeginHorizontal(m_dark_style);
        {
          EditorGUILayout.LabelField("SPP/Frame: ", m_dark_style,
                                     GUILayout.MaxWidth(150.0f));
          GUILayout.FlexibleSpace();
          EditorGUI.BeginChangeCheck();
          {
            mp_samples_per_frame.intValue =
              EditorGUILayout.IntField(mp_samples_per_frame.intValue);
            if (mp_samples_per_frame.intValue <= 0) {
              mp_samples_per_frame.intValue = 1;
            }
            if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
              rove_setup.SetSamplesPerFrame(mp_samples_per_frame.intValue);
            }
          }
          EditorGUILayout.EndHorizontal();
        }
        EditorGUILayout.Space();
        if (!Application.isPlaying) {
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUILayout.LabelField("Force OpenCL: ", m_dark_style,
                                       GUILayout.MaxWidth(150.0f));
            GUILayout.FlexibleSpace();
            mp_force_opencl.boolValue =
              EditorGUILayout.Toggle(mp_force_opencl.boolValue);
            EditorGUILayout.EndHorizontal();
          }
        }
        EditorGUILayout.Space();
        if (!Application.isPlaying) {
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUILayout.LabelField("Allow iGPU: ", m_dark_style,
                                       GUILayout.MaxWidth(150.0f));
            GUILayout.FlexibleSpace();
            mp_use_integrated_device.boolValue =
              EditorGUILayout.Toggle(mp_use_integrated_device.boolValue);
            EditorGUILayout.EndHorizontal();
          }
        }
        EditorGUILayout.Space();
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    if (Application.isPlaying) {
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(15);
          m_dark_style.fontSize = 16;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("System Info", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(15); 
          EditorGUILayout.LabelField("CPU threads: " + 
                                     rove_setup.m_system_info.m_thread_count +
                                     ".",
                                     m_dark_style);
          if (rove_setup.m_system_info.m_compute_api == RoveSetup.CL) {
            EditorGUILayout.LabelField("Compute API: OpenCL.", m_dark_style);
          } else if (rove_setup.m_system_info.m_compute_api == RoveSetup.CUDA) {
            EditorGUILayout.LabelField("Compute API: CUDA.", m_dark_style);
          }
          EditorGUILayout.LabelField("Active device count: " + 
                               rove_setup.m_system_info.m_device_count + ".",
                               m_dark_style);
          EditorGUILayout.LabelField("---", m_dark_style);
          for (uint d = 0; d < rove_setup.m_system_info.m_device_count; ++d) {
            EditorGUILayout.LabelField(
              d + ": " + rove_setup.m_system_info.m_device_names[d], 
              m_dark_style);
          }
          GUILayout.Space(15);
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
    }
    serializedObject.ApplyModifiedProperties();
  }
}

