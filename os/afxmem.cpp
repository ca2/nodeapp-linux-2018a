#include "framework.h"




_PNH CLASS_DECL_LNX AfxSetNewHandler(_PNH pfnNewHandler)
{
#ifdef _ApplicationFrameworkDLL
   __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   _PNH pfnOldHandler = pState->m_pfnNewHandler;
   pState->m_pfnNewHandler = pfnNewHandler;
   return pfnOldHandler;
#else
   _PNH pfnOldHandler = _afxNewHandler;
   _afxNewHandler = pfnNewHandler;
   return pfnOldHandler;
#endif
}



/////////////////////////////////////////////////////////////////////////////
// stop on a specific primitive::memory request

// Obsolete API
void CLASS_DECL_LNX AfxSetAllocStop(LONG lRequestNumber)
{
   _CrtSetBreakAlloc(lRequestNumber);
}

WINBOOL CLASS_DECL_LNX AfxCheckMemory()
  // check all of primitive::memory (look for primitive::memory tromps)
{
   return _CrtCheckMemory();
}

// -- true if block of exact size, allocated on the heap
// -- set *plRequestNumber to request number (or 0)
WINBOOL CLASS_DECL_LNX AfxIsMemoryBlock(const void * pData, UINT nBytes,
      LONG* plRequestNumber)
{
   return _CrtIsMemoryBlock(pData, nBytes, plRequestNumber, NULL, NULL);
}
