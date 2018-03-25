using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveMaterials))]
public class RoveMaterialsEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private GUIStyle m_button_style;
  private Texture2D m_bg;
  private Texture2D m_gbg;
  private bool[] m_expand_atlas;
  private bool[] m_expand_material;
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
    m_dark_style.wordWrap = true;
    RoveMaterials rove_materials = (RoveMaterials)target;
    m_expand_atlas = new bool[RoveMaterials.ATLAS_COUNT];
    m_expand_material = new bool[rove_materials.m_materials.Count];
    serializedObject.Update();
    serializedObject.ApplyModifiedProperties();
  }
  ///
  public override void OnInspectorGUI()
  {
    if (m_button_style == null) {
      m_button_style = new GUIStyle(GUI.skin.button);
      m_button_style.fontSize = 13;
      m_button_style.alignment = TextAnchor.MiddleLeft;
      m_button_style.normal.background = m_gbg;
      m_button_style.normal.textColor = Color.white;
      m_button_style.active.background = m_gbg;
      m_button_style.active.textColor = Color.white;
    }
    serializedObject.Update();
    RoveMaterials rove_materials = (RoveMaterials)target;
    if (rove_materials.m_materials.Count != m_expand_material.Length) {
      m_expand_material = new bool[rove_materials.m_materials.Count];
    }
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
          EditorGUILayout.LabelField("Materials", m_dark_style);
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
      m_dark_style.alignment = TextAnchor.MiddleCenter;
      if (!Application.isPlaying) {
        GUILayout.Space(10);
        EditorGUILayout.BeginHorizontal(m_dark_style);
        {
          GUILayout.Space(10);
          EditorGUILayout.LabelField("Scene-wide material information will " +
                                     "display during play mode.", m_dark_style);
          GUILayout.Space(10);
          EditorGUILayout.EndHorizontal();
        }
        GUILayout.Space(10);
      } else {
        GUILayout.Space(20);
        m_dark_style.fontSize = 17;
        EditorGUILayout.LabelField("Global List", m_dark_style);
        m_dark_style.fontSize = 15;
        GUILayout.Space(20);
        for (int m = 0; m < rove_materials.m_materials.Count; ++m) {
          EditorGUILayout.BeginVertical("box");
          {
            EditorGUILayout.BeginVertical(m_dark_style);
            {
              m_dark_style.normal.background = m_gbg;
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                string material_name = Path.GetFileNameWithoutExtension(
                        AssetDatabase.GetAssetPath(rove_materials.m_materials[m]));
                if (GUILayout.Button(material_name, m_button_style)) { 
                  m_expand_material[m] = !m_expand_material[m];
                }
                EditorGUILayout.EndHorizontal();
              }
              m_dark_style.normal.background = m_bg;
              if (m_expand_material[m]) {
                RoveMaterialEditor material_editor = 
                  (RoveMaterialEditor)RoveMaterialEditor.CreateEditor(
                                 rove_materials.m_materials[m], 
                                 typeof(RoveMaterialEditor));
                material_editor.InspectorProperties(false, 0);
              }
              EditorGUILayout.EndVertical();
            }
            EditorGUILayout.EndVertical();
          }
        }
        GUILayout.Space(20);
        m_dark_style.fontSize = 17;
        EditorGUILayout.LabelField("Texture Atlas", m_dark_style);
        m_dark_style.fontSize = 15;
        GUILayout.Space(20);
        for (int a = 0; a < RoveMaterials.ATLAS_COUNT; ++a) {
          EditorGUILayout.BeginVertical("box");
          {
            EditorGUILayout.BeginVertical(m_dark_style);
            {
              string atlas_type;
              if (a == RoveAtlas.TYPE_ALBEDO) {
                atlas_type = "Albedo";
              } else if (a == RoveAtlas.TYPE_NORMAL) {
                atlas_type = "Normal";
              } else if (a == RoveAtlas.TYPE_METALLIC) {
                atlas_type = "Metallic";
              } else if (a == RoveAtlas.TYPE_EMISSION) {
                atlas_type = "Emissive";
              } else {
                atlas_type = "Undefined";
              }
              RoveAtlas atlas = rove_materials.m_atlases[a];
              m_dark_style.normal.background = m_gbg;
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                m_button_style.alignment = TextAnchor.MiddleCenter;
                if (GUILayout.Button(atlas_type, m_button_style)) { 
                  m_expand_atlas[a] = !m_expand_atlas[a]; 
                }
                m_button_style.alignment = TextAnchor.MiddleLeft;
                EditorGUILayout.EndHorizontal();
              }
              m_dark_style.normal.background = m_bg;
              if (m_expand_atlas[a]) {
                GUILayout.Space(20);
                EditorGUILayout.BeginHorizontal(m_dark_style);
                {
                  GUILayout.Space((EditorGUIUtility.currentViewWidth / 2) -
                                  (128 / 2) - 25);
                  EditorGUILayout.LabelField(new GUIContent(atlas.m_texture), 
                                             m_dark_style,
                                             GUILayout.Width(128),
                                             GUILayout.Height(128));
                  EditorGUILayout.EndHorizontal();  
                }
                GUILayout.Space(20);
                EditorGUILayout.LabelField(atlas.m_width + " x " + atlas.m_height,
                                           m_dark_style);
                GUILayout.Space(20);
              }
              EditorGUILayout.EndVertical();
            }
            EditorGUILayout.EndVertical();
          }
        }
        m_dark_style.fontSize = 12;
        m_dark_style.alignment = TextAnchor.MiddleLeft;
      }
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
  }
}
