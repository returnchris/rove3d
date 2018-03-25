using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveMaterial))]
public class RoveMaterialEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private GUIStyle m_button_style;
  private Texture2D m_bg;
  private Texture2D m_wbg;
  private Texture2D m_gbg;
  private string m_material_name;
  private SerializedProperty mp_folded_out;
  private SerializedProperty mp_double_sided;
  private SerializedProperty mp_albedo;
  private SerializedProperty mp_metallic;
  private SerializedProperty mp_smoothness;
  private SerializedProperty mp_glass;
  private SerializedProperty mp_emission;
  private SerializedProperty mp_albedo_texture;
  private SerializedProperty mp_normal_texture;
  private SerializedProperty mp_metallic_texture;
  private SerializedProperty mp_emission_texture;
  private SerializedProperty mp_albedo_texture2d;
  private SerializedProperty mp_normal_texture2d;
  private SerializedProperty mp_metallic_texture2d;
  private SerializedProperty mp_emission_texture2d;
  private static string[] m_available_types = { "Opaque", "Glass" };
  private static string[] m_available_sources = { "(R: Metallic) (A: Smoothness)" };
  private static string[] m_available_glass_sources = { "(A: Smoothness)" };
  private static string[] m_available_tangents = { "Mikk Tangent Space" };
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
    m_dark_style = new GUIStyle();
    m_dark_style.normal.textColor = Color.white;
    m_dark_style.normal.background = m_bg;
    m_dark_style.fontSize = 12;
    m_dark_style.alignment = TextAnchor.MiddleLeft;
    RoveMaterial rove_material = (RoveMaterial)target;
    m_material_name = Path.GetFileNameWithoutExtension(
                        AssetDatabase.GetAssetPath(rove_material));
    mp_folded_out = serializedObject.FindProperty("m_folded_out");
    mp_double_sided = serializedObject.FindProperty("m_double_sided");
    mp_albedo = serializedObject.FindProperty("m_albedo");
    mp_metallic = serializedObject.FindProperty("m_metallic");
    mp_smoothness = serializedObject.FindProperty("m_smoothness");
		mp_glass = serializedObject.FindProperty("m_glass");
		mp_emission = serializedObject.FindProperty("m_emission");
    SerializedProperty textures = serializedObject.FindProperty("m_textures");
    mp_albedo_texture = textures.GetArrayElementAtIndex((int)RoveAtlas.TYPE_ALBEDO); 
    mp_normal_texture = textures.GetArrayElementAtIndex((int)RoveAtlas.TYPE_NORMAL); 
    mp_metallic_texture = textures.GetArrayElementAtIndex((int)RoveAtlas.TYPE_METALLIC); 
    mp_emission_texture = textures.GetArrayElementAtIndex((int)RoveAtlas.TYPE_EMISSION); 
    mp_albedo_texture2d = mp_albedo_texture.FindPropertyRelative("m_texture");
    mp_normal_texture2d = mp_normal_texture.FindPropertyRelative("m_texture");
    mp_metallic_texture2d = mp_metallic_texture.FindPropertyRelative("m_texture");
    mp_emission_texture2d = mp_emission_texture.FindPropertyRelative("m_texture");
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
    RoveMaterial rove_material = (RoveMaterial)target;
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
          EditorGUILayout.LabelField("Material", m_dark_style);
          m_dark_style.fontSize = 12;
          m_dark_style.alignment = TextAnchor.MiddleLeft;
          EditorGUILayout.EndVertical();
        }
        m_dark_style.normal.background = m_bg;
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    EditorGUILayout.BeginVertical("box");
    {
      EditorGUILayout.BeginVertical(m_dark_style);
      {
        GUILayout.Space(15);
        m_dark_style.fontSize = 17;
        m_dark_style.alignment = TextAnchor.MiddleCenter;
        EditorGUILayout.LabelField(m_material_name, m_dark_style);
        m_dark_style.fontSize = 12;
        m_dark_style.alignment = TextAnchor.MiddleLeft;
        GUILayout.Space(15);
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    InspectorProperties(false, 0);
  }
  ///
  public RoveMaterial InspectorProperties(bool folded, int sub_material_index)
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
    RoveMaterial rove_material = (RoveMaterial)target;
    EditorGUILayout.BeginVertical("box");
    {
      EditorGUILayout.BeginVertical(m_dark_style);
      {
        bool fold_out = true;
        if (folded) {
          fold_out = mp_folded_out.boolValue;
          Rect drop_rect = EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            m_dark_style.normal.background = m_gbg;
            EditorGUILayout.BeginHorizontal(m_dark_style);
            {
              if (GUILayout.Button(m_material_name, m_button_style)) { 
                mp_folded_out.boolValue = !mp_folded_out.boolValue;
              }
              if (GUILayout.Button("Select", GUILayout.MaxWidth(75.0f))) {
                EditorGUIUtility.ShowObjectPicker<RoveMaterial>(target, 
                                                                false, " ", 
                                                                sub_material_index);   
              }
              EditorGUILayout.EndHorizontal();
            }
            m_dark_style.normal.background = m_bg;
            EditorGUILayout.EndHorizontal();
          }
          if (Event.current.type == EventType.DragUpdated) {
            if (drop_rect.Contains(Event.current.mousePosition)) {
              if (DragAndDrop.objectReferences.Length == 1 &&
                  (DragAndDrop.objectReferences[0].GetType()) == 
                                                  typeof(RoveMaterial)) {
                DragAndDrop.visualMode = DragAndDropVisualMode.Link;
              } else {
                DragAndDrop.visualMode = DragAndDropVisualMode.Rejected;
              }
            }
          } else if (Event.current.type == EventType.DragPerform) {
            if (drop_rect.Contains(Event.current.mousePosition)) {
              if (DragAndDrop.objectReferences.Length == 1 &&
                  (DragAndDrop.objectReferences[0].GetType()) == 
                                                  typeof(RoveMaterial)) {
                DragAndDrop.AcceptDrag();
                RoveMaterial dropped_material = 
                  (RoveMaterial)DragAndDrop.objectReferences[0];
                if (dropped_material != null) {
                  dropped_material.m_folded_out = mp_folded_out.boolValue;
                }
                EditorGUILayout.EndVertical();
                EditorGUILayout.EndVertical();
                return dropped_material; 
              }
            }
          } else if (Event.current.commandName == "ObjectSelectorUpdated") {
            if (EditorGUIUtility.GetObjectPickerControlID() == sub_material_index) {
              RoveMaterial chosen_material = 
                (RoveMaterial)EditorGUIUtility.GetObjectPickerObject();
              if (chosen_material != null) {
                chosen_material.m_folded_out = mp_folded_out.boolValue;
              }
              EditorGUILayout.EndVertical();
              EditorGUILayout.EndVertical();
              return chosen_material; 
            }
          }
        }
        if (fold_out) {
          GUILayout.Space(20);
          EditorGUILayout.BeginHorizontal();
          {
            EditorGUILayout.LabelField("Type: ", m_dark_style);
            int choice = (mp_glass.floatValue > 0.0f) ? 1 : 0;
            EditorGUI.BeginChangeCheck();
            {
              choice = EditorGUILayout.Popup(choice, m_available_types);
              mp_glass.floatValue = (choice == 0) ? 0.0f : 1.0f;
              if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                rove_material.SetGlass(mp_glass.floatValue);
              }
            }
            GUILayout.FlexibleSpace();
            EditorGUILayout.EndHorizontal();
          }
          GUILayout.Space(20);
          EditorGUILayout.BeginHorizontal(m_dark_style);
          {
            EditorGUILayout.BeginVertical(m_dark_style);
            {
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                m_dark_style.normal.background = m_wbg;
                EditorGUILayout.BeginVertical(m_dark_style);
                {
                  EditorGUI.BeginChangeCheck();
                  {
                    mp_albedo_texture2d.objectReferenceValue = 
                      EditorGUILayout.ObjectField(
                        mp_albedo_texture2d.objectReferenceValue, 
                        typeof(Texture2D), false, GUILayout.MaxWidth(30.0f)); 
                    if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                      rove_material.SetTexture(RoveAtlas.TYPE_ALBEDO, 
                                 (Texture2D)mp_albedo_texture2d.objectReferenceValue);
                      serializedObject.Update();
                    }
                  }
                  EditorGUILayout.EndVertical();
                }
                m_dark_style.normal.background = m_bg;
                EditorGUILayout.LabelField("Albedo: ", m_dark_style,
                                           GUILayout.MaxWidth(90.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              if (mp_glass.floatValue == 0.0f) {
                EditorGUILayout.BeginHorizontal(m_dark_style);
                {
                  m_dark_style.normal.background = m_wbg;
                  EditorGUILayout.BeginVertical(m_dark_style);
                  {
                    EditorGUI.BeginChangeCheck();
                    {
                      mp_metallic_texture2d.objectReferenceValue = 
                        EditorGUILayout.ObjectField(
                          mp_metallic_texture2d.objectReferenceValue, 
                          typeof(Texture2D), false, GUILayout.MaxWidth(30.0f)); 
                      if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                        rove_material.SetTexture(RoveAtlas.TYPE_METALLIC, 
                              (Texture2D)mp_metallic_texture2d.objectReferenceValue);
                        serializedObject.Update();
                      }
                    }
                    EditorGUILayout.EndVertical();
                  }
                  m_dark_style.normal.background = m_bg;
                  EditorGUILayout.LabelField("Metallic: ", m_dark_style,
                                             GUILayout.MaxWidth(90.0f));
                  GUILayout.FlexibleSpace();
                  EditorGUILayout.EndHorizontal();
                }
                EditorGUILayout.Space();
                EditorGUILayout.BeginHorizontal(m_dark_style);
                {
                  GUILayout.FlexibleSpace();
                  EditorGUILayout.LabelField("Smoothness: ", m_dark_style,
                                             GUILayout.MaxWidth(90.0f));
                  EditorGUILayout.EndHorizontal();
                }
                EditorGUILayout.Space();
              } else {
                EditorGUILayout.BeginHorizontal(m_dark_style);
                {
                  m_dark_style.normal.background = m_wbg;
                  EditorGUILayout.BeginVertical(m_dark_style);
                  {
                    EditorGUI.BeginChangeCheck();
                    {
                      mp_metallic_texture2d.objectReferenceValue = 
                        EditorGUILayout.ObjectField(
                          mp_metallic_texture2d.objectReferenceValue, 
                          typeof(Texture2D), false, GUILayout.MaxWidth(30.0f)); 
                      if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                        rove_material.SetTexture(RoveAtlas.TYPE_METALLIC, 
                              (Texture2D)mp_metallic_texture2d.objectReferenceValue);
                        serializedObject.Update();
                      }
                    }
                    EditorGUILayout.EndVertical();
                  }
                  m_dark_style.normal.background = m_bg;
                  EditorGUILayout.LabelField("Smoothness: ", m_dark_style,
                                             GUILayout.MaxWidth(90.0f));
                  GUILayout.FlexibleSpace();
                  EditorGUILayout.EndHorizontal();
                }
                EditorGUILayout.Space();
              }
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                GUILayout.FlexibleSpace();
                EditorGUILayout.LabelField("Sources:", m_dark_style,
                                           GUILayout.MaxWidth(90.0f));
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                m_dark_style.normal.background = m_wbg;
                EditorGUILayout.BeginVertical(m_dark_style);
                {
                  EditorGUI.BeginChangeCheck();
                  {
                    mp_normal_texture2d.objectReferenceValue = 
                      EditorGUILayout.ObjectField(
                        mp_normal_texture2d.objectReferenceValue, 
                        typeof(Texture2D), false, GUILayout.MaxWidth(30.0f)); 
                    if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                      rove_material.SetTexture(RoveAtlas.TYPE_NORMAL, 
                            (Texture2D)mp_normal_texture2d.objectReferenceValue);
                      serializedObject.Update();
                    }
                  }
                  EditorGUILayout.EndVertical();
                }
                m_dark_style.normal.background = m_bg;
                EditorGUILayout.LabelField("Normal Map: ", m_dark_style,
                                           GUILayout.MaxWidth(90.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                m_dark_style.normal.background = m_wbg;
                EditorGUILayout.BeginVertical(m_dark_style);
                {
                  EditorGUI.BeginChangeCheck();
                  {
                    mp_emission_texture2d.objectReferenceValue = 
                      EditorGUILayout.ObjectField(
                        mp_emission_texture2d.objectReferenceValue, 
                        typeof(Texture2D), false, GUILayout.MaxWidth(30.0f)); 
                    if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                      rove_material.SetTexture(RoveAtlas.TYPE_EMISSION, 
                            (Texture2D)mp_emission_texture2d.objectReferenceValue);
                      serializedObject.Update();
                    }
                  }
                  EditorGUILayout.EndVertical();
                }
                m_dark_style.normal.background = m_bg;
                EditorGUILayout.LabelField("Emission: ", m_dark_style,
                                           GUILayout.MaxWidth(90.0f));
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUILayout.LabelField("Double-sided: ", m_dark_style,
                                           GUILayout.MaxWidth(90.0f));
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.EndVertical();
            }
            ///
            EditorGUILayout.BeginVertical(m_dark_style);
            {
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_albedo.colorValue = EditorGUILayout.ColorField(
                                           mp_albedo.colorValue,
                                           GUILayout.MaxWidth(50.0f));
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_material.SetAlbedo(mp_albedo.colorValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              if (mp_glass.floatValue == 0.0f) {
                EditorGUILayout.BeginHorizontal(m_dark_style);
                {
                  EditorGUI.BeginChangeCheck();
                  {
                    mp_metallic.floatValue =
                      EditorGUILayout.Slider(mp_metallic.floatValue, 0.0f, 1.0f);
                    if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                      rove_material.SetMetallic(mp_metallic.floatValue);
                    }
                  }
                  GUILayout.FlexibleSpace();
                  EditorGUILayout.EndHorizontal();
                }
                EditorGUILayout.Space();
              }
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_smoothness.floatValue =
                    EditorGUILayout.Slider(mp_smoothness.floatValue, 0.0f, 1.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_material.SetSmoothness(mp_smoothness.floatValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                int choice = 0;
                if (mp_glass.floatValue == 0.0f) {
                  choice = EditorGUILayout.Popup(choice, m_available_sources);
                } else {
                  choice = EditorGUILayout.Popup(choice, m_available_glass_sources);
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                int choice = 0;
                choice = EditorGUILayout.Popup(choice, m_available_tangents);
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.Space();
              EditorGUILayout.BeginHorizontal(m_dark_style);
              {
                EditorGUI.BeginChangeCheck();
                {
                  mp_emission.floatValue =
                    EditorGUILayout.Slider(mp_emission.floatValue, 0.0f, 1.0f);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_material.SetEmission(mp_emission.floatValue);
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
                  mp_double_sided.boolValue =
                    EditorGUILayout.Toggle(mp_double_sided.boolValue);
                  if (Application.isPlaying && EditorGUI.EndChangeCheck()) {
                    rove_material.SetDoubleSided(mp_double_sided.boolValue);
                  }
                }
                GUILayout.FlexibleSpace();
                EditorGUILayout.EndHorizontal();
              }
              EditorGUILayout.EndVertical();
            }
            EditorGUILayout.EndHorizontal();
          }
          GUILayout.Space(15);
        }
        EditorGUILayout.EndVertical();
      }
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
    return (RoveMaterial)target;
  }
}
