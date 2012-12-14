#pragma once



#ifdef _DEBUG
#define DEBUG
#endif

#define LINUX

#undef _GNU_SOURCE
#define __STRICT_ANSI__ 1
#define _POSIX_SOURCE 1
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 700
#define _XOPEN_SOURCE_EXTENDED
#define _LARGEFILE64_SOURCE
#define _SVID_SOURCE

#include <features.h>
#define _GNU_SOURCE
#define __USE_GNU 1

#define __WINESRC__


#include "bare_operational_system.h"


