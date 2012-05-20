#pragma once

#define LINUX

#ifndef _DEBUG
#define _DEBUG
#endif

#include "linux_system.h"

#include <typeinfo>


typedef std::type_info std_type_info;


#include <inttypes.h>

#if defined(_LP64)






#else


#define _X86_




#endif


typedef int64_t_t int64_t;


#define stricmp strcasecmp









#define WINAPI



#include "operational_system.h"        // Target version control

