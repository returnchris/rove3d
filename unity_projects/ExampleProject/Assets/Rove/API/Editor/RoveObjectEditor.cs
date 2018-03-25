using System.Collections;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
///
[CustomEditor(typeof(RoveObject))]
[CanEditMultipleObjects]
public class RoveObjectEditor : Editor
{
  private Texture2D m_logo;
  private GUIStyle m_dark_style;
  private Texture2D m_bg;
  private Texture2D m_wbg;
  private Texture2D m_gbg;
  private SerializedProperty mp_materials;
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
    RoveObject rove_object = (RoveObject)target;
    if (rove_object.m_materials == null) {
      rove_object.m_materials = new List<RoveMaterial>();
    }
    mp_materials = serializedObject.FindProperty("m_materials");
    serializedObject.Update();
    MeshFilter mesh_filter = rove_object.GetComponent<MeshFilter>();
    if (!mesh_filter) {
      Debug.LogError("Rove: Could not find a mesh filter on object.");
      Destroy(rove_object);
      return;
    }
    if (mp_materials.arraySize != mesh_filter.sharedMesh.subMeshCount) {
      mp_materials.ClearArray();
      for (int m = 0; m < mesh_filter.sharedMesh.subMeshCount; ++m) {
        mp_materials.InsertArrayElementAtIndex(m);
        SerializedProperty p_material = mp_materials.GetArrayElementAtIndex(m);
        p_material.objectReferenceValue = null;
      }
    }
    serializedObject.ApplyModifiedProperties();
  }
  ///
  public override void OnInspectorGUI()
  {
    serializedObject.Update();
    RoveObject rove_object = (RoveObject)target;
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
          EditorGUILayout.LabelField("Object", m_dark_style);
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
      GUILayout.Space(15);
      m_dark_style.fontSize = 17;
      m_dark_style.alignment = TextAnchor.MiddleCenter;
      EditorGUILayout.LabelField("Sub-Mesh Materials", m_dark_style);
      m_dark_style.fontSize = 12;
      m_dark_style.alignment = TextAnchor.MiddleLeft;
      GUILayout.Space(15);
      for (int m = 0; m < mp_materials.arraySize; ++m) {
        SerializedProperty p_material = mp_materials.GetArrayElementAtIndex(m); 
        RoveMaterial material = (RoveMaterial)p_material.objectReferenceValue;
        if (material == null) {
          EditorGUILayout.BeginVertical("box");
          {
            EditorGUILayout.BeginVertical(m_dark_style);
            {
							Rect drop_rect = EditorGUILayout.BeginHorizontal(m_dark_style);
							{
								m_dark_style.normal.background = m_gbg;
								EditorGUILayout.BeginHorizontal(m_dark_style);
								{
									GUIStyle button_style = new GUIStyle(GUI.skin.button);
									button_style.fontSize = 13;
									button_style.alignment = TextAnchor.MiddleLeft;
									button_style.normal.background = m_gbg;
									button_style.normal.textColor = Color.white;
									button_style.active.background = m_gbg;
									button_style.active.textColor = Color.white;
									if (GUILayout.Button("Default Material", button_style)) { }
									if (GUILayout.Button("Select", GUILayout.MaxWidth(75.0f))) {
										EditorGUIUtility.ShowObjectPicker<RoveMaterial>(target,
																																		false, " ", m);
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
										p_material.objectReferenceValue = dropped_material;
										rove_object.m_materials[m] = dropped_material;
                    if (Application.isPlaying) {
                      rove_object.ChangeSubMeshMaterial(m, dropped_material); 
                    }
									}
								}
							} else if (Event.current.commandName == "ObjectSelectorUpdated") {
                if (EditorGUIUtility.GetObjectPickerControlID() == m) {
                  RoveMaterial chosen_material =
                    (RoveMaterial)EditorGUIUtility.GetObjectPickerObject();
                  p_material.objectReferenceValue = chosen_material;
                  rove_object.m_materials[m] = chosen_material;
                  if (Application.isPlaying) {
                    rove_object.ChangeSubMeshMaterial(m, chosen_material); 
                  }
                }
              }
              EditorGUILayout.EndVertical();
            }
            EditorGUILayout.EndVertical();
          }
        } else {
          RoveMaterialEditor material_editor = 
            (RoveMaterialEditor)RoveMaterialEditor.CreateEditor(
                                  material, typeof(RoveMaterialEditor));
          RoveMaterial new_material = material_editor.InspectorProperties(true, m);
          if (new_material != material) {
            p_material.objectReferenceValue = new_material;
            rove_object.m_materials[m] = new_material;
            if (Application.isPlaying) {
              rove_object.ChangeSubMeshMaterial(m, new_material); 
            }
          }
        }
      }
      EditorGUILayout.EndVertical();
    }
    serializedObject.ApplyModifiedProperties();
  }
}
