#include "framework.h"


// Note: in separate module so it can be replaced if needed

void CLASS_DECL_LINUX __abort()
{
//   TRACE(::ca2::trace::category_AppMsg, 0, "__abort called.\n");

   __lnx_term();
   abort();
}

/////////////////////////////////////////////////////////////////////////////
