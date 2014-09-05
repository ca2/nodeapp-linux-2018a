#pragma once


#include <features.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>


#undef __USE_BSD
#include <string.h>
#define __USE_BSD 1

#include <strings.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include <malloc.h>
#include <inttypes.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <math.h>
#include <assert.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>




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


typedef unsigned char BYTE;
typedef uint32_t UINT;



#define __cdecl
#define _stdcall

#define _NORMAL_BLOCK  1


// Ubuntu apt-get install libx11-dev
// CentOS yum install libX11-devel
// openSUSE zypper install libx11-devel
#include <X11/Xlib.h>
#include <X11/Xutil.h>


typedef uint_ptr                 WPARAM;
typedef long_ptr                 LPARAM;
typedef long_ptr                 LRESULT;
typedef struct oswindow_data *   oswindow;
typedef struct hthread *         HTHREAD;
typedef XID                      Window;

struct device_context;
struct gdi_object;




typedef struct device_context * HDC;

typedef struct gdi_object * HGDIOBJ;

union _XEvent;

typedef union _XEvent XEvent;

#define DECL_SPEC_ANY


#ifdef _DEBUG


#define DEBUG


#endif


#include "version_linux.h"


#include "version_cpu.h"


#ifdef __cplusplus


inline void debug_break() { asm("int $3"); }


#endif


#ifndef SIZE_T_MAX
	#define SIZE_T_MAX  UINT_MAX
#endif

#ifndef CDECL
	#define CDECL
#endif

#ifndef EXPORT
	#define EXPORT
#endif

// UNALIGNED is used for unaligned data access (in carchive mostly)
#if !defined(UNALIGNED)
#if defined(_M_IA64) || defined(_M_AMD64)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif
#endif



// for global data that should be in COMDATs (packaged data)
#ifndef __COMDAT
#define __COMDAT // __declspec(selectany)
#endif


#define NO_ANSIUNI_ONLY

#define MAX_DWORD_PTR ((dword_ptr)(-1))

#define TRUE 1

#define FALSE 0



//#define max(x,y) (((x) > (y)) ? (x) : (y))
//#define min(x,y) (((x) < (y)) ? (x) : (y))


//#define __max(a,b)  (((a) > (b)) ? (a) : (b))
//#define __min(a,b)  (((a) < (b)) ? (a) : (b))



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

#define OS64BIT
#define OSBIT 64
#if !defined(__LP64__)
#define __LP64__
#endif

#if !defined(_LP64)
#define _LP64
#endif


typedef uint64_t dword_ptr;




#else


#define OS32BIT
#define OSBIT 32
#define _X86_


typedef uint32_t dword_ptr;


#define __int3264   int32_t


#endif


//typedef uint8_t byte;


typedef void * PVOID;


#define PURE = 0


#define __stdcall
#define _gmtime64 gmtime


#define stricmp strcasecmp
#define strnicmp strncasecmp


#define _stricmp  stricmp
#define _strnicmp  strnicmp





