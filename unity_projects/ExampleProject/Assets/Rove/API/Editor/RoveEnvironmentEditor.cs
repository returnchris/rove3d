using System.Collections;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveEnvironment))]
public class RoveEnvironmentEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private Texture2D m_bg;
  private Texture2D m_wbg;
  private Texture2D m_gbg;
  private SerializedProperty mp_type;
  private SerializedProperty mp_sky_intensity;
  private SerializedProperty mp_sun_intensity;
  private SerializedProperty mp_sun_color;
  private SerializedProperty mp_environment_map;
  private SerializedProperty mp_map_intensity;
  private SerializedProperty mp_map_rotation;
  ///
  private static string[] m_available_environments = { "Physical Sky",
                                                       "HDR Environment Map" };
  ///
  void OnEnable()
  {
    m_logo = (Texture2D)Resources.Load("RoveLogo");
    m_dark_style = new GUIStyle();
    Color[] bgp = new Color[1];
    bgp[0].r = 0.15f;
    bgp[0].g = 0.15f;
    bgp[0].b = 0.15f;
    bgp[0].a = 1.0f;
    m_bg = new Texture2D(1, 1);
    m_bg.SetPixels(bgp);
    m_bg.wrapMode = TextureWrapMode.Repeat;
    m_bg.Apply();
    Color[] wbgp = new Color[1];
    wbgp[0].r = 0.44f;
    wbgp[0].g = 0.44f;
    wbgp[0].b = 0.44f;
    wbgp[0].a = 1.0f;
    m_wbg = new Texture2D(1, 1);
    m_wbg.SetPixels(wbgp);
    m_wbg.wrapMode = TextureWrapMode.Repeat;
    m_wbg.Apply();
    Color[] gbgp = new Color[1];
    gbgp[0].r = 0.8f;
    gbgp[0].g = 0.6f;
    gbgp[0].b = 0.18f;
    gbgp[0].a = 1.0f;
    m_gbg = new Texture2D(1, 1);
    m_gbg.SetPixels(gbgp);
    m_gbg.wrapMode = TextureWrapMode.Repeat;
    m_gbg.Apply();
    ///
    mp_type = serializedObject.FindProperty("m_type");
    mp_sky_intensity = serializedObject.FindProperty("m_sky_intensity");
    mp_sun_intensity = serializedObject.FindProperty("m_sun_intensity");
    mp_sun_color = serializedObject.FindProperty("m_sun_color");
    mp_environment_map = serializedObject.FindProperty("m_environment_map");
    mp_map_intensity = serializedObject.FindProperty("m_map_intensity");
    mp_map_rotation = serializedObject.FindProperty("m_map_rotation");
  }
  ///
  public override void OnInspectorGUI()
  {
    serializedObject.Update();
    RoveEnvironment rove_environment = (RoveEnvironment)target;
    m_dark_style.normal.textColor = Color.white;
    m_dark_style.normal.background = m_bg;
    m_dark_style.fontSize = 12;
    m_dark_style.alignment = TextAnchor.MiddleLeft;
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
          EditorGUILayout.LabelField("Environment", m_dark_style);
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
      GUILayout.Space(20);
      EditorGUILayout.BeginHorizontal(m_dark_style);
      {
        EditorGUI.BeginChangeCheck();
        {
          EditorGUILayout.LabelField("Type: ", m_dark_style);
          mp_type.intValue = EditorGUILayout.Popup(mp_type.intValue,
                                                   m_available_environments);
          if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
            serializedObject.ApplyModifiedProperties();
            serializedObject.Update();
            rove_environment.SetType((uint)mp_type.intValue);
            if ((uint)mp_type.intValue == RoveEnvironment.TYPE_MAP) {
              rove_environment.SetEnvironmentMap(
                         (Texture2D)mp_environment_map.objectReferenceValue);
              rove_environment.SetMapProperties(mp_map_intensity.floatValue,
                                                mp_map_rotation.floatValue);
            }
          }
        }
        EditorGUILayout.EndHorizontal();
      }
      GUILayout.Space(20);
      EditorGUILayout.BeginVertical("box");
      {
        if (mp_type.intValue == RoveEnvironment.TYPE_MAP) {
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            m_dark_style.normal.background = m_wbg;
            EditorGUILayout.BeginVertical(m_dark_style);
            {
              EditorGUI.BeginChangeCheck();
              {
                mp_environment_map.objectReferenceValue =
                  EditorGUILayout.ObjectField(
                    mp_environment_map.objectReferenceValue,
                    typeof(Texture2D), false, GUILayout.MaxWidth(30.0f));
                if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                  rove_environment.SetEnvironmentMap(
                             (Texture2D)mp_environment_map.objectReferenceValue);
                }
              }
              EditorGUILayout.EndVertical();
            }
            m_dark_style.normal.background = m_bg;
            EditorGUILayout.LabelField("HDR Texture (RGBAHalf)", m_dark_style,
                                       GUILayout.MaxWidth(70.0f));
            GUILayout.FlexibleSpace();
            EditorGUILayout.EndHorizontal();
          }
        }
        EditorGUILayout.BeginHorizontal(m_dark_style);
        {
          EditorGUILayout.BeginVertical(m_dark_style);
          {
            if (mp_type.intValue == RoveEnvironment.TYPE_SKY) {
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUILayout.LabelField("Sky Intensity: ", m_dark_style,
                                           GUILayout.MaxWidth(80.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUILayout.LabelField("Sun Intensity: ", m_dark_style,
                                           GUILayout.MaxWidth(80.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUILayout.LabelField("Color: ", m_dark_style,
                                           GUILayout.MaxWidth(80.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
            } else {
							EditorGUILayout.Space();
							EditorGUILayout.BeginHorizontal(m_dark_style);
							{
								EditorGUILayout.LabelField("Intensity: ", m_dark_style,
																					 GUILayout.MaxWidth(80.0f));
								GUILayout.FlexibleSpace();
								EditorGUILayout.EndHorizontal();
							}
							EditorGUILayout.Space();
							EditorGUILayout.BeginHorizontal(m_dark_style);
							{
								EditorGUILayout.LabelField("Rotation: ", m_dark_style,
																					 GUILayout.MaxWidth(80.0f));
								GUILayout.FlexibleSpace();
								EditorGUILayout.EndHorizontal();
							}
							EditorGUILayout.Space();
            }
            EditorGUILayout.EndVertical();
          }
          ///
          EditorGUILayout.BeginVertical(m_dark_style);
          {
            if (mp_type.intValue == RoveEnvironment.TYPE_SKY) {
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_sky_intensity.floatValue = 
                    EditorGUILayout.Slider(mp_sky_intensity.floatValue, 0.0f, 10.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_environment.SetSkyIntensity(mp_sky_intensity.floatValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_sun_intensity.floatValue = 
                    EditorGUILayout.Slider(mp_sun_intensity.floatValue, 0.0f, 2.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_environment.SetSunIntensity(mp_sun_intensity.floatValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_sun_color.colorValue = 
                    EditorGUILayout.ColorField(mp_sun_color.colorValue,
                                               GUILayout.MaxWidth(50.0f));
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_environment.SetSunColor(mp_sun_color.colorValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
            } else {
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_map_intensity.floatValue = 
                    EditorGUILayout.Slider(mp_map_intensity.floatValue, 0.0f, 10.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_environment.SetMapProperties(mp_map_intensity.floatValue,
                                                      mp_map_rotation.floatValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_map_rotation.floatValue = 
                    EditorGUILayout.Slider(mp_map_rotation.floatValue, -1.0f, 1.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_environment.SetMapProperties(mp_map_intensity.floatValue,
                                                      mp_map_rotation.floatValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
							EditorGUILayout.Space();
						}
            EditorGUILayout.EndVertical();
          }
          EditorGUILayout.EndHorizontal();
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
  }
}
