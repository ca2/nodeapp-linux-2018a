#pragma once


#define SECURITY_WIN32


#include "ca.h"


#ifdef LINUX
    #define CLASS_DECL_lnx
#endif


string get_error_message(DWORD dwError);

sp(::ca::application)     lnx_instantiate_application(::ca::application * pappSystem, const char * pszId);

/////////////////////////////////////////////////////////////////////////////
// explicit initialization for general purpose classes

//CLASS_DECL_lnx WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = _MFC_VER);
CLASS_DECL_lnx WINBOOL AfxInitialize(WINBOOL bDLL = FALSE, DWORD dwVersion = 0);

/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Debugger hook on specified allocation request - Obsolete
CLASS_DECL_lnx void AfxSetAllocStop(LONG lRequestNumber);

// Return TRUE if primitive::memory is sane or print out what is wrong
CLASS_DECL_lnx bool __check_memory();

// Return TRUE if valid primitive::memory block of nBytes
CLASS_DECL_lnx WINBOOL AfxIsMemoryBlock(const void * p, UINT nBytes,
   LONG* plRequestNumber = NULL);

// helper routines for non-C++ EH implementations
// for THROW_LAST auto-delete backward compatiblity
CLASS_DECL_lnx void AfxThrowLastCleanup();

// other out-of-line helper functions
CLASS_DECL_lnx void AfxTryCleanup();


/////////////////////////////////////////////////////////////////////////////
// Global implementation helpers

// window creation hooking
CLASS_DECL_lnx void AfxHookWindowCreate(::user::interaction * pWnd);
CLASS_DECL_lnx WINBOOL AfxUnhookWindowCreate();
CLASS_DECL_lnx void AfxResetMsgCache();

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
#include "lnx_graphics_path.h"
#include "lnx_graphics.h"
#include "lnx_graphics_object.h"
#include "lnx_bitmap.h"
#include "lnx_dib.h"
#include "lnx_palette.h"
#include "lnx_pen.h"
#include "lnx_font.h"
#include "lnx_brush.h"
#include "lnx_region.h"
//xxx #include "draw_dib.h"
#include "lnx_thread.h"
#include "lnx_window.h"
#include "lnx_os.h"
#include "lnx_port_forward.h"
#include "lnx_copydesk.h"
#include "lnx_crypt.h"
#include "lnx_ip_enum.h"

#define NULL_REF(class) (*((class *) NULL))
// xxx CLASS_DECL_lnx WNDPROC AfxGetAfxWndProc();
#define AfxWndProc (*AfxGetAfxWndProc())

#define LNX_THREAD(pthread) (dynamic_cast < ::lnx::thread * > (dynamic_cast < ::ca::thread * >(pthread)))
#define LNX_WINDOW(pwnd) (dynamic_cast < ::lnx::window * > (((sp(::ca::window))(pwnd)).m_p))
#define LNX_DC(pgraphics) (dynamic_cast < ::lnx::graphics * > (dynamic_cast < ::ca::graphics * > (pgraphics)))
#define SP_DC(pgraphics) (dynamic_cast < ::lnx::graphics * > (( ::ca::graphics * )(pgraphics)))
#define LNX_HDC(pgraphics) ((HDC)*(dynamic_cast < ::lnx::graphics * > (dynamic_cast < ::ca::graphics * > (pgraphics))))
#define SP_HDC(pgraphics) ((HDC)*(dynamic_cast < ::lnx::graphics * > ((::ca::graphics *)(pgraphics))))
#define LNX_DIB(pdib) (dynamic_cast < ::lnx::dib * > (dynamic_cast < ::ca::dib * >(pdib)))

#include "lnx_shell.h"

CLASS_DECL_lnx void __trace_message(const char * lpszPrefix, ::ca::signal_object * pobj);
CLASS_DECL_lnx void __trace_message(const char * lpszPrefix, LPMESSAGE lpmsg);

CLASS_DECL_lnx WINBOOL __cdecl __is_idle_message(::ca::signal_object * pobj);
CLASS_DECL_lnx WINBOOL __cdecl __is_idle_message(MESSAGE* pMsg);


CLASS_DECL_lnx void AfxProcessWndProcException(base_exception*, ::ca::signal_object * pobj);
CLASS_DECL_lnx void __cdecl __pre_translate_message(::ca::signal_object * pobj);


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



int32_t CLASS_DECL_lnx __lnx_main(int32_t argc, char * argv[]);


CLASS_DECL_lnx void vfxThrowFileException(::ca::application * papp, int32_t cause, LONG lOsError, const char * lpszFileName = NULL);
