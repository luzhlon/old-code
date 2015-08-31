
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINHOOK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINHOOK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WINHOOK_EXPORTS
#define WINHOOK_API __declspec(dllexport)
#else
#define WINHOOK_API __declspec(dllimport)
#endif

// This class is exported from the winhook.dll
class WINHOOK_API CWinhook {
public:
	CWinhook(void);
	// TODO: add your methods here.
};

extern WINHOOK_API int nWinhook;

WINHOOK_API int fnWinhook(void);

