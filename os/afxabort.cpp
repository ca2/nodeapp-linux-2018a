#include "framework.h"


// Note: in separate module so it can be replaced if needed

void CLASS_DECL_VMSLNX AfxAbort()
{
//   TRACE(::radix::trace::category_AppMsg, 0, "AfxAbort called.\n");

   AfxWinTerm();
   abort();
}

/////////////////////////////////////////////////////////////////////////////
