#pragma once

namespace win
{
   class thread;
} // namespace win

//#include "types.h"
//#include "template.h"

#ifndef __AFXTLS_H__
   #include "thread_slots.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// _AFX_DEBUG_STATE

#ifdef DEBUG

class _AFX_DEBUG_STATE : public no_track_object
{
public:
   _AFX_DEBUG_STATE();
   virtual ~_AFX_DEBUG_STATE();
};

EXTERN_PROCESS_LOCAL(_AFX_DEBUG_STATE, afxDebugState)

#endif //DEBUG

/////////////////////////////////////////////////////////////////////////////
// _AFX_WIN_STATE


class _AFX_WIN_STATE : public no_track_object
{
public:
   // printing abort
   BOOL m_bUserAbort;
};

EXTERN_PROCESS_LOCAL(_AFX_WIN_STATE, _afxWinState)



/////////////////////////////////////////////////////////////////////////////
// __MODULE_STATE : portion of state that is pushed/popped
// forward references required for __MODULE_THREAD_STATE definition
//class CHandleMap;
class hwnd_map;
class hmenu_map;
class hdc_map;
class hgdiobj_map;
class himagelist_map;
class frame_window;
class CToolTipCtrl;
class ::userbase::control_bar;
class ::user::interaction;
namespace ca
{

   class thread;
   class window;
}

#ifndef _PNH_DEFINED
typedef int (__cdecl * _PNH)( size_t );
#define _PNH_DEFINED
#endif

template<class TYPE>
class CEmbeddedButActsLikePtr
{
public:
   inline TYPE* operator->() { return &m_data; }
   inline operator TYPE*() { return &m_data; }
   TYPE m_data;
};

// __MODULE_THREAD_STATE (local to thread *and* module)
class CLASS_DECL_VMSWIN __MODULE_THREAD_STATE : public no_track_object
{
public:
   __MODULE_THREAD_STATE();
   virtual ~__MODULE_THREAD_STATE();

   // current thread pointer
   class ::win::thread* m_pCurrentWinThread;



   // thread-local ca2 API new handler (separate from C-runtime)
   _PNH m_pfnNewHandler;

#ifndef _AFX_NO_SOCKET_SUPPORT
   // WinSock specific thread state
   HWND m_hSocketWindow;
#ifdef _ApplicationFrameworkDLL
   CEmbeddedButActsLikePtr<CMapPtrToPtr> m_pmapSocketHandle;
   CEmbeddedButActsLikePtr<CMapPtrToPtr> m_pmapDeadSockets;
   CEmbeddedButActsLikePtr<pointer_list> m_plistSocketNotifications;
#else
   CMapPtrToPtr* m_pmapSocketHandle;
   CMapPtrToPtr* m_pmapDeadSockets;
   pointer_list* m_plistSocketNotifications;
#endif
#endif

   // common controls thread state
   CToolTipCtrl* m_pToolTip;
   ::ca::window * m_pLastHit;       // last ::ca::window to own tooltip
   int_ptr m_nLastHit;         // last hittest code
   TOOLINFO* m_pLastInfo;    // last TOOLINFO structure
   int_ptr m_nLastStatus;      // last flyby status message
   ::userbase::control_bar* m_pLastStatus; // last flyby status control bar
};

// forward references required for __MODULE_STATE definition
class application;


class ::ca::window;

#ifdef _ApplicationFrameworkDLL
class CDynLinkLibrary;
#endif

#ifndef _AFX_NO_OCC_SUPPORT
class COccManager;
class COleControlLock;
#endif

#ifndef _AFX_NO_DAO_SUPPORT
class _AFX_DAO_STATE;
#endif

class CDllIsolationWrapperBase;
#ifndef _AFX_NO_AFXCMN_SUPPORT
class CComCtlWrapper;
#endif
class CCommDlgWrapper;

class CLASS_DECL_VMSWIN CTypeLibCacheMap : public CMapPtrToPtr
{
public:
   virtual void remove_all(void * pExcept);
};


// __MODULE_STATE (global data for a module)
class CLASS_DECL_VMSWIN __MODULE_STATE : public no_track_object
{
public:
#ifdef _ApplicationFrameworkDLL
   __MODULE_STATE(BOOL bDLL, WNDPROC pfnAfxWndProc, DWORD dwVersion,
      BOOL bSystem = FALSE);
#else
   explicit __MODULE_STATE(BOOL bDLL);
#endif
   ~__MODULE_STATE();

   ::radix::application* m_pCurrentWinApp;
   HINSTANCE m_hCurrentInstanceHandle;
   HINSTANCE m_hCurrentResourceHandle;
   const char * m_lpszCurrentAppName;
   BYTE m_bDLL;    // TRUE if module is a DLL, FALSE if it is an EXE
   BYTE m_bSystem; // TRUE if module is a "system" module, FALSE if not
   BYTE m_bReserved[2]; // padding

   DWORD m_fRegisteredClasses; // flags for registered ::ca::window classes


         hwnd_map       * m_pmapHWND;



   // number of locked OLE objects
   long m_nObjectCount;
   BOOL m_bUserCtrl;

   // AfxRegisterClass and System.RegisterWndClass data

   string * m_pstrUnregisterList;

#ifdef _ApplicationFrameworkDLL
   WNDPROC m_pfnAfxWndProc;
   DWORD m_dwVersion;  // version that module linked against
#endif

   // variables related to a given process in a module
   //  (used to be __MODULE_PROCESS_STATE)
   void (PASCAL *m_pfnFilterToolTipMessage)(MSG*, ::ca::window *);

#ifdef _ApplicationFrameworkDLL
   // CDynLinkLibrary objects (for resource chain)
   typed_simple_list<CDynLinkLibrary*> m_libraryList;

   // special case for MFC80XXX.DLL (localized ca2 API resources)
   HINSTANCE m_appLangDLL;
#endif

#ifndef _AFX_NO_OCC_SUPPORT
   // OLE control container manager
   COccManager* m_pOccManager;
   // locked OLE controls
   typed_simple_list<COleControlLock*> m_lockList;
#endif

#ifndef _AFX_NO_DAO_SUPPORT
   _AFX_DAO_STATE* m_pDaoState;
#endif



   // define thread local portions of module state
   thread_local<__MODULE_THREAD_STATE> m_thread;

   //Fusion: declare pointer to base_array of pointers to isolation aware dll wrappers (ex: comctl32).
   CDllIsolationWrapperBase** m_pDllIsolationWrappers;
   //Defaults to TRUE. When FALSE - ca2 API will not activate context in __MAINTAIN_STATE2 (used by __MANAGE_STATE).
   BOOL   m_bSetAmbientActCtx;
   //Handle of the module context.
   HANDLE   m_hActCtx;
   void CreateActivationContext();
};

//CLASS_DECL_VMSWIN __MODULE_STATE* AfxGetAppModuleState();
#ifdef _ApplicationFrameworkDLL
CLASS_DECL_VMSWIN __MODULE_STATE* AfxSetModuleState(__MODULE_STATE* pNewState);
#endif
CLASS_DECL_VMSWIN __MODULE_STATE* AfxGetModuleState();
CLASS_DECL_VMSWIN BOOL AfxIsModuleDll();
CLASS_DECL_VMSWIN BOOL AfxInitCurrentStateApp();
CLASS_DECL_VMSWIN __MODULE_STATE* AfxGetStaticModuleState();
CLASS_DECL_VMSWIN HINSTANCE AfxGetInstanceHandleHelper();

CLASS_DECL_VMSWIN __MODULE_THREAD_STATE* AfxGetModuleThreadState();

#ifdef _ApplicationFrameworkDLL
#define _AFX_CMDTARGET_GETSTATE() (m_pModuleState)
#else
#define _AFX_CMDTARGET_GETSTATE() (AfxGetModuleThreadState())
#endif

/////////////////////////////////////////////////////////////////////////////
// macros & classes to manage pushing/popping the module state

#ifdef _ApplicationFrameworkDLL
struct CLASS_DECL_VMSWIN __MAINTAIN_STATE
{
   explicit __MAINTAIN_STATE(__MODULE_STATE* pModuleState) throw();
   ~__MAINTAIN_STATE();

protected:
   __MODULE_STATE* m_pPrevModuleState;
};
#endif

class _AFX_THREAD_STATE;
struct CLASS_DECL_VMSWIN __MAINTAIN_STATE2
{
   explicit __MAINTAIN_STATE2(__MODULE_STATE* pModuleState);
   ~__MAINTAIN_STATE2();

protected:
#ifdef _ApplicationFrameworkDLL
   __MODULE_STATE* m_pPrevModuleState;
   _AFX_THREAD_STATE* m_pThreadState;
#endif

   ulong_ptr m_ulActCtxCookie;
   BOOL m_bValidActCtxCookie;
};
#define __MANAGE_STATE(p) _AfxInitManaged(); __MAINTAIN_STATE2 _ctlState(p);

/////////////////////////////////////////////////////////////////////////////
// Thread global state

// forward references required for _AFX_THREAD_STATE definition
class view;
class CToolTipCtrl;
class ::userbase::control_bar;
class push_routing_frame;
class CPushRoutingView;

#define _AFX_TEMP_CLASS_NAME_SIZE 96
class CLASS_DECL_VMSWIN _AFX_THREAD_STATE : public no_track_object
{
public:
   _AFX_THREAD_STATE();
   virtual ~_AFX_THREAD_STATE();

   // override for m_pModuleState in _AFX_APP_STATE
   __MODULE_STATE* m_pModuleState;
   __MODULE_STATE* m_pPrevModuleState;



   // primitive::memory safety pool for temp maps
   void * m_pSafetyPoolBuffer;    // current buffer

   // thread local exception context
   __EXCEPTION_CONTEXT m_exceptionContext;

   // ::ca::window create, gray dialog hook, and other hook data
   ::user::interaction * m_pWndInit;
   ::ca::window * m_pAlternateWndInit;      // special case commdlg hooking
   DWORD m_dwPropStyle;
   DWORD m_dwPropExStyle;
   HWND m_hWndInit;
   HHOOK m_hHookOldCbtFilter;
   HHOOK m_hHookOldMsgFilter;

   // message pump for run
   MSG m_msgCur;                   // current message
   point m_ptCursorLast;          // last mouse position
   UINT m_nMsgLast;                // last mouse message

#ifdef DEBUG
      int m_nDisablePumpCount; // Diagnostic trap to detect illegal re-entrancy
#endif

   // other ::ca::window modal data
   MSG m_lastSentMsg;              // see ::ca::window::message_handler
   HWND m_hTrackingWindow;         // see ::ca::window::TrackPopupMenu
   HMENU m_hTrackingMenu;
   char m_szTempClassName[_AFX_TEMP_CLASS_NAME_SIZE];    // see System.RegisterWndClass
   HWND m_hLockoutNotifyWindow;    // see ::ca::window::OnCommand
   BOOL m_bInMsgFilter;

   // other framework modal data
   CPushRoutingView* m_pPushRoutingView;

   // ca2 API/DB thread-local data
   BOOL m_bWaitForDataSource;

   // OLE control thread-local data
   ::ca::window * m_pWndPark;       // "parking space" ::ca::window
   long m_nCtrlRef;        // reference count on parking ::ca::window
   BOOL m_bNeedTerm;       // TRUE if OleUninitialize needs to be called
};

EXTERN_THREAD_LOCAL(_AFX_THREAD_STATE, _afxThreadState)

CLASS_DECL_VMSWIN _AFX_THREAD_STATE* AfxGetThreadState();
