#pragma once


#ifdef _DEBUG
#define DEBUG
#endif



#ifndef LINUX
#define LINUX
#endif



#define _ISOC99_SOURCE
#define _ISOC11_SOURCE
#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE 1
#endif // _POSIX_SOURCE
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif // _POSIX_C_SOURCE
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE



#undef _SVID_SOURCE
#undef _ATFILE_SOURCE
#undef _BSD_SOURCE
#undef _GNU_SOURCE



#define __WINESRC__



#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread


#ifdef _LP64
#define OS64BIT
#endif


#ifdef cplus
#define inline static
#endif









