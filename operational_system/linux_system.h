#pragma once



#if defined(_LP64)

    typedef long int int_ptr, *pint_ptr;
    typedef unsigned long int uint_ptr, *puint_ptr;

    typedef long long long_ptr, *plong_ptr;
    typedef unsigned long long ulong_ptr, *pulong_ptr;

#else

    typedef int int_ptr, *pint_ptr;
    typedef unsigned int uint_ptr, *puint_ptr;

    typedef long long_ptr, *plong_ptr;
    typedef unsigned int ulong_ptr, *pulong_ptr;

#endif


//typedef void * HANDLE;
//typedef void * HDC;
typedef unsigned char BYTE;
typedef unsigned int UINT;
//typedef int_ptr WPARAM;
//typedef int_ptr LPARAM;



#define __cdecl
#define _stdcall

// dbg alloc MS VC CRT
#define _NORMAL_BLOCK  1




