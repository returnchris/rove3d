#include <stdint.h>
#include <d3d11.h>
///
#define ROVE_EXPORT __declspec(dllexport) __stdcall
///
extern "C"
{
  #include "IUnityGraphics.h"
  #include "IUnityInterface.h"
  #include "IUnityGraphicsD3D11.h"
  ///
  static IUnityGraphicsD3D11* rove_id3d_unity = nullptr;
  ///
  ID3D11Device ROVE_EXPORT* RoveUnityGetD3D11Device();
  void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API
         UnityPluginLoad(IUnityInterfaces* unityInterfaces);
}
