#pragma once


#define LINUX


#include "linux_system.h"


#include "bare_operational_system.h"


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



