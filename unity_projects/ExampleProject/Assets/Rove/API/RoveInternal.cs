//#define ROVE_FORCE_GL
using UnityEngine;
using System;
using System.Runtime.InteropServices;
using RoveStatus = System.UInt32;
using RoveLock = System.UInt32;
using RoveComputeAPI = System.UInt32;
///
public class RoveInternal
{
  ///
  private static IntPtr handle;
  private static IntPtr[] function_ptrs;
  private static string[] function_names = { 
                                            "RoveSetD3D11Device",
                                            "RoveGetLock",
                                            "RoveUnitySetupEvent",
                                            "RoveShutdown",
                                            "RoveUnityResizeEvent",
                                            "RoveStartUpdate",
                                            "RoveUnityFinishUpdateEvent",
                                            "RoveResetRenderAtStart",
                                            "RoveResetRenderAtEnd",
                                            "RoveGetThreadCount",
                                            "RoveGetComputeAPI",
                                            "RoveGetDeviceCount",
                                            "RoveGetDeviceName",
                                            "RoveImportMesh",
                                            "RoveFreeMesh",
                                            "RoveDefineSubMesh",
                                            "RoveChangeSubMeshMaterial",
                                            "RoveSetMeshTransform",
                                            "RoveAddMaterial",
                                            "RoveSetMaterialMapFlags",
                                            "RoveSetMaterialDoubleSided",
                                            "RoveSetMaterialAlbedo",
                                            "RoveSetMaterialMetallic",
                                            "RoveSetMaterialSmoothness",
                                            "RoveSetMaterialGlass",
                                            "RoveSetMaterialEmission",
                                            "RoveSetMaterialAlbedoBounds",
                                            "RoveSetMaterialNormalBounds",
                                            "RoveSetMaterialMetallicBounds",
                                            "RoveSetMaterialEmissionBounds",
                                            "RoveSetAlbedoAtlas",
                                            "RoveSetNormalAtlas",
                                            "RoveSetMetallicAtlas",
                                            "RoveSetEmissionAtlas",
                                            "RoveSetFieldOfView",
                                            "RoveSetMaxBounces",
                                            "RoveSetSamplesPerFrame",
                                            "RoveSetImageProperties",
                                            "RoveSetCameraApertureSize",
                                            "RoveSetCameraFocalDepth",
                                            "RoveSetCameraTransform",
                                            "RoveSetEnvironmentType",
                                            "RoveSetSunDirection",
                                            "RoveSetSkyIntensity",
                                            "RoveSetSunIntensity",
                                            "RoveSetSunColor",
                                            "RoveSetEnvironmentMap",
                                            "RoveSetEnvironmentProperties"
                                          };
  public delegate void SetD3D11DeviceDelegate(IntPtr device);
  public static SetD3D11DeviceDelegate SetD3D11Device;
  public delegate RoveLock GetLockDelegate();
  public static GetLockDelegate GetLock;
  public delegate IntPtr SetupDelegate(string data_path,
                                       bool use_multi_device,
                                       bool use_integrated_device,
                                       uint optimization_level,
                                       IntPtr target);
  public static SetupDelegate Setup;
  public delegate void ShutdownDelegate();
  public static ShutdownDelegate Shutdown;
  public delegate IntPtr ResizeDelegate(IntPtr target);
  public static ResizeDelegate Resize;
  public delegate RoveStatus StartUpdateDelegate();
  public static StartUpdateDelegate StartUpdate;
  public delegate IntPtr FinishUpdateDelegate();
  public static FinishUpdateDelegate FinishUpdate;
  public delegate void ResetRenderAtStartDelegate();
  public static ResetRenderAtStartDelegate ResetRenderAtStart;
  public delegate void ResetRenderAtEndDelegate();
  public static ResetRenderAtEndDelegate ResetRenderAtEnd;
  public delegate uint GetThreadCountDelegate();
  public static GetThreadCountDelegate GetThreadCount;
  public delegate RoveComputeAPI GetComputeAPIDelegate();
  public static GetComputeAPIDelegate GetComputeAPI;
  public delegate uint GetDeviceCountDelegate();
  public static GetDeviceCountDelegate GetDeviceCount;
  public delegate IntPtr GetDeviceNameDelegate(uint device_index);
  public static GetDeviceNameDelegate GetDeviceName;
  public delegate RoveStatus ImportMeshDelegate(uint vertex_count,
													                      uint triangle_count,
                                                bool has_normals,
                                                bool has_tangents,
                                                bool has_uvs,
													                      Vector3[] vertices,
													                      int[] triangles,
													                      Vector3[] normals,
													                      Vector4[] tangents,
													                      Vector2[] uvs,
													                      uint sub_mesh_count);
  public static ImportMeshDelegate ImportMesh;
  public delegate void FreeMeshDelegate(uint mesh_id);
  public static FreeMeshDelegate FreeMesh;
  public delegate void DefineSubMeshDelegate(uint mesh_id,
                                             uint sub_mesh_index,
                                             uint triangle_offset,
                                             uint triangle_count,
                                             uint material_id);
  public static DefineSubMeshDelegate DefineSubMesh;
  public delegate void ChangeSubMeshMaterialDelegate(uint mesh_id,
                                                     uint sub_mesh_index,
                                                     uint material_id);
  public static ChangeSubMeshMaterialDelegate ChangeSubMeshMaterial;
  public delegate void SetMeshTransformDelegate(uint mesh_id,
                                                float[] transform);
  public static SetMeshTransformDelegate SetMeshTransform;
  public delegate uint AddMaterialDelegate(uint map_flags,
                                           bool double_sided,
                                           float[] albedo,
                                           float metallic,
                                           float smoothness,
                                           float glass,
                                           float emission,
                                           float[] albedo_bounds,
                                           float[] normal_bounds,
                                           float[] metallic_bounds,
                                           float[] emission_bounds);
  public static AddMaterialDelegate AddMaterial;
  public delegate void SetMaterialMapFlagsDelegate(uint material_id,
                                                   uint map_flag);
  public static SetMaterialMapFlagsDelegate SetMaterialMapFlags;
  public delegate void SetMaterialDoubleSidedDelegate(uint material_id,
                                                      bool double_sided);
  public static SetMaterialDoubleSidedDelegate SetMaterialDoubleSided;
  public delegate void SetMaterialAlbedoDelegate(uint material_id,
                                                 float[] albedo);
  public static SetMaterialAlbedoDelegate SetMaterialAlbedo;
  public delegate void SetMaterialMetallicDelegate(uint material_id,
                                                   float metallic);
  public static SetMaterialMetallicDelegate SetMaterialMetallic;
  public delegate void SetMaterialSmoothnessDelegate(uint material_id,
                                                   float smoothness);
  public static SetMaterialSmoothnessDelegate SetMaterialSmoothness;
  public delegate void SetMaterialGlassDelegate(uint material_id,
                                                float glass);
  public static SetMaterialGlassDelegate SetMaterialGlass;
  public delegate void SetMaterialEmissionDelegate(uint material_id,
                                                   float emission);
  public static SetMaterialEmissionDelegate SetMaterialEmission;
  public delegate void SetMaterialAlbedoBoundsDelegate(uint material_id,
                                                       float[] bounds);
  public static SetMaterialAlbedoBoundsDelegate SetMaterialAlbedoBounds;
  public delegate void SetMaterialNormalBoundsDelegate(uint material_id,
                                                       float[] bounds);
  public static SetMaterialNormalBoundsDelegate SetMaterialNormalBounds;
  public delegate void SetMaterialMetallicBoundsDelegate(uint material_id,
                                                         float[] bounds);
  public static SetMaterialMetallicBoundsDelegate SetMaterialMetallicBounds;
  public delegate void SetMaterialEmissionBoundsDelegate(uint material_id,
                                                         float[] bounds);
  public static SetMaterialEmissionBoundsDelegate SetMaterialEmissionBounds;
  public delegate RoveStatus SetAlbedoAtlasDelegate(byte[] atlas,
                                                    uint width,
                                                    uint height);
  public static SetAlbedoAtlasDelegate SetAlbedoAtlas;
  public delegate RoveStatus SetNormalAtlasDelegate(byte[] atlas,
                                                    uint width,
                                                    uint height);
  public static SetNormalAtlasDelegate SetNormalAtlas;
  public delegate RoveStatus SetMetallicAtlasDelegate(byte[] atlas,
                                                      uint width,
                                                      uint height);
  public static SetMetallicAtlasDelegate SetMetallicAtlas;
  public delegate RoveStatus SetEmissionAtlasDelegate(byte[] atlas,
                                                      uint width,
                                                      uint height);
  public static SetEmissionAtlasDelegate SetEmissionAtlas;
  public delegate void SetFieldOfViewDelegate(float field_of_view);
  public static SetFieldOfViewDelegate SetFieldOfView;
  public delegate void SetMaxBouncesDelegate(uint max_bounces);
  public static SetMaxBouncesDelegate SetMaxBounces;
  public delegate void SetSamplesPerFrameDelegate(uint samples_per_frame);
  public static SetSamplesPerFrameDelegate SetSamplesPerFrame;
  public delegate void SetImagePropertiesDelegate(bool tonemap,
                                                  float gamma,
                                                  float exposure,
                                                  float contrast,
                                                  float saturation,
                                                  float brightness);
  public static SetImagePropertiesDelegate SetImageProperties;
  public delegate void SetCameraApertureSizeDelegate(float aperture_size);
  public static SetCameraApertureSizeDelegate SetCameraApertureSize;
  public delegate void SetCameraFocalDepthDelegate(float focal_depth);
  public static SetCameraFocalDepthDelegate SetCameraFocalDepth;
  public delegate void SetCameraTransformDelegate(float[] transform);
  public static SetCameraTransformDelegate SetCameraTransform;
  public delegate void SetEnvironmentTypeDelegate(uint type);
  public static SetEnvironmentTypeDelegate SetEnvironmentType;
  public delegate void SetSunDirectionDelegate(float[] direction);
  public static SetSunDirectionDelegate SetSunDirection;
  public delegate void SetSkyIntensityDelegate(float intensity);
  public static SetSkyIntensityDelegate SetSkyIntensity;
  public delegate void SetSunIntensityDelegate(float intensity);
  public static SetSunIntensityDelegate SetSunIntensity;
  public delegate void SetSunColorDelegate(float[] color);
  public static SetSunColorDelegate SetSunColor; 
  public delegate void SetEnvironmentMapDelegate(byte[] map,
                                                 uint width,
                                                 uint height);
  public static SetEnvironmentMapDelegate SetEnvironmentMap;
  public delegate void SetEnvironmentPropertiesDelegate(float intensity,
                                                        float rotation);
  public static SetEnvironmentPropertiesDelegate SetEnvironmentProperties;
  ///
  #if (UNITY_EDITOR_LINUX || UNITY_STANDALONE_LINUX) 
    [DllImport("libdl.so")]
    protected static extern IntPtr dlopen(string filename, int flags);
    [DllImport("libdl.so")]
    protected static extern IntPtr dlsym(IntPtr handle, string symbol);
    [DllImport("libdl.so")]
    protected static extern int dlclose(IntPtr handle);
    [DllImport("libdl.so")]
    protected static extern IntPtr dlerror();
  #elif (UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX) 
    [DllImport("libdl.dylib")]
    protected static extern IntPtr dlopen(string filename, int flags);
    [DllImport("libdl.dylib")]
    protected static extern IntPtr dlsym(IntPtr handle, string symbol);
    [DllImport("libdl.dylib")]
    protected static extern int dlclose(IntPtr handle);
    [DllImport("libdl.dylib")]
    protected static extern IntPtr dlerror();
  #elif (UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN)
    #if (!ROVE_FORCE_GL)
      [DllImport ("RovePluginProxy")]
      public static extern IntPtr RoveUnityGetD3D11Device();
    #endif
    [DllImport("kernel32.dll")]
    protected static extern IntPtr LoadLibrary(string filename);
    [DllImport("kernel32.dll")]
    protected static extern IntPtr GetProcAddress(IntPtr hModule, 
                                                  string procname);
    [DllImport("kernel32.dll")]
    protected static extern bool FreeLibrary(IntPtr hModule);
    [DllImport("kernel32.dll")]
    protected static extern uint GetLastError();
  #endif
  ///
  public static bool LoadPlugin(bool force_opencl)
  {
    bool use_nvidia = SystemInfo.graphicsDeviceName.Contains("NVIDIA") ||
                      SystemInfo.graphicsDeviceName.Contains("GeForce");
    function_ptrs = new IntPtr[function_names.Length];
    #if (UNITY_EDITOR_LINUX || UNITY_STANDALONE_LINUX)
      if (use_nvidia && !force_opencl) {
        #if (UNITY_EDITOR)
          handle = dlopen("Assets/Rove/ImportedPlugins/x86_64/libRoveGLCUDA.so", 2);
        #else
          handle = dlopen(Application.dataPath + "/Rove/Data/libRoveGLCUDA.so", 2);
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library."); 
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      } else {
        #if (UNITY_EDITOR)
          handle = dlopen("Assets/Rove/ImportedPlugins/x86_64/libRoveGLCL.so", 2);
        #else
          handle = dlopen(Application.dataPath + "/Rove/Data/libRoveGLCL.so", 2);
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library."); 
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      }
      for (uint fi = 1; fi < function_names.Length; ++fi) {
        function_ptrs[fi] = dlsym(handle, function_names[fi]); 
        if (function_ptrs[fi] == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get function pointer: " + 
                         function_names[fi]);
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      }
    #elif (UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX) 
      if (use_nvidia && !force_opencl) {
        #if (UNITY_EDITOR)
          handle = dlopen("Assets/Rove/ImportedPlugins/x86_64/RoveGLCUDA.bundle", 2);
        #else
          handle = dlopen(Application.dataPath + "/Rove/Data/RoveGLCUDA.bundle", 2);
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library."); 
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      } else {
        #if (UNITY_EDITOR)
          handle = dlopen("Assets/Rove/ImportedPlugins/x86_64/RoveGLCL.bundle", 2);
        #else
          handle = dlopen(Application.dataPath + "/Rove/Data/RoveGLCL.bundle", 2);
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library."); 
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      }
      for (uint fi = 1; fi < function_names.Length; ++fi) {
        function_ptrs[fi] = dlsym(handle, function_names[fi]); 
        if (function_ptrs[fi] == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get function pointer: " + 
                         function_names[fi]);
          string error = Marshal.PtrToStringAuto(dlerror());
          Debug.LogError(error);
          return false;
        }
      }
    #elif (UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN)
      if (use_nvidia && !force_opencl) {
        #if (ROVE_FORCE_GL)
          #if (UNITY_EDITOR)
            handle = LoadLibrary("Assets/Rove/ImportedPlugins/x86_64/RoveGLCUDA.dll");
          #else
            handle = LoadLibrary(Application.dataPath + "/Rove/Data/RoveGLCUDA.dll");
          #endif
        #else
          #if (UNITY_EDITOR)
            handle = LoadLibrary("Assets/Rove/ImportedPlugins/x86_64/RoveD3D11CUDA.dll");
          #else
            handle = LoadLibrary(Application.dataPath + "/Rove/Data/RoveD3D11CUDA.dll");
          #endif
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library.");
          Debug.LogError("Error code: " + GetLastError() + ".");
          return false;
        }
      } else {
        #if (ROVE_FORCE_GL)
          #if (UNITY_EDITOR)
            handle = LoadLibrary("Assets/Rove/ImportedPlugins/x86_64/RoveGLCL.dll");
          #else
            handle = LoadLibrary(Application.dataPath + "/Rove/Data/RoveGLCL.dll");
          #endif
        #else
          #if (UNITY_EDITOR)
            handle = LoadLibrary("Assets/Rove/ImportedPlugins/x86_64/RoveD3D11CL.dll");
          #else
            handle = LoadLibrary(Application.dataPath + "/Rove/Data/RoveD3D11CL.dll");
          #endif
        #endif
        if (handle == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get library.");
          Debug.LogError("Error code: " + GetLastError() + ".");
          return false;
        }
      }
      #if (ROVE_FORCE_GL)
        for (uint fi = 1; fi < function_names.Length; ++fi) {
      #else
        for (uint fi = 0; fi < function_names.Length; ++fi) {
      #endif
        function_ptrs[fi] = GetProcAddress(handle, function_names[fi]); 
        if (function_ptrs[fi] == IntPtr.Zero) {
          Debug.LogError("Rove: Failed to get function pointer: " + 
                         function_names[fi]);
          Debug.LogError("Error code: " + GetLastError() + ".");
          return false;
        }
      }
#endif
    uint i = 0;
    #if ((!ROVE_FORCE_GL) && (UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN))
      SetD3D11Device = (SetD3D11DeviceDelegate)
        Marshal.GetDelegateForFunctionPointer(function_ptrs[i++], 
                                              typeof(SetD3D11DeviceDelegate));
    #else
      ++i;
    #endif
    GetLock = (GetLockDelegate)Marshal.GetDelegateForFunctionPointer(
               function_ptrs[i++], typeof(GetLockDelegate));
    Setup = (SetupDelegate)Marshal.GetDelegateForFunctionPointer(
             function_ptrs[i++], typeof(SetupDelegate));
    Shutdown = (ShutdownDelegate)Marshal.GetDelegateForFunctionPointer(
                function_ptrs[i++], typeof(ShutdownDelegate));
    Resize = (ResizeDelegate)Marshal.GetDelegateForFunctionPointer(
              function_ptrs[i++], typeof(ResizeDelegate));
    StartUpdate = (StartUpdateDelegate)Marshal.GetDelegateForFunctionPointer(
                   function_ptrs[i++], typeof(StartUpdateDelegate));
    FinishUpdate = (FinishUpdateDelegate)Marshal.GetDelegateForFunctionPointer(
                    function_ptrs[i++], typeof(FinishUpdateDelegate));
    ResetRenderAtStart = 
      (ResetRenderAtStartDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(ResetRenderAtStartDelegate));
    ResetRenderAtEnd = 
      (ResetRenderAtEndDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(ResetRenderAtEndDelegate));
    GetThreadCount = 
      (GetThreadCountDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(GetThreadCountDelegate));
    GetComputeAPI = 
      (GetComputeAPIDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(GetComputeAPIDelegate));
    GetDeviceCount = 
      (GetDeviceCountDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(GetDeviceCountDelegate));
    GetDeviceName = 
      (GetDeviceNameDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(GetDeviceNameDelegate));
    ImportMesh = (ImportMeshDelegate)Marshal.GetDelegateForFunctionPointer(
                  function_ptrs[i++], typeof(ImportMeshDelegate));
    FreeMesh = (FreeMeshDelegate)Marshal.GetDelegateForFunctionPointer(
                function_ptrs[i++], typeof(FreeMeshDelegate));
    DefineSubMesh = (DefineSubMeshDelegate)Marshal.GetDelegateForFunctionPointer(
                     function_ptrs[i++], typeof(DefineSubMeshDelegate));
    ChangeSubMeshMaterial = 
      (ChangeSubMeshMaterialDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(ChangeSubMeshMaterialDelegate));
    SetMeshTransform = 
      (SetMeshTransformDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMeshTransformDelegate));
    AddMaterial = (AddMaterialDelegate)Marshal.GetDelegateForFunctionPointer(
                   function_ptrs[i++], typeof(AddMaterialDelegate));
    SetMaterialMapFlags = 
      (SetMaterialMapFlagsDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialMapFlagsDelegate));
    SetMaterialDoubleSided = 
      (SetMaterialDoubleSidedDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialDoubleSidedDelegate));
    SetMaterialAlbedo = 
      (SetMaterialAlbedoDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialAlbedoDelegate));
    SetMaterialMetallic = 
      (SetMaterialMetallicDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialMetallicDelegate));
    SetMaterialSmoothness = 
      (SetMaterialSmoothnessDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialSmoothnessDelegate));
    SetMaterialGlass = 
      (SetMaterialGlassDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialGlassDelegate));
    SetMaterialEmission = 
      (SetMaterialEmissionDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialEmissionDelegate));
    SetMaterialAlbedoBounds = 
      (SetMaterialAlbedoBoundsDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialAlbedoBoundsDelegate));
    SetMaterialNormalBounds = 
      (SetMaterialNormalBoundsDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialNormalBoundsDelegate));
    SetMaterialMetallicBounds = 
      (SetMaterialMetallicBoundsDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialMetallicBoundsDelegate));
    SetMaterialEmissionBounds = 
      (SetMaterialEmissionBoundsDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaterialEmissionBoundsDelegate));
    SetAlbedoAtlas = 
      (SetAlbedoAtlasDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetAlbedoAtlasDelegate));
    SetNormalAtlas = 
      (SetNormalAtlasDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetNormalAtlasDelegate));
    SetMetallicAtlas = 
      (SetMetallicAtlasDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMetallicAtlasDelegate));
    SetEmissionAtlas = 
      (SetEmissionAtlasDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetEmissionAtlasDelegate));
    SetFieldOfView = 
      (SetFieldOfViewDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetFieldOfViewDelegate));
    SetMaxBounces = 
      (SetMaxBouncesDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetMaxBouncesDelegate));
    SetSamplesPerFrame = 
      (SetSamplesPerFrameDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetSamplesPerFrameDelegate));
    SetImageProperties = 
      (SetImagePropertiesDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetImagePropertiesDelegate));
    SetCameraApertureSize = 
      (SetCameraApertureSizeDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetCameraApertureSizeDelegate));
    SetCameraFocalDepth = 
      (SetCameraFocalDepthDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetCameraFocalDepthDelegate));
    SetCameraTransform = 
      (SetCameraTransformDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetCameraTransformDelegate));
    SetEnvironmentType = 
      (SetEnvironmentTypeDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetEnvironmentTypeDelegate));
    SetSunDirection = 
      (SetSunDirectionDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetSunDirectionDelegate));
    SetSkyIntensity = 
      (SetSkyIntensityDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetSkyIntensityDelegate));
    SetSunIntensity = 
      (SetSunIntensityDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetSunIntensityDelegate));
    SetSunColor = 
      (SetSunColorDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetSunColorDelegate));
    SetEnvironmentMap = 
      (SetEnvironmentMapDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetEnvironmentMapDelegate));
    SetEnvironmentProperties = 
      (SetEnvironmentPropertiesDelegate)Marshal.GetDelegateForFunctionPointer(
       function_ptrs[i++], typeof(SetEnvironmentPropertiesDelegate));
    return true;
  }
  ///
  public static void UnloadPlugin()
  {
    #if (UNITY_EDITOR_LINUX || UNITY_STANDALONE_LINUX)
      dlclose(handle);
    #elif (UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX)
      dlclose(handle);
    #elif (UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN)
      FreeLibrary(handle);
    #endif
  }
}
