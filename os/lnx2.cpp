#include "framework.h"

// Global helper functions
 CLASS_DECL_LNX ::radix::application * AfxGetApp()
{ return dynamic_cast < ::radix::application * > (afxCurrentWinApp); }

 CLASS_DECL_LNX HINSTANCE AfxGetInstanceHandle()
   { ASSERT(afxCurrentInstanceHandle != NULL);
      return afxCurrentInstanceHandle; }
 CLASS_DECL_LNX HINSTANCE AfxGetResourceHandle()
   { ASSERT(afxCurrentResourceHandle != NULL);
      return afxCurrentResourceHandle; }
 CLASS_DECL_LNX void AfxSetResourceHandle(HINSTANCE hInstResource)
   { ASSERT(hInstResource != NULL); afxCurrentResourceHandle = hInstResource; }
 CLASS_DECL_LNX const char * AfxGetAppName()
   { ASSERT(afxCurrentAppName != NULL); return afxCurrentAppName; }
 CLASS_DECL_LNX ::user::interaction * AfxGetMainWnd()
{
      ::radix::thread* pThread = dynamic_cast < ::radix::thread * > (::win::get_thread());
      return pThread != NULL ? pThread->GetMainWnd() : NULL;
 }

 CLASS_DECL_LNX WINBOOL AfxGetAmbientActCtx()
   {    return afxAmbientActCtx; }
 CLASS_DECL_LNX void AfxSetAmbientActCtx(WINBOOL bSet)
   {  afxAmbientActCtx = bSet; }



#ifdef _ApplicationFrameworkDLL
// __MAINTAIN_STATE functions
 __MAINTAIN_STATE::__MAINTAIN_STATE(__MODULE_STATE* pNewState)
   {  m_pPrevModuleState = AfxSetModuleState(pNewState); }
#endif

// __MAINTAIN_STATE2 functions
 __MAINTAIN_STATE2::~__MAINTAIN_STATE2()
{
#ifdef _ApplicationFrameworkDLL
   // Not a good place to report errors here, so just be safe
   if(m_pThreadState)
   {
      m_pThreadState->m_pModuleState = m_pPrevModuleState;
   }
#endif

/*   if (m_bValidActCtxCookie)
   {
      WINBOOL bRet;
      bRet = AfxDeactivateActCtx(0, m_ulActCtxCookie);
      ASSERT(bRet == TRUE);
   }*/
}

