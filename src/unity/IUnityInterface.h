#pragma once

#if defined(__CYGWIN32__)
	#define UNITY_INTERFACE_API __stdcall
	#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
	#define UNITY_INTERFACE_API __stdcall
	#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(__QNX__)
	#define UNITY_INTERFACE_API
	#define UNITY_INTERFACE_EXPORT
#else
	#define UNITY_INTERFACE_API
	#define UNITY_INTERFACE_EXPORT
#endif

struct UnityInterfaceGUID {
	unsigned long long m_GUIDHigh;
	unsigned long long m_GUIDLow;
};
typedef struct UnityInterfaceGUID UnityInterfaceGUID;

#define UNITY_GET_INTERFACE_GUID(TYPE) TYPE##_GUID
#define UNITY_GET_INTERFACE(INTERFACES, TYPE) \
  (TYPE*)INTERFACES->GetInterface(UNITY_GET_INTERFACE_GUID(TYPE));

#define UNITY_DECLARE_INTERFACE(NAME) \
typedef struct NAME NAME;             \
struct NAME

// Rove changed this to static to avoid multiple definitions
#define UNITY_REGISTER_INTERFACE_GUID(HASHH, HASHL, TYPE) \
static const UnityInterfaceGUID TYPE##_GUID = {HASHH, HASHL};

typedef void IUnityInterface;

typedef struct IUnityInterfaces {
	IUnityInterface* (UNITY_INTERFACE_API * GetInterface)(UnityInterfaceGUID guid);
	void (UNITY_INTERFACE_API * RegisterInterface)(UnityInterfaceGUID guid, 
                                                   IUnityInterface* ptr);
} IUnityInterfaces;

void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad
                           (IUnityInterfaces* unityInterfaces);
void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload();
