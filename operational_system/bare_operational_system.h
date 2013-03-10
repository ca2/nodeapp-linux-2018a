#pragma once








#include <features.h>


#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
//#include <unistd.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <malloc.h>
#include <inttypes.h>
#define __USE_BSD
#include <sys/time.h>
#undef __USE_BSD
#include <pthread.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>



#define _gmtime64 gmtime
#define _stricmp  stricmp

#ifndef _XSERVER64
#ifndef _XTYPEDEF_XID
#define _XTYPEDEF_XID
typedef unsigned long XID;
#endif
#endif


#pragma once



#if defined(_LP64)

    typedef int64_t int_ptr, *pint_ptr;
    typedef uint64_t uint_ptr, *puint_ptr;

    typedef int64_t long_ptr, *plong_ptr;
    typedef uint64_t ulong_ptr, *pulong_ptr;

#else

    typedef int32_t int_ptr, *pint_ptr;
    typedef uint32_t uint_ptr, *puint_ptr;

    typedef int32_t long_ptr, *plong_ptr;
    typedef unint32_t ulong_ptr, *pulong_ptr;

#endif


//typedef void * HANDLE;
//typedef void * HDC;
typedef unsigned char BYTE;
typedef uint32_t UINT;
//typedef int_ptr WPARAM;
//typedef int_ptr LPARAM;



#define __cdecl
#define _stdcall

// dbg alloc MS VC CRT
#define _NORMAL_BLOCK  1


// Ubuntu apt-get install libx11-dev
#include <X11/Xlib.h>
#include <X11/Xutil.h>


/* Types use for passing & returning polymorphic values */
typedef uint_ptr            WPARAM;
typedef long_ptr            LPARAM;
typedef long_ptr            LRESULT;

#include "osdisplay.h"
#include "oswindow.h"


struct device_context;

struct gdi_object;

typedef XID Window;

/*struct tag_WINDOW;

typedef struct tag_WINDOW WINDOW, * LPWINDOW;

typedef LPWINDOW oswindow;*/

typedef struct device_context * HDC;

typedef struct gdi_object * HGDIOBJ;

union _XEvent;

typedef union _XEvent XEvent;

//typedef Font HFONT;


#define DECL_SPEC_ANY


#ifdef _DEBUG


#define DEBUG


#endif


#include "version_linux.h"


#include "version_cpu.h"


#ifdef __cplusplus


inline void __debug_break() { asm("int $3"); }


#endif


// SIZE_T_MAX is used by the collection classes
#ifndef SIZE_T_MAX
	#define SIZE_T_MAX  UINT_MAX
#endif

// FASTCALL is used for static member functions with little or no params
#ifndef FASTCALL
	#define FASTCALL
#endif

// CDECL and EXPORT are defined in case WINDOWS.H doesn't
#ifndef CDECL
	#define CDECL
#endif

#ifndef EXPORT
	#define EXPORT
#endif

// UNALIGNED is used for unaligned data access (in CArchive mostly)
#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif

// __DEPRECATED is used for functions that should no longer be used
#ifndef __DEPRECATED
#ifdef _AFX_DISABLE_DEPRECATED
	#define __DEPRECATED(_Message)
#else
	#define __DEPRECATED(_Message) __declspec(deprecated(_Message))
#endif
#endif

// _AFX_INSECURE_DEPRECATE is used for deprecated, insecure functions.
#ifndef _AFX_INSECURE_DEPRECATE
#ifdef _AFX_SECURE_NO_DEPRECATE
#define _AFX_INSECURE_DEPRECATE(_Message)
#else
#define _AFX_INSECURE_DEPRECATE(_Message) __declspec(deprecated(_Message))
#endif // _AFX_SECURE_NO_DEPRECATE
#endif // _AFX_INSECURE_DEPRECATE

// _API is used on global public functions
#ifndef _API
	#define _API //__stdcall
#endif

// _OLEAPI is used for some special OLE functions
#ifndef _OLEAPI
	#define _OLEAPI __stdcall
#endif

// __CDECL is used for rare functions taking variable arguments
#ifndef __CDECL
	#define __CDECL __cdecl
#endif

// __EXPORT is used for functions which need to be exported
#ifndef __EXPORT
	#define __EXPORT EXPORT
#endif

#ifndef __STATIC
	#define __STATIC extern
	#define __STATIC_DATA extern __declspec(selectany)
#endif

// The following macros are used to enable export/import

// for data
#ifndef __DATA_EXPORT
	#define __DATA_EXPORT __declspec(dllexport)
#endif
#ifndef __DATA_IMPORT
	#define __DATA_IMPORT __declspec(dllimport)
#endif

// for classes
#ifndef __CLASS_EXPORT
	#define __CLASS_EXPORT __declspec(dllexport)
#endif
#ifndef __CLASS_IMPORT
	#define __CLASS_IMPORT __declspec(dllimport)
#endif

// for global APIs
#ifndef __API_EXPORT
	#define __API_EXPORT __declspec(dllexport)
#endif
#ifndef __API_IMPORT
	#define __API_IMPORT __declspec(dllimport)
#endif

// This macro is used to reduce size requirements of some classes
#ifndef __ALWAYS_VTABLE
#ifndef __NOVTABLE
#if _MSC_VER >= 1100 && !defined(DEBUG)
#define __NOVTABLE __declspec(novtable)
#else
#define __NOVTABLE
#endif
#endif
#endif

// for global data that should be in COMDATs (packaged data)
#ifndef __COMDAT
#define __COMDAT __declspec(selectany)
#endif

// The following macros are used on data declarations/definitions
//  (they are redefined for extension DLLs and the shared MFC DLL)
#define __DATADEF
#define __API CLASS_DECL_ca


// used when building extension DLLs
#ifndef __EXT_DATA
	#define __EXT_DATA
	#define __EXT_DATADEF
	#define __EXT_CLASS
	#define __EXT_API
#endif

// BASED_XXXX macros are provided for backward compatibility
#ifndef BASED_CODE
	#define BASED_CODE
#endif

#ifndef BASED_DEBUG
	#define BASED_DEBUG
#endif

#ifndef BASED_STACK
	#define BASED_STACK
#endif

#ifdef _AFX_DEVBUILD
	#define __IMPL_DATA __DATA_EXPORT
#else
	#define __IMPL_DATA CLASS_DECL_ca
#endif

#define NO_ANSIUNI_ONLY

#define MAX_DWORD_PTR ((dword_ptr)(-1))

#define TRUE 1

#define FALSE 0



#define max(x,y) (((x) > (y)) ? (x) : (y))
#define min(x,y) (((x) < (y)) ? (x) : (y))


#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#define __min(a,b)  (((a) < (b)) ? (a) : (b))



typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef uint32_t    u_int;
typedef unsigned long   u_long;


#define DECL_C


#define _PRE_WIDEN(x) L##x
#define _WIDEN(x) _PRE_WIDEN(x)

#define NOTHROW throw()
#define THROWS // gnucc null throw statement means that the function can throw any exception


#ifdef DEBUG

#define RELEASENOTHROW

#else

#define RELEASENOTHROW throw()

#endif


#define DWORD     uint32_t

#define BYTE      uint8_t

//#define NULL 0

#define _strcmpi strcasecmp


#define __forceinline inline


#if defined(__LP64__) || defined(_LP64)

#if !defined(__LP64__)
#define __LP64__
#endif

#if !defined(_LP64)
#define _LP64
#endif


typedef uint64_t dword_ptr;




#else


#define _X86_


typedef uint32_t dword_ptr;


#define __int3264   int32_t


#endif


typedef uint8_t byte;


typedef void * PVOID;


#define _strcmpi strcasecmp


#define PURE = 0


#define __stdcall


#include "cross/win/win.h"



