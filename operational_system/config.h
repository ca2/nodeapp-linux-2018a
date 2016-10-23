#pragma once



//#define _GNU_SOURCE
#define _REENTRANT
#define _THREAD_SAFE




#ifndef LINUX
#define LINUX
#endif
#define BSD_STYLE_SOCKETS
#define VARIADIC_TEMPLATE_FORMAT2



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


#define __PLATFORM   "linux"



#ifndef OSBIT
#include "nodeapp/operational_system/bit.h"
#endif


#ifdef cplus
#define inline static
#endif

#define static_inline static inline


#define AXIS_FREEIMAGE


#define FONT_SANS "FreeSans"
#define FONT_SERIF "FreeSerif"
#define FONT_MONO "Ubuntu Monospace"
#define FONT_SANS_EX "Liberation Sans"
#define FONT_SERIF_EX "Liberation Serif"
#define FONT_SANS_FX "Ubuntu"
#define FONT_SERIF_FX "Bitstream Charter"







