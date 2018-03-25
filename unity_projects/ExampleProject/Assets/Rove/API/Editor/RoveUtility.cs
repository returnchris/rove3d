using System.IO;
using UnityEngine;
using UnityEditor;
using UnityEditor.Callbacks;
///
public class RoveUtility
{
  [PostProcessBuildAttribute(1)]
  public static void OnPostprocessBuild(BuildTarget target,
                                        string pathToBuiltProject)
  {
    bool found_data_directory = false;
    string[] sub_directories = 
      Directory.GetDirectories(new FileInfo(pathToBuiltProject).Directory.FullName);
    string data_path = "";
    foreach(string sub in sub_directories) {
      if (sub.Contains("_Data")) {
        data_path = sub + "/Rove/Data";
        found_data_directory = true;
      } else if (sub.Contains(".app")) {
        data_path = sub + "/Contents/Rove/Data";
        found_data_directory = true;
      }
    }
    if (!found_data_directory) {
      Debug.LogError("Rove: Failed to find built application data directory.");
    }
    System.IO.Directory.CreateDirectory(data_path);
    File.Copy("Assets/Rove/Data/sky.data", data_path + "/sky.data");
    if (target == BuildTarget.StandaloneOSXIntel64) {
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveGLCL.bundle", 
                data_path + "/RoveGLCL.bundle");
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveGLCUDA.bundle", 
                data_path + "/RoveGLCUDA.bundle");
    } else if (target == BuildTarget.StandaloneWindows64) {
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveD3D11CL.dll", 
                data_path + "/RoveD3D11CL.dll");
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveD3D11CUDA.dll", 
                data_path + "/RoveD3D11CUDA.dll");
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveGLCL.dll", 
                data_path + "/RoveGLCL.dll");
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/RoveGLCUDA.dll", 
                data_path + "/RoveGLCUDA.dll");
    } else if (target == BuildTarget.StandaloneLinux64) {
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/libRoveGLCL.so", 
                data_path + "/libRoveGLCL.so");
      File.Copy("Assets/Rove/ImportedPlugins/x86_64/libRoveGLCUDA.so", 
                data_path + "/libRoveGLCUDA.so");
    } else {
      found_data_directory = false;
    }
    if (!found_data_directory) {
      Debug.LogError("Rove: Could not copy plugins for unrecognized platform: " +
                     target);
    }
  }
}
