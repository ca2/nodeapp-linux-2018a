#pragma once

#define SECURITY_WIN32

#include "ca.h"
//#include <shlobj.h>
//#include <Security.h>
//#include <shlobj.h>






#ifdef LINUX
    #define CLASS_DECL_LNX
#elif defined(_VMSWIN_DLL)
    #define CLASS_DECL_LNX  _declspec(dllexport)
#else
    #define CLASS_DECL_LNX  _declspec(dllimport)
#endif

string get_error_message(DWORD dwError);

::ca::application *     win_instantiate_application(::ca::application * pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_LNX WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_LNX WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_LNX void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if primitive::memory is sane or print out what is wrong
CLASS_DECL_LNX WINBOOL AfxCheckMemory();

// Return TRUE if valid primitive::memory block of nBytes
CLASS_DECL_LNX WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
   LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_LNX void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_LNX void AfxTryCleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
CLASS_DECL_LNX void AfxHookWindowCreate(::user::interaction * pWnd);
CLASS_DECL_LNX WINBOOL AfxUnhookWindowCreate();
CLASS_DECL_LNX void AfxResetMsgCache();

// for backward compatibility to previous versions
#define _AfxHookWindowCreate    AfxHookWindowCreate
#define _AfxUnhookWindowCreate  AfxUnhookWindowCreate

#include "lnx1.h"
#include "implementation.h"
#include "lnx_state.h"
#include "handle.h"
#include "dir.h"
#include "folder_watch.h"
#include "factory_exchange.h"
#include "window_draw.h"
#include "graphics.h"
#include "graphics_object.h"
#include "bitmap.h"
#include "dib.h"
#include "palette.h"
#include "pen.h"
#include "font.h"
#include "brush.h"
#include "lnx_region.h"
//xxx #include "draw_dib.h"
#include "thread.h"
#include "window.h"
#include "osi.h"
#include "port_forward.h"

#define NULL_REF(class) (*((class *) NULL))
CLASS_DECL_LNX WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

#define WIN_THREAD(pthread) (dynamic_cast < ::win::thread * > (dynamic_cast < ::ca::thread * >(pthread)))
#define WIN_WINDOW(pwnd) (dynamic_cast < ::win::window * > (dynamic_cast < ::ca::window * >(pwnd)))
#define WIN_DC(pgraphics) (dynamic_cast < ::win::graphics * > (dynamic_cast < ::ca::graphics * > (pgraphics)))
#define SP_DC(pgraphics) (dynamic_cast < ::win::graphics * > (( ::ca::graphics * )(pgraphics)))
#define WIN_HDC(pgraphics) ((HDC)*(dynamic_cast < ::win::graphics * > (dynamic_cast < ::ca::graphics * > (pgraphics))))
#define SP_HDC(pgraphics) ((HDC)*(dynamic_cast < ::win::graphics * > ((::ca::graphics *)(pgraphics))))
#define WIN_DIB(pdib) (dynamic_cast < ::win::dib * > (dynamic_cast < ::ca::dib * >(pdib)))



CLASS_DECL_LNX void _AfxTraceMsg(const char * lpszPrefix, gen::signal_object * pobj);
CLASS_DECL_LNX void _AfxTraceMsg(const char * lpszPrefix, LPMSG lpmsg);

CLASS_DECL_LNX WINBOOL __cdecl AfxIsIdleMessage(gen::signal_object * pobj);
CLASS_DECL_LNX WINBOOL __cdecl AfxIsIdleMessage(MSG* pMsg);


CLASS_DECL_LNX void AfxProcessWndProcException(base_exception*, gen::signal_object * pobj);
CLASS_DECL_LNX void __cdecl AfxPreTranslateMessage(gen::signal_object * pobj);


#include "application.h"
