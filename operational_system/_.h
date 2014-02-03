#pragma once


#ifdef _DEBUG
#define DEBUG
#endif


#ifndef LINUX
#define LINUX
#endif


#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#endif // __STRICT_ANSI__


#define __STRICT_ANSI__ 1
#define _ISOC99_SOURCE	1
#define _ISOC11_SOURCE	1
#define _POSIX_SOURCE	2
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 1
#define _XOPEN_SOURCE_EXTENDED 1
#define _LARGEFILE_SOURCE 1
#define _LARGEFILE64_SOURCE 1
#undef _BSD_SOURCE
#define _SVID_SOURCE 1
#define _ATFILE_SOURCE 1
#undef _GNU_SOURCE


#define __WINESRC__


#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread


#ifdef _LP64
#define OS64BIT
#endif









