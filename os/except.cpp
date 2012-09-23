#include "framework.h"

/////////////////////////////////////////////////////////////////////////////
// __EXCEPTION_CONTEXT (thread global state)

inline __EXCEPTION_CONTEXT* AfxGetExceptionContext()
{
   DWORD lError = GetLastError();
   __EXCEPTION_CONTEXT* pContext = &_afxThreadState->m_exceptionContext;
   SetLastError(lError);
   return pContext;
}

/////////////////////////////////////////////////////////////////////////////
// __EXCEPTION_LINK linked 'jmpbuf' and out-of-line helpers

__EXCEPTION_LINK::__EXCEPTION_LINK()
{
   // setup initial link state
   m_pException = NULL;    // no current exception yet

   // wire into top of exception link stack
   __EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
   m_pLinkPrev = pContext->m_pLinkTop;
   pContext->m_pLinkTop = this;
}


// out-of-line cleanup called from inline __EXCEPTION_LINK destructor
CLASS_DECL_LNX void AfxTryCleanup()
{
   __EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
   __EXCEPTION_LINK* pLinkTop = pContext->m_pLinkTop;

   // delete current exception
   ASSERT(pLinkTop != NULL);
   if (pLinkTop == NULL)
      return;
   if (pLinkTop->m_pException != NULL)
      pLinkTop->m_pException->Delete();

   // remove ourself from the top of the chain
   pContext->m_pLinkTop = pLinkTop->m_pLinkPrev;
}

// special out-of-line implementation of THROW_LAST (for auto-delete behavior)
void CLASS_DECL_LNX AfxThrowLastCleanup()
{
   __EXCEPTION_CONTEXT* pContext = AfxGetExceptionContext();
   __EXCEPTION_LINK* pLinkTop = pContext->m_pLinkTop;

   // check for THROW_LAST inside of auto-delete block
   if (pLinkTop != NULL)
   {
      // make sure current exception does not get auto-deleted
      pLinkTop->m_pException = NULL;
   }

   // THROW_LAST macro will do actual 'throw'
}


