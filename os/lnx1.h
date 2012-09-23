#pragma once

namespace win
{
   class thread;

   class win
   {
      int function();
   };

   CLASS_DECL_LNX HINSTANCE   LoadLibrary(const char * lpsz);
   CLASS_DECL_LNX WINBOOL        SHGetSpecialFolderPath(HWND hwnd, string &str, int csidl, WINBOOL fCreate);
   CLASS_DECL_LNX DWORD       GetFileAttributes(const char * lpFileName);
   CLASS_DECL_LNX WINBOOL        CreateDirectory(const char * lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
   CLASS_DECL_LNX DWORD       GetCurrentDirectory(string & str);
   CLASS_DECL_LNX DWORD       GetTempPath(string & str);
//   CLASS_DECL_LNX LONG        RegQueryValue(HKEY hkey, const char * lpszSubKey, string & str);
   CLASS_DECL_LNX HICON       ExtractIcon(HINSTANCE hInst, const char * lpszExeFileName, UINT nIconIndex);
   CLASS_DECL_LNX WINBOOL        DeleteFile(const char * lpFileName);
   CLASS_DECL_LNX int         GetMenuStringW(HMENU hMenu, UINT uIDItem, string & str, UINT flags);
   CLASS_DECL_LNX void        TimeToFileTime(::ca::application * papp, const class time& time, LPFILETIME pFileTime);

} // namespace win



// Placed on frame for EXCEPTION linkage, or base_exception cleanup
struct CLASS_DECL_LNX __EXCEPTION_LINK
{
   __EXCEPTION_LINK* m_pLinkPrev;    // previous top, next in handler chain
   base_exception* m_pException;   // current exception (NULL in try block)

   __EXCEPTION_LINK();       // for initialization and linking
   ~__EXCEPTION_LINK()       // for cleanup and unlinking
      { AfxTryCleanup(); };
};

// Exception global state - never access directly
struct CLASS_DECL_LNX __EXCEPTION_CONTEXT
{
   __EXCEPTION_LINK* m_pLinkTop;

   // Note: most of the exception context is now in the __EXCEPTION_LINK
};

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif

_PNH CLASS_DECL_LNX AfxGetNewHandler();
_PNH CLASS_DECL_LNX AfxSetNewHandler(_PNH pfnNewHandler);
CLASS_DECL_ca int __CDECL AfxNewHandler(size_t nSize);

void CLASS_DECL_LNX AfxAbort();


// helpers for registering your own WNDCLASSes
CLASS_DECL_LNX const char * AfxRegisterWndClass(UINT nClassStyle,
   HCURSOR hCursor = 0, HBRUSH hbrBackground = 0, HICON hIcon = 0);

//CLASS_DECL_LNX WINBOOL AfxRegisterClass(WNDCLASS* lpWndClass);

// helper to initialize rich edit 1.0 control
CLASS_DECL_LNX WINBOOL AfxInitRichEdit();
// helper to initialize rich edit 2.0 control
CLASS_DECL_LNX WINBOOL AfxInitRichEdit2();

CLASS_DECL_LNX LRESULT CALLBACK AfxWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
//CLASS_DECL_LNX WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

typedef void (__MSG_CALL ::ca::window::*__PMSGW)(void);
   // like '__PMSG' but for ::ca::window derived classes only

typedef void (__MSG_CALL ::radix::thread::*__PMSGT)(void);
   // like '__PMSG' but for thread-derived classes only




//CLASS_DECL_LNX LONG AfxDelRegTreeHelper(HKEY hParentKey, const string & strKeyName);


// Advanced initialization: for overriding default diagnostics
CLASS_DECL_LNX WINBOOL AfxDiagnosticInit(void);


CLASS_DECL_LNX ::win::thread * AfxGetThread();
CLASS_DECL_LNX void AfxSetThread(::radix::thread * pthread);
CLASS_DECL_LNX MSG* AfxGetCurrentMessage();

CLASS_DECL_LNX void AfxEndThread(::radix::application * papp, UINT nExitCode, WINBOOL bDelete = TRUE);

CLASS_DECL_LNX void AfxInitThread();
CLASS_DECL_LNX void AfxTermThread(::radix::application * papp, HINSTANCE hInstTerm = NULL);

/////////////////////////////////////////////////////////////////////////////
// Global functions for access to the one and only application

#define afxCurrentWinApp    AfxGetModuleState()->m_pCurrentWinApp
#define afxCurrentInstanceHandle    AfxGetModuleState()->m_hCurrentInstanceHandle
#define afxCurrentResourceHandle    AfxGetModuleState()->m_hCurrentResourceHandle
#define afxCurrentAppName   AfxGetModuleState()->m_lpszCurrentAppName
#define afxContextIsDLL     AfxGetModuleState()->m_bDLL
#define afxRegisteredClasses    AfxGetModuleState()->m_fRegisteredClasses
#define afxAmbientActCtx    AfxGetModuleState()->m_bSetAmbientActCtx

#ifndef _AFX_NO_OCC_SUPPORT
#define afxOccManager   AfxGetModuleState()->m_pOccManager
#endif

//Fusion: Access macros for WinSxS dynamic wrappers.
#ifndef _AFX_NO_AFXCMN_SUPPORT
#define _AFX_COMCTL32_ISOLATION_WRAPPER_INDEX 0
#define afxComCtlWrapper static_cast<CComCtlWrapper*>(AfxGetModuleState()->m_pDllIsolationWrappers[_AFX_COMCTL32_ISOLATION_WRAPPER_INDEX])
#endif

#define _AFX_COMMDLG_ISOLATION_WRAPPER_INDEX 1
#define afxCommDlgWrapper static_cast<CCommDlgWrapper*>(AfxGetModuleState()->m_pDllIsolationWrappers[_AFX_COMMDLG_ISOLATION_WRAPPER_INDEX])

// Advanced initialization: for overriding default WinMain
//CLASS_DECL_LNX WINBOOL AfxWinInit(__in HINSTANCE hInstance, __in HINSTANCE hPrevInstance,
  // __in_z LPTSTR lpCmdLine, __in int nCmdShow);
CLASS_DECL_LNX void AfxWinTerm();

CLASS_DECL_LNX ::radix::application* AfxGetApp();
CLASS_DECL_LNX ::user::interaction* AfxGetMainWnd();
//CLASS_DECL_LNX HINSTANCE CLASS_DECL_LNX System.m_hInstance;
CLASS_DECL_LNX HINSTANCE AfxGetResourceHandle();
CLASS_DECL_LNX void AfxSetResourceHandle(HINSTANCE hInstResource);
CLASS_DECL_LNX const char * AfxGetAppName();

CLASS_DECL_LNX HINSTANCE AfxGetResourceHandle();
CLASS_DECL_LNX HINSTANCE AfxFindStringResourceHandle(UINT nID);

CLASS_DECL_LNX void AfxDeleteObject(HGDIOBJ* pObject);

void CLASS_DECL_LNX AfxGlobalFree(HGLOBAL hGlobal);
