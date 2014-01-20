#pragma once


#ifdef _DEBUG
#define DEBUG
#endif

#ifndef LINUX
#define LINUX
#endif

#undef _GNU_SOURCE
#define __STRICT_ANSI__ 1
#define _POSIX_SOURCE 1
#define _POSIX_C_SOURCE 200809L
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED 1
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#ifndef _SVID_SOURCE
#define _SVID_SOURCE 1
#endif

#include <features.h>
#define _GNU_SOURCE
#define __USE_GNU 1


#define __WINESRC__


#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread

#ifdef _LP64
#define OS64BIT
#endif









