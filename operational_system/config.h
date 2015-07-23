#pragma once


#ifndef LINUX
#define LINUX
#endif
#define BSD_STYLE_SOCKETS
#define VARIADIC_TEMPLATE



#ifdef _DEBUG
#define DEBUG
#endif



#ifndef LINUX
#define LINUX
#endif


#define __WINESRC__



#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT
#define CLASS_DECL_THREAD __thread


#if defined(_LP64)

#define OS64BIT
#define AMD64


#else

#define _X86_

#endif


#ifdef cplus
#define inline static
#endif

#define static_inline static inline


#define AXIS_FREEIMAGE



