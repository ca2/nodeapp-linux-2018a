#pragma once



#define LINUX


//#define _POSIX_SOURCE


#define DECL_C
//#define PASCAL


#define _PRE_WIDEN(x) L ## x
#define _WIDEN(x) _PRE_WIDEN(x)

#define NOTHROW throw()
#define THROWS // gnucc null throw statement means that the function can throw any exception


#define _DEBUG // ca2 is in debug - bugs come and go, always



#include <inttypes.h>

#define DWORD     uint32_t
#define BYTE      uint8_t

//#define NULL 0
#define _strcmpi strcasecmp

#include <stdlib.h>


#define __forceinline inline

#if defined(__LP64__) || defined(_LP64)

#if !defined(__LP64__)
#define __LP64__
#endif

#if !defined(_LP64)
#define _LP64
#endif

typedef uint64_t DWORD_PTR;

#else

#define _X86_

typedef uint32_t DWORD_PTR;


#endif


typedef uint8_t byte;


#define __int3264   int32_t

typedef void * PVOID;

#define _strcmpi strcasecmp




#ifndef _DEBUG
#define _DEBUG
#endif

#define PURE = 0





#include "linux_system.h"



#ifndef _DEBUG
#define _DEBUG // ca2 is in debug - bugs come and go, always
#endif


#define __stdcall


#include "bare_operational_system.h"        // Target version control


#include "cross/win/win.h"
