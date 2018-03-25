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
///
public class RoveCapture : MonoBehaviour
{
  public int m_global_framecount;
  public string m_rendering_sub_directory;
  public string m_rendering_directory;
  public bool m_animation_rendering;
  public bool m_frame_rendering;
  public int m_animation_framecount;
  public int m_animation_current_frame;
  private RoveSetup m_rove_setup;
  ///
  void Start()
  {
    m_rove_setup = RoveSetup.GetAPIComponent<RoveSetup>();       
    if (!m_rove_setup) {
      Debug.LogError("Rove: Could not find an active RoveSetup component.");
      Destroy(this);
      return;
    }
    m_global_framecount = 0;
  }
  ///
  void Update()
  {
    if (!m_rove_setup.m_setup_success) {
      return;
    }
    if (m_global_framecount > 0) {
      if (m_frame_rendering) {
        StartCoroutine(SaveFrame(false));
      } else if (m_animation_rendering) {
        StartCoroutine(SaveFrame(true));
      }
    }
    ++m_global_framecount;
  }
  /// API functions.
  ///
  public void CaptureNewAnimation(string sub_directory,
                                  int framecount)
  {
    #if UNITY_EDITOR
      RoveInternal.ResetRenderAtStart();
      m_animation_framecount = framecount;
      m_animation_current_frame = 0;
      m_rendering_sub_directory = sub_directory;
      m_rendering_directory = Application.dataPath + "/RoveRenders";
      if (m_rendering_sub_directory.Length > 0) {
        m_rendering_directory += "/" + m_rendering_sub_directory;
      }
      Directory.CreateDirectory(m_rendering_directory);
      m_frame_rendering = false;
      m_animation_rendering = true;
    #endif
  }
  ///
  public void CaptureCurrentFrame(string sub_directory)
  {
    #if UNITY_EDITOR
      m_rendering_sub_directory = sub_directory;
      m_rendering_directory = Application.dataPath + "/RoveRenders";
      if (m_rendering_sub_directory.Length > 0) {
        m_rendering_directory += "/" + m_rendering_sub_directory;
      }
      Directory.CreateDirectory(m_rendering_directory);
      m_animation_rendering = false;
      m_frame_rendering = true;
    #endif
  }
  ///
  private IEnumerator SaveFrame(bool is_animation)
  {
    #if UNITY_EDITOR
      yield return new WaitForEndOfFrame();
      int width = Screen.width;
      int height = Screen.height;
      Texture2D rendered_image = new Texture2D(width, height,
                                               TextureFormat.RGB24, false);
      rendered_image.ReadPixels(new Rect(0, 0, width, height), 0, 0);
      rendered_image.Apply();
      byte[] bytes = rendered_image.EncodeToPNG();
      DestroyImmediate(rendered_image, true);
      uint i = 0;
      while (true) {
        string filename = m_rendering_directory + "/frame_" + i + ".png";
        if (File.Exists(filename)) {
          ++i;
          continue;
        }
        try {
          File.WriteAllBytes(filename, bytes);
        } catch {
          Debug.LogError("Rove: Failed to write rendered image to: " +
                         filename + ".");
          throw;
        }
        if (is_animation) {
          Debug.Log("Rove: Saved rendered frame: " + m_animation_current_frame + "/" +
                    m_animation_framecount + ", out to: " + filename + ".");
          ++m_animation_current_frame;
          if (m_animation_current_frame >= m_animation_framecount) {
            Debug.Log("Rove: Animation capture completed. " +
                      "Saved to: '" + m_rendering_directory + "/'.");
            m_animation_rendering = false;
            EditorApplication.isPlaying = false;
            Application.Quit();
            AssetDatabase.Refresh();
          }
        } else {
          Debug.Log("Rove: Saved rendered frame out to: " + filename + ".");
          m_frame_rendering = false;
          AssetDatabase.Refresh();
        }
        break; 
      }
      if (is_animation) {
        RoveInternal.ResetRenderAtStart();
      }
    #endif
    yield return null;
  }
}
