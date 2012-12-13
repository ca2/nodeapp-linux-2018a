#pragma once


#define SECURITY_WIN32


#include "app/appseed/ca2/ca2.h"
#include "nodeapp/os/lnx.h"


#ifdef WINDOWS
#ifdef _VMSWIN2_DLL
    #define CLASS_DECL_win2  _declspec(dllexport)
#else
    #define CLASS_DECL_win2  _declspec(dllimport)
#endif
#else
    #define CLASS_DECL_win2
#endif



#include "factory_exchange.h"


#include "win2_printer.h"




#include "application.h"
