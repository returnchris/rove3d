using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveCamera))]
public class RoveCameraEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private Texture2D m_bg;
  private Texture2D m_gbg;
  private SerializedProperty mp_field_of_view;
  private SerializedProperty mp_aperture_size;
  private SerializedProperty mp_focal_depth;
  private SerializedProperty mp_tonemap;
  private SerializedProperty mp_gamma;
  private SerializedProperty mp_exposure;
  private SerializedProperty mp_contrast;
  private SerializedProperty mp_saturation;
  private SerializedProperty mp_brightness;
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
    mp_field_of_view = serializedObject.FindProperty("m_field_of_view");
    mp_aperture_size = serializedObject.FindProperty("m_aperture_size");
    mp_focal_depth = serializedObject.FindProperty("m_focal_depth");
    mp_tonemap = serializedObject.FindProperty("m_tonemap");
    mp_gamma = serializedObject.FindProperty("m_gamma");
    mp_exposure = serializedObject.FindProperty("m_exposure");
    mp_contrast = serializedObject.FindProperty("m_contrast");
    mp_saturation = serializedObject.FindProperty("m_saturation");
    mp_brightness = serializedObject.FindProperty("m_brightness");
    serializedObject.Update();
    serializedObject.ApplyModifiedProperties();
  }
  ///
  public override void OnInspectorGUI()
  {
    serializedObject.Update();
    RoveCamera rove_camera = (RoveCamera)target;
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
          EditorGUILayout.LabelField("Camera", m_dark_style);
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
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(20);
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Lens", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(20);
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUI.BeginChangeCheck();
            {
              EditorGUILayout.LabelField("Field Of View: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_field_of_view.floatValue =
                EditorGUILayout.Slider(mp_field_of_view.floatValue, 1.0f, 360.0f);
              if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                rove_camera.SetFieldOfView(mp_field_of_view.floatValue);
              }
            }
            EditorGUILayout.EndHorizontal();
          }
          EditorGUILayout.BeginHorizontal();
          {
            EditorGUI.BeginChangeCheck();
            {
              EditorGUILayout.LabelField("Aperture Size: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_aperture_size.floatValue =
                  EditorGUILayout.Slider(mp_aperture_size.floatValue, 0.0f, 1.0f);
              if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                rove_camera.SetCameraApertureSize(mp_aperture_size.floatValue);
              }
            }
            EditorGUILayout.EndHorizontal();
          }
          EditorGUILayout.BeginHorizontal();
          {
            EditorGUI.BeginChangeCheck();
            {
              EditorGUILayout.LabelField("Focal Depth: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_focal_depth.floatValue =
                EditorGUILayout.Slider(mp_focal_depth.floatValue, 0.1f, 500.0f);
              if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                rove_camera.SetCameraFocalDepth(mp_focal_depth.floatValue);
              }
            }
            EditorGUILayout.EndHorizontal();
          }
          GUILayout.Space(10);
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    EditorGUILayout.BeginVertical(m_dark_style);
    {
      EditorGUILayout.BeginVertical("box");
      {
        EditorGUILayout.BeginVertical(m_dark_style);
        {
          GUILayout.Space(20);
          m_dark_style.fontSize = 17;
          m_dark_style.alignment = TextAnchor.MiddleCenter;
          EditorGUILayout.LabelField("Image Adjustment", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          GUILayout.Space(20);
          EditorGUI.BeginChangeCheck();
          {
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Tonemapping: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              string[] tonemaps = { "None", "Basic" };
              mp_tonemap.intValue =
                EditorGUILayout.Popup(mp_tonemap.intValue, tonemaps);
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Gamma: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_gamma.floatValue =
                EditorGUILayout.Slider(mp_gamma.floatValue, 0.0f, 5.0f);
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Exposure: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_exposure.floatValue =
                EditorGUILayout.Slider(mp_exposure.floatValue, 0.0f, 10.0f);
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Contrast: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_contrast.floatValue =
                EditorGUILayout.Slider(mp_contrast.floatValue, 0.0f, 2.0f);
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Saturation: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_saturation.floatValue =
                EditorGUILayout.Slider(mp_saturation.floatValue, 0.0f, 2.0f);
              EditorGUILayout.EndHorizontal();
            }
            EditorGUILayout.BeginHorizontal();
            {
              EditorGUILayout.LabelField("Brightness: ", m_dark_style,
                                         GUILayout.MaxWidth(100.0f));
              mp_brightness.floatValue =
                EditorGUILayout.Slider(mp_brightness.floatValue, 0.0f, 2.0f);
              EditorGUILayout.EndHorizontal();
            }
            if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
              rove_camera.SetImageProperties(mp_tonemap.boolValue,
                                             mp_gamma.floatValue,
                                             mp_exposure.floatValue,
                                             mp_contrast.floatValue,
                                             mp_saturation.floatValue,
                                             mp_brightness.floatValue);
            }
          }
          GUILayout.Space(10);
          EditorGUILayout.EndVertical();
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
  }
}
