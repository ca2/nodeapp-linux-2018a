#pragma once


#define SECURITY_WIN32


#include "core.h"


#define CLASS_DECL_LINUX


string get_error_message(DWORD dwError);

sp(base_application)     lnx_instantiate_application(sp(base_application) pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_LINUX WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_LINUX WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_LINUX void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if primitive::memory is sane or print out what is wrong
CLASS_DECL_LINUX bool __check_memory();

// Return TRUE if valid primitive::memory block of nBytes
CLASS_DECL_LINUX WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
   LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_LINUX void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_LINUX void AfxTryCleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
CLASS_DECL_LINUX void AfxHookWindowCreate(sp(::user::interaction) pWnd);
CLASS_DECL_LINUX WINBOOL AfxUnhookWindowCreate();
CLASS_DECL_LINUX void AfxResetMsgcache();

// for backward compatibility to previous versions
#define _AfxHookWindowCreate    AfxHookWindowCreate
#define _AfxUnhookWindowCreate  AfxUnhookWindowCreate

#include "lnx1.h"
#include "implementation.h"
#include "lnx_state.h"
// xxx #include "handle.h"
// xxx #include "lnx_file_find.h"
#include "lnx_dir.h"
// xxx #include "lnx_folder_watch.h"
#include "lnx_factory_exchange.h"
#include "lnx_window_draw.h"
#include "lnx_thread.h"
#include "lnx_window.h"
#include "lnx_os.h"
#include "lnx_port_forward.h"
#include "lnx_copydesk.h"
#include "lnx_crypt.h"
#include "lnx_ip_enum.h"

#define NULL_REF(class) (*((class *) NULL))
// xxx CLASS_DECL_LINUX WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

#define LNX_THREAD(pthread) (dynamic_cast < ::lnx::thread * > (dynamic_cast < ::thread * >(pthread)))
#define LNX_WINDOW(pwnd) (dynamic_cast < ::lnx::window * > (((sp(::window))(pwnd)).m_p))
#define LNX_DC(pgraphics) (dynamic_cast < ::lnx::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics)))
#define SP_DC(pgraphics) (dynamic_cast < ::lnx::graphics * > (( ::draw2d::graphics * )(pgraphics)))
#define LNX_HDC(pgraphics) ((HDC)*(dynamic_cast < ::lnx::graphics * > (dynamic_cast < ::draw2d::graphics * > (pgraphics))))
#define SP_HDC(pgraphics) ((HDC)*(dynamic_cast < ::lnx::graphics * > ((::draw2d::graphics *)(pgraphics))))
#define LNX_DIB(pdib) (dynamic_cast < ::lnx::dib * > (dynamic_cast < ::draw2d::dib * >(pdib)))

#include "lnx_shell.h"

CLASS_DECL_LINUX void __trace_message(const char * lpszPrefix, ::signal_details * pobj);
CLASS_DECL_LINUX void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);

CLASS_DECL_LINUX WINBOOL __cdecl __is_idle_message(::signal_details * pobj);
CLASS_DECL_LINUX WINBOOL __cdecl __is_idle_message(MESSAGE* pMsg);


CLASS_DECL_LINUX void AfxProcessWndProcException(::exception::base*, ::signal_details * pobj);
CLASS_DECL_LINUX void __cdecl __pre_translate_message(::signal_details * pobj);


#include "lnx_application.h"



WINBOOL PeekMessage(
    LPMESSAGE lpMsg,
    oswindow hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax,
    UINT wRemoveMsg);

WINBOOL GetMessage(
    LPMESSAGE lpMsg,
    oswindow hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax);



int32_t CLASS_DECL_LINUX __lnx_main(int32_t argc, char * argv[]);


CLASS_DECL_LINUX void vfxThrowFileException(sp(base_application) papp, int32_t cause, LONG lOsError, const char * lpszFileName = NULL);
