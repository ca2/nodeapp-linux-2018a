#pragma once

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)

CLASS_DECL_LNX WINBOOL AfxEndDeferRegisterClass(LONG fToRegister, const char ** ppszClass);


// Temporary ::collection::map management (locks temp ::collection::map on current thread)
//CLASS_DECL_LNX void AfxLockTempMaps(::radix::application * papp);
//CLASS_DECL_LNX WINBOOL AfxUnlockTempMaps(::radix::application * papp, WINBOOL bDeleteTemps = TRUE);



// from wincore.cpp
extern CLASS_DECL_LNX const char _afxWnd[];           // simple child windows/controls
extern CLASS_DECL_LNX const char _afxWndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_LNX const char _afxWndMDIFrame[];
extern CLASS_DECL_LNX const char _afxWndFrameOrView[];
extern CLASS_DECL_LNX const char _afxWndOleControl[];


/////////////////////////////////////////////////////////////////////////////
// Special helpers

CLASS_DECL_LNX void AfxCancelModes(HWND hWndRcvr);
CLASS_DECL_LNX WINBOOL AfxHelpEnabled();  // determine if ID_HELP handler exists
CLASS_DECL_LNX WINBOOL AfxCustomLogFont(UINT nIDS, LOGFONT* pLogFont);
CLASS_DECL_LNX WINBOOL AfxGetPropSheetFont(string & strFace, WORD& wSize, WINBOOL bWizard);

CLASS_DECL_LNX WINBOOL _AfxIsComboBoxControl(HWND hWnd, UINT nStyle);
CLASS_DECL_LNX WINBOOL _AfxCheckCenterDialog(const char * lpszResource);
CLASS_DECL_LNX WINBOOL _AfxCompareClassName(HWND hWnd, const char * lpszClassName);
CLASS_DECL_LNX HWND _AfxChildWindowFromPoint(HWND, POINT);

// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3             MAKELONG(70, 4)
#define VERSION_IE4             MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
#define VERSION_6      MAKELONG(0, 6)
extern int _afxComCtlVersion;
DWORD _AfxGetComCtlVersion();
