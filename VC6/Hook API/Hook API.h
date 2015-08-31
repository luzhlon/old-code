
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HOOKAPI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// HOOKAPI_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef HOOKAPI_EXPORTS
#define HOOKAPI_API __declspec(dllexport)
#else
#define HOOKAPI_API __declspec(dllimport)
#endif

// This class is exported from the Hook API.dll
class HOOKAPI_API CHookAPI {
public:
	// TODO: add your methods here.
};

extern HOOKAPI_API int nHookAPI;

HOOKAPI_API int fnHookAPI(void);

