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


typedef __int64_t __int64;









#define WINAPI



#include "operational_system.h"        // Target version control

