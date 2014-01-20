#pragma once

#define AfxDeferRegisterClass(fClass) AfxEndDeferRegisterClass(fClass)

CLASS_DECL_LINUX WINBOOL AfxEndDeferRegisterClass(LONG fToRegister, const char ** ppszClass);


// Temporary ::collection::map management (locks temp ::collection::map on current thread)
//CLASS_DECL_LINUX void AfxLockTempMaps(sp(base_application) papp);
//CLASS_DECL_LINUX WINBOOL AfxUnlockTempMaps(sp(base_application) papp, WINBOOL bDeleteTemps = TRUE);



// from wincore.cpp
extern CLASS_DECL_LINUX const char _afxWnd[];           // simple child windows/controls
extern CLASS_DECL_LINUX const char _afxWndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_LINUX const char _afxWndMDIFrame[];
extern CLASS_DECL_LINUX const char _afxWndFrameOrView[];
extern CLASS_DECL_LINUX const char _afxWndOleControl[];


/////////////////////////////////////////////////////////////////////////////
// Special helpers

CLASS_DECL_LINUX void AfxcancelModes(oswindow hWndRcvr);
CLASS_DECL_LINUX WINBOOL AfxHelpEnabled();  // determine if ID_HELP handler exists
CLASS_DECL_LINUX WINBOOL AfxCustomLogFont(UINT nIDS, LOGFONT* pLogFont);
CLASS_DECL_LINUX WINBOOL AfxGetPropSheetFont(string & strFace, WORD& wSize, WINBOOL bWizard);

CLASS_DECL_LINUX WINBOOL _AfxIsComboBoxControl(oswindow hWnd, UINT nStyle);
CLASS_DECL_LINUX WINBOOL _AfxCheckCenterDialog(const char * lpszResource);
CLASS_DECL_LINUX WINBOOL _AfxCompareClassName(oswindow hWnd, const char * lpszClassName);
CLASS_DECL_LINUX oswindow _AfxChildWindowFromPoint(oswindow, POINT);

// for determining version of COMCTL32.DLL
#define VERSION_WIN4    MAKELONG(0, 4)
#define VERSION_IE3             MAKELONG(70, 4)
#define VERSION_IE4             MAKELONG(71, 4)
#define VERSION_IE401   MAKELONG(72, 4)
#define VERSION_6      MAKELONG(0, 6)
extern int32_t _afxComCtlVersion;
DWORD _AfxGetComCtlVersion();
