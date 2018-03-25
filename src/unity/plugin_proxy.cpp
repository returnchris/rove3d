#include "plugin_proxy.h"
///
ID3D11Device* RoveUnityGetD3D11Device()
{
  if (rove_id3d_unity) {
    return rove_id3d_unity->GetDevice();
  } else {
    return nullptr;
  }
}
///
void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
       UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
  rove_id3d_unity = UNITY_GET_INTERFACE(unityInterfaces, 
                                        IUnityGraphicsD3D11);
}