#pragma once

#include "IUnityInterface.h"

UNITY_DECLARE_INTERFACE(IUnityGraphicsD3D11) {
	struct ID3D11Device* (UNITY_INTERFACE_API * GetDevice)();
};
UNITY_REGISTER_INTERFACE_GUID(0xAAB37EF87A87D748ULL, 0xBF76967F07EFB177ULL,
                                                      IUnityGraphicsD3D11)
