#ifndef __GNUC__
#pragma once
#endif

//#if core_level_3
//#define IMPLEMENT_VISTA_TOOLS
//#endif

#include "lnx.h"
#include "lnx_internal.h"

CLASS_DECL_VMSLNX void vfxThrowFileException(::ca::application * papp, int cause, LONG lOsError, const char * lpszFileName = NULL);
