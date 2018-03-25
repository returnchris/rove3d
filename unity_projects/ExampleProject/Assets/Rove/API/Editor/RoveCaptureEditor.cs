using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveCapture))]
public class RoveCaptureEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private Texture2D m_bg;
  private Texture2D m_gbg;
  private SerializedProperty mp_rendering_sub_directory;
  private SerializedProperty mp_rendering_directory;
  private SerializedProperty mp_animation_framecount;
  private SerializedProperty mp_animation_current_frame;
  private SerializedProperty mp_frame_rendering;
  private SerializedProperty mp_animation_rendering;
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
    mp_rendering_sub_directory = 
      serializedObject.FindProperty("m_rendering_sub_directory");
    mp_rendering_directory = serializedObject.FindProperty("m_rendering_directory");
    mp_animation_framecount = serializedObject.FindProperty("m_animation_framecount");
    mp_animation_current_frame = 
      serializedObject.FindProperty("m_animation_current_frame");
    mp_frame_rendering = serializedObject.FindProperty("m_frame_rendering");
    mp_animation_rendering = serializedObject.FindProperty("m_animation_rendering");
    serializedObject.Update();
    serializedObject.ApplyModifiedProperties();
  }
  ///
  public override void OnInspectorGUI()
  {
    serializedObject.Update();
    RoveCapture rove_capture = (RoveCapture)target;
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
          EditorGUILayout.LabelField("Capture", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          EditorGUILayout.EndVertical();
        }
        m_dark_style.normal.background = m_bg;
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    EditorGUILayout.BeginVertical(m_dark_style);
    {
      EditorGUILayout.Space();
      if (Application.isPlaying && rove_capture.m_animation_rendering) {
        m_dark_style.alignment = TextAnchor.MiddleCenter;
        EditorGUILayout.LabelField("Animation capture in progress...",  
                                   m_dark_style);
        m_dark_style.alignment = TextAnchor.MiddleLeft;
        EditorGUILayout.Space();
        EditorGUILayout.EndVertical();
        serializedObject.ApplyModifiedProperties();
        return;
      }
      EditorGUILayout.BeginHorizontal(m_dark_style);
      {
        EditorGUILayout.LabelField("Sub-Directory: ", m_dark_style,
                                   GUILayout.MaxWidth(100.0f));
        mp_rendering_sub_directory.stringValue =
          EditorGUILayout.TextField(mp_rendering_sub_directory.stringValue);
        EditorGUILayout.EndHorizontal();
      }
      EditorGUILayout.Space();
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(20);
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Capture Frame", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(20);
          if (Application.isPlaying) {
            EditorGUILayout.BeginHorizontal(m_dark_style);
            {
              GUILayout.Space((EditorGUIUtility.currentViewWidth / 2) -
                              (100 / 2) - 17);
              if (GUILayout.Button("Capture", GUILayout.MaxWidth(100.0f))) {
                rove_capture.CaptureCurrentFrame(mp_rendering_sub_directory.stringValue);  
              }
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.Space();
          } else {
            m_dark_style.alignment = TextAnchor.MiddleCenter;
            EditorGUILayout.LabelField("Available in play mode.",  m_dark_style);
            m_dark_style.alignment = TextAnchor.MiddleLeft;
            GUILayout.Space(10);
          }
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.Space();
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(20);
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Capture Animation", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(20);
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUILayout.LabelField("Capture Framecount: ", m_dark_style,
                                       GUILayout.MaxWidth(130.0f));
            GUILayout.FlexibleSpace();
            mp_animation_framecount.intValue =
              EditorGUILayout.IntField(mp_animation_framecount.intValue);
            if (mp_animation_framecount.intValue < 1) {
              mp_animation_framecount.intValue = 1;
            }
            EditorGUILayout.EndHorizontal();
          }
          EditorGUILayout.Space();
          EditorGUILayout.Space();
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            GUILayout.Space((EditorGUIUtility.currentViewWidth / 2) -
                            (100 / 2) - 17);
            if (!Application.isPlaying) {
              if ((mp_animation_rendering.boolValue == true) &&
                  (!EditorApplication.isPlayingOrWillChangePlaymode)) {
                mp_animation_rendering.boolValue = false;
              }
              if (GUILayout.Button("Capture", GUILayout.MaxWidth(100.0f))) {
                mp_animation_current_frame.intValue = 0;
                mp_rendering_directory.stringValue = 
                  Application.dataPath + "/RoveRenders";
                if (mp_rendering_sub_directory.stringValue.Length > 0) {
                  mp_rendering_directory.stringValue += 
                    "/" + mp_rendering_sub_directory.stringValue;
                }
                Directory.CreateDirectory(mp_rendering_directory.stringValue);
                mp_frame_rendering.boolValue  = false;
                mp_animation_rendering.boolValue = true;
                serializedObject.ApplyModifiedProperties();
                EditorApplication.isPlaying = true;
              }
            } else {
              if (GUILayout.Button("Capture", GUILayout.MaxWidth(100.0f))) {
                rove_capture.CaptureNewAnimation(mp_rendering_sub_directory.stringValue,
                                                 mp_animation_framecount.intValue);
              }
            }
            EditorGUILayout.EndHorizontal();
          }
          EditorGUILayout.Space();
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.Space();
      /*
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(20);
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Capture Volumetric", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(20);
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(10);
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Coming soon...",  m_dark_style);
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(10);
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
      */
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
  }
}
