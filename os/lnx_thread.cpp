#include "framework.h"
#include <process.h>    // for _beginthreadex and _endthreadex
#include <ddeml.h>  // for MSGF_DDEMGR

namespace lnx
{
   class thread;
} // namespace lnx

WINBOOL CLASS_DECL_lnx AfxInternalPumpMessage();
LRESULT CLASS_DECL_lnx AfxInternalProcessWndProcException(base_exception*, const MSG* pMsg);
WINBOOL AfxInternalPreTranslateMessage(MSG* pMsg);
WINBOOL AfxInternalIsIdleMessage(MSG* pMsg);
__STATIC void CLASS_DECL_lnx _AfxPreInitDialog(::user::interaction * pWnd, LPRECT lpRectOld, DWORD* pdwStyleOld);
__STATIC void CLASS_DECL_lnx _AfxPostInitDialog(::user::interaction * pWnd, const RECT& rectOld, DWORD dwStyleOld);

namespace ca
{

   thread_startup::thread_startup()
   {
   }

   thread_startup::~thread_startup()
   {
   }

} // namespace ca





/////////////////////////////////////////////////////////////////////////////
// thread entry point

#ifdef _MT

struct _AFX_THREAD_STARTUP : ::ca::thread_startup
{
   // following are "in" parameters to thread startup
   _AFX_THREAD_STATE* pThreadState;    // thread state of parent thread
   ::lnx::thread* pThread;    // thread for new thread
   DWORD dwCreateFlags;    // thread creation flags
   _PNH pfnNewHandler;     // new handler for new thread

   //HANDLE hEvent;          // event triggered after success/non-success
   //HANDLE hEvent2;         // event triggered after thread is resumed

   // strictly "out" -- set after hEvent is triggered
   WINBOOL bError;    // TRUE if error during startup
};

UINT APIENTRY _AfxThreadEntry(void * pParam)
{
   _AFX_THREAD_STARTUP* pStartup = (_AFX_THREAD_STARTUP*)pParam;
   ASSERT(pStartup != NULL);
   ASSERT(pStartup->pThreadState != NULL);
   ASSERT(pStartup->pThread != NULL);
   //ASSERT(pStartup->hEvent != NULL);
   ASSERT(!pStartup->bError);

   ::lnx::thread* pThread = pStartup->pThread;

   pThread->::se_translator::attach();

   try
   {
      // inherit parent's module state
      _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
      pThreadState->m_pModuleState = pStartup->pThreadState->m_pModuleState;

      // set current thread pointer for System.GetThread
      __MODULE_STATE* pModuleState = AfxGetModuleState();
      __MODULE_THREAD_STATE* pState = pModuleState->m_thread;
      pState->m_pCurrentWinThread = pThread;

      // forced initialization of the thread
      AfxInitThread();

      // thread inherits cast's main ::ca::window if not already set
      //if (papp != NULL && GetMainWnd() == NULL)
      {
         // just attach the HWND
         // trans         threadWnd.Attach(pApp->GetMainWnd()->get_handle());
         //GetMainWnd() = pApp->GetMainWnd();
      }
   }
   catch(base_exception *)
   {
      // Note: DELETE_EXCEPTION(e) not required.

      // exception happened during thread initialization!!
      //TRACE(::radix::trace::category_AppMsg, 0, "Warning: Error during thread initialization!\n");

      // set error flag and allow the creating thread to notice the error
//         threadWnd.Detach();
      pStartup->bError = TRUE;
      VERIFY(::SetEvent(pStartup->hEvent));
      AfxEndThread(dynamic_cast < ::radix::application * > (pThread->m_papp), (UINT)-1, FALSE);
      ASSERT(FALSE);  // unreachable
   }

   pThread->thread_entry(pStartup);

   // pStartup is invlaid after the following
   // SetEvent (but hEvent2 is valid)
   HANDLE hEvent2 = pStartup->hEvent2;

   // allow the creating thread to return from thread::CreateThread
   VERIFY(::SetEvent(pStartup->hEvent));

   // wait for thread to be resumed
   VERIFY(::WaitForSingleObject(hEvent2, INFINITE) == WAIT_OBJECT_0);
   ::CloseHandle(hEvent2);



   int n = pThread->m_p->main();

   return pThread->thread_term(n);
}

#endif //_MT

CLASS_DECL_lnx ::lnx::thread * AfxGetThread()
{
   // check for current thread in module thread state
   __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   ::lnx::thread* pThread = pState->m_pCurrentWinThread;
   return pThread;
}


CLASS_DECL_lnx void AfxSetThread(::radix::thread * pthread)
{
   // check for current thread in module thread state
   __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   pState->m_pCurrentWinThread = dynamic_cast < ::lnx::thread * > (pthread->::ca::thread_sp::m_p);
}



CLASS_DECL_lnx MSG * AfxGetCurrentMessage()
{
   _AFX_THREAD_STATE* pState = AfxGetThreadState();
   ASSERT(pState);
   return &(pState->m_msgCur);
}



CLASS_DECL_lnx void AfxInternalProcessWndProcException(base_exception*, gen::signal_object * pobj)
{
   SCAST_PTR(user::lnx::message::base, pbase, pobj);
   if (pbase->m_uiMessage == WM_CREATE)
   {
      pbase->set_lresult(-1);
      return;  // just fail
   }
   else if (pbase->m_uiMessage == WM_PAINT)
   {
      // force validation of ::ca::window to prevent getting WM_PAINT again
      ValidateRect(pbase->m_hwnd, NULL);
      pbase->set_lresult(0);
      return;
   }
   return;   // sensible default for rest of commands
}

CLASS_DECL_lnx void AfxProcessWndProcException(base_exception* e, gen::signal_object * pobj)
{
   ::radix::thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->ProcessWndProcException( e, pobj );
   else
      return AfxInternalProcessWndProcException( e, pobj );
}

void AfxInternalPreTranslateMessage(gen::signal_object * pobj)
{
   try
   {
      SCAST_PTR(user::lnx::message::base, pbase, pobj);

      //   ASSERT_VALID(this);

      ::radix::thread *pThread = dynamic_cast < ::radix::thread * > (::lnx::get_thread());
      if( pThread )
      {
         // if this is a thread-message, short-circuit this function
         if (pbase->m_hwnd == NULL)
         {
            pThread->DispatchThreadMessageEx(pobj);
            if(pobj->m_bRet)
               return;
         }
      }

      // walk from target to main ::ca::window
      ::user::interaction* pMainWnd = pThread->GetMainWnd();
      if(pMainWnd != NULL && pMainWnd->IsWindow())
      {
         pMainWnd->WalkPreTranslateTree(pobj);
         if(pobj->m_bRet)
            return;
      }

      // in case of modeless dialogs, last chance route through main
      //   ::ca::window's accelerator table
      ::ca::window * pWnd = ::lnx::window::from_handle(pbase->m_hwnd);
      if (pMainWnd != NULL)
      {
         if (pWnd != NULL && LNX_WINDOW(pWnd)->GetTopLevelParent() != pMainWnd)
         {
            pMainWnd->pre_translate_message(pobj);
            if(pobj->m_bRet)
               return;
         }
      }
      user::LPWndArray wnda = Sys(pThread->get_app()).frames();
      for(int i = 0; i < wnda.get_count(); i++)
      {
         ::user::interaction * pui = wnda[i];
         try
         {
            if(pui != NULL)
            {
               if(pui->m_pguie != NULL)
               {
                  if(pui->m_pguie != pMainWnd
                  && pui != pMainWnd)
                  {
                     pui->m_pguie->pre_translate_message(pobj);
                     if(pobj->m_bRet)
                        return;
                  }
               }
               else
               {
                  if(pui != pMainWnd)
                  {
                     pui->pre_translate_message(pobj);
                     if(pobj->m_bRet)
                        return;
                  }
               }
            }
         }
         catch(...)
         {
         }
      }
   }
   catch(...)
   {
   }

   // no special processing
}

void __cdecl AfxPreTranslateMessage(gen::signal_object * pobj)
{
   ::radix::thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->pre_translate_message( pobj );
   else
      return AfxInternalPreTranslateMessage( pobj );
}

WINBOOL AfxInternalIsIdleMessage(gen::signal_object * pobj)
{
   SCAST_PTR(user::lnx::message::base, pbase, pobj);
   // Return FALSE if the message just dispatched should _not_
   // cause on_idle to be run.  Messages which do not usually
   // affect the state of the ::fontopus::user interface and happen very
   // often are checked for.

   if(pbase == NULL)
      return FALSE;

   // redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
   if (pbase->m_uiMessage == WM_MOUSEMOVE || pbase->m_uiMessage == WM_NCMOUSEMOVE)
   {
      // mouse move at same position as last mouse move?
      _AFX_THREAD_STATE *pState = AfxGetThreadState();
      point ptCursor;
      App(pobj->get_app()).get_cursor_pos(&ptCursor);
      if (pState->m_ptCursorLast == ptCursor && pbase->m_uiMessage == pState->m_nMsgLast)
         return FALSE;

      pState->m_ptCursorLast = ptCursor;  // remember for next time
      pState->m_nMsgLast = pbase->m_uiMessage;
      return TRUE;
   }

   // WM_PAINT and WM_SYSTIMER (caret blink)
   return pbase->m_uiMessage != WM_PAINT && pbase->m_uiMessage != 0x0118;
}



WINBOOL AfxInternalIsIdleMessage(LPMSG lpmsg)
{
   // Return FALSE if the message just dispatched should _not_
   // cause on_idle to be run.  Messages which do not usually
   // affect the state of the ::fontopus::user interface and happen very
   // often are checked for.

   if(lpmsg == NULL)
      return FALSE;

   // redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
   if (lpmsg->message == WM_MOUSEMOVE || lpmsg->message == WM_NCMOUSEMOVE)
   {
      // mouse move at same position as last mouse move?
      _AFX_THREAD_STATE *pState = AfxGetThreadState();
      if (pState->m_ptCursorLast == lpmsg->pt && lpmsg->message == pState->m_nMsgLast)
         return FALSE;

      pState->m_ptCursorLast = lpmsg->pt;  // remember for next time
      pState->m_nMsgLast = lpmsg->message;
      return TRUE;
   }

   // WM_PAINT and WM_SYSTIMER (caret blink)
   return lpmsg->message != WM_PAINT && lpmsg->message != 0x0118;
}

WINBOOL __cdecl AfxIsIdleMessage(gen::signal_object * pobj)
{
   ::radix::thread *pThread = App(pobj->get_app()).GetThread();
   if( pThread )
      return pThread->is_idle_message(pobj);
   else
      return AfxInternalIsIdleMessage(pobj);
}

WINBOOL __cdecl AfxIsIdleMessage(MSG* pMsg)
{
   lnx::thread * pThread = AfxGetThread();
   if(pThread)
      return pThread->is_idle_message( pMsg );
   else
      return AfxInternalIsIdleMessage( pMsg );
}


/*thread* CLASS_DECL_lnx AfxBeginThread(::ca::application * papp, __THREADPROC pfnThreadProc, LPVOID pParam,
                              int nPriority, UINT nStackSize, DWORD dwCreateFlags,
                              LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
   ASSERT(pfnThreadProc != NULL);

   thread* pThread = DEBUG_NEW thread(papp, pfnThreadProc, pParam);
   ASSERT_VALID(pThread);

   if (!pThread->CreateThread(dwCreateFlags|CREATE_SUSPENDED, nStackSize,
      lpSecurityAttrs))
   {
      pThread->Delete();
      return NULL;
   }
   VERIFY(pThread->SetThreadPriority(nPriority));
   if (!(dwCreateFlags & CREATE_SUSPENDED))
      VERIFY(pThread->ResumeThread() != (DWORD)-1);

   return pThread;
}*/
void CLASS_DECL_lnx AfxEndThread(::radix::application * papp, UINT nExitCode, WINBOOL bDelete)
{
   // remove current thread object from primitive::memory
   __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
   ::lnx::thread* pThread = pState->m_pCurrentWinThread;
   if (pThread != NULL)
   {
      ASSERT_VALID(pThread);
      //ASSERT(pThread != System::smart_pointer < ex2::application *>::m_p);

      if (bDelete)
         pThread->Delete();
      pState->m_pCurrentWinThread = NULL;
   }

   // allow cleanup of any thread local objects
   AfxTermThread(papp);

   // allow C-runtime to cleanup, and exit the thread
   _endthreadex(nExitCode);
}

extern thread_slot_data* _afxThreadData;
void CLASS_DECL_lnx AfxTermThread(::radix::application * papp, HINSTANCE hInstTerm)
{

   try
   {
      // cleanup thread local tooltip window
      if (hInstTerm == NULL)
      {
//         __MODULE_THREAD_STATE* pModuleThreadState = AfxGetModuleThreadState();
      }
   }
   catch( base_exception* e )
   {
      e->Delete();
   }

   try
   {
      // cleanup the rest of the thread local data
      if (_afxThreadData != NULL)
         _afxThreadData->DeleteValues(hInstTerm, FALSE);
   }
   catch( base_exception* e )
   {
      e->Delete();
   }
}


/////////////////////////////////////////////////////////////////////////////
// Global functions for thread initialization and thread cleanup

LRESULT CALLBACK _AfxMsgFilterHook(int code, WPARAM wParam, LPARAM lParam);

void CLASS_DECL_lnx AfxInitThread()
{
   if (!afxContextIsDLL)
   {
      // set message filter proc
      _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
      ASSERT(pThreadState->m_hHookOldMsgFilter == NULL);
      pThreadState->m_hHookOldMsgFilter = ::SetWindowsHookEx(WH_MSGFILTER,
         _AfxMsgFilterHook, NULL, ::GetCurrentThreadId());
   }
}

namespace lnx
{

   void thread::set_p(::radix::thread * p)
   {
      m_p = p;
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread construction


   void thread::construct(__THREADPROC pfnThreadProc, LPVOID pParam)
   {
      m_evFinish.SetEvent();
      if(System.GetThread() != NULL)
      {
         m_pAppThread = AfxGetThread()->m_pAppThread;
      }
      else
      {
         m_pAppThread = NULL;
      }
      m_pfnThreadProc = pfnThreadProc;
      m_pThreadParams = pParam;

      CommonConstruct();
   }

   thread::thread(::ca::application * papp) :
      ca(papp),
      message_window_simple_callback(papp),//,
      //m_evFinish(FALSE, TRUE)
      radix::thread(NULL)
   {
      m_evFinish.SetEvent();
      m_pAppThread = dynamic_cast < ::ca::thread * > (papp);
      m_pThreadParams = NULL;
      m_pfnThreadProc = NULL;

      CommonConstruct();
   }

   void thread::CommonConstruct()
   {
      m_ptimera      = NULL;
      m_puiptra      = NULL;
      m_puiMain      = NULL;
      m_puiActive    = NULL;

      m_pmapHDC      = NULL;
      m_pmapHGDIOBJ  = NULL;

      m_nDisablePumpCount  = 0;

      // no HTHREAD until it is created
      m_hThread = NULL;
      m_nThreadID = 0;

      _AFX_THREAD_STATE* pState = AfxGetThreadState();
      // initialize message pump
      m_nDisablePumpCount = 0;
      pState->m_nMsgLast = WM_NULL;
      System.get_cursor_pos(&(pState->m_ptCursorLast));

      // most threads are deleted when not needed
      m_bAutoDelete = TRUE;
      m_bRun = false;

      m_pmapHDC = new hdc_map;
      m_pmapHGDIOBJ = new hgdiobj_map;
      m_frameList.Construct(offsetof(frame_window, m_pNextFrameWnd));
      m_ptimera = new ::user::interaction::timer_array;
      m_puiptra = new user::LPWndArray;

   }


   thread::~thread()
   {
      if(m_puiptra != NULL)
      {
         CSingleLock sl(&m_mutexUiPtra, TRUE);
         ::user::LPWndArray * puiptra = m_puiptra;
         m_puiptra = NULL;
         for(int i = 0; i < puiptra->get_size(); i++)
         {
            ::user::interaction * pui = puiptra->element_at(i);
            if(pui->m_pthread != NULL)
            {
               try
               {
                  if(LNX_THREAD(pui->m_pthread) == this
                  || LNX_THREAD(pui->m_pthread->m_p) == LNX_THREAD(m_p)
                  || LNX_THREAD(pui->m_pthread) == LNX_THREAD(m_p))
                  {
                     pui->m_pthread = NULL;
                  }
               }
               catch(...)
               {
               }
            }
         }
         sl.Unlock();
      }

      __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
/*      // clean up temp objects
      pState->m_pmapHGDIOBJ->delete_temp();
      pState->m_pmapHDC->delete_temp();
      pState->m_pmapHWND->delete_temp();*/

      for(int i = 0; i < m_captraDeletePool.get_count(); i++)
      {
         try
         {
            ::ca::ca * pca = m_captraDeletePool[i];
            if(dynamic_cast < ::ca::application * > (pca) == m_papp)
            {
               m_papp = NULL;
            }
         }
         catch(...)
         {
         }
      }

      // free thread object
      if (m_hThread != NULL)
         CloseHandle(m_hThread);

      // cleanup module state
      if (pState->m_pCurrentWinThread == this)
         pState->m_pCurrentWinThread = NULL;

      window::DeleteTempMap();
      m_pmapHDC->delete_temp();
      m_pmapHGDIOBJ->delete_temp();

      try
      {
         // cleanup temp/permanent maps (just the maps themselves)
         delete m_pmapHDC;
         delete m_pmapHGDIOBJ;
      }
      catch(...)
      {
      }

   }

   void * thread::get_os_data()
   {
      return m_hThread;
   }

   int_ptr thread::get_os_int()
   {
      return m_nThreadID;
   }

   bool thread::Begin(int nPriority, UINT nStackSize, DWORD dwCreateFlags,
      LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      if (!CreateThread(dwCreateFlags|CREATE_SUSPENDED, nStackSize,
         lpSecurityAttrs))
      {
         Delete();
         return false;
      }
      VERIFY(SetThreadPriority(nPriority));
      if (!(dwCreateFlags & CREATE_SUSPENDED))
      {
         ENSURE(ResumeThread() != (DWORD)-1);
      }
      return true;
   }

   void thread::on_delete(::ca::ca * p)
   {
   }



   ::user::interaction * thread::SetMainWnd(::user::interaction * pui)
   {
      ::user::interaction * puiPrevious = m_puiMain;
      m_puiMain  = pui;
      return puiPrevious;
   }

   void thread::add(::user::interaction * pui)
   {
      CSingleLock sl(&m_mutexUiPtra, TRUE);
      m_puiptra->add(pui);
   }

   void thread::remove(::user::interaction * pui)
   {
      if(pui == NULL)
         return;
      if(GetMainWnd() == pui)
      {
         SetMainWnd(NULL);
      }
      CSingleLock sl(&m_mutexUiPtra, TRUE);
      if(m_puiptra != NULL)
      {
         m_puiptra->remove(pui);
         m_puiptra->remove(pui->m_pguie);
         m_puiptra->remove(pui->m_pimpl);
      }
      sl.Unlock();
      if(m_ptimera != NULL)
      {
         m_ptimera->unset(pui);
         m_ptimera->unset(pui->m_pguie);
         m_ptimera->unset(pui->m_pimpl);
      }

      try
      {
         if(LNX_THREAD(pui->m_pthread) == this)
         {
            pui->m_pthread = NULL;
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pimpl != NULL && pui->m_pimpl != pui)
         {
            if(LNX_THREAD(pui->m_pimpl->m_pthread) == this)
            {
               pui->m_pimpl->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
      try
      {
         if(pui->m_pguie != NULL && pui->m_pguie != pui)
         {
            if(LNX_THREAD(pui->m_pguie->m_pthread) == this)
            {
               pui->m_pguie->m_pthread = NULL;
            }
         }
      }
      catch(...)
      {
      }
   }

   int thread::get_ui_count()
   {
      CSingleLock sl(&m_mutexUiPtra, TRUE);
      return m_puiptra->get_count();
   }

   ::user::interaction * thread::get_ui(int iIndex)
   {
      CSingleLock sl(&m_mutexUiPtra, TRUE);
      return m_puiptra->element_at(iIndex);
   }

   void thread::set_timer(::user::interaction * pui, uint_ptr nIDEvent, UINT nEllapse)
   {
      if(m_spwindowMessage.is_null())
      {
         return;
      }
      m_ptimera->set(pui, nIDEvent, nEllapse);
      CSingleLock sl(&m_ptimera->m_mutex, TRUE);
      int iMin = 100;
      for(int i = 0; i < m_ptimera->m_timera.get_count(); i++)
      {
         if(m_ptimera->m_timera.element_at(i).m_uiElapse < natural(iMin))
         {
            iMin = m_ptimera->m_timera.element_at(i).m_uiElapse;
         }
      }
      sl.Unlock();
      if(m_spwindowMessage->IsWindow())
      {
         m_spwindowMessage->SetTimer((uint_ptr)-2, iMin, NULL);
      }
   }

   void thread::unset_timer(::user::interaction * pui, uint_ptr nIDEvent)
   {
      m_ptimera->unset(pui, nIDEvent);
   }

   void thread::set_auto_delete(bool bAutoDelete)
   {
      m_bAutoDelete = bAutoDelete;
   }

   void thread::set_run(bool bRun)
   {
      m_bRun = bRun;
   }

   CEvent & thread::get_finish_event()
   {
      return m_evFinish;
   }

   bool thread::get_run()
   {
      return m_bRun;
   }

   ::ca::thread * thread::get_app_thread()
   {
      return m_pAppThread;
   }

   ::user::interaction * thread::get_active_ui()
   {
      return m_puiActive;
   }

   ::user::interaction * thread::set_active_ui(::user::interaction * pui)
   {
      ::user::interaction * puiPrevious = m_puiActive;
      m_puiActive = pui;
      return puiPrevious;
   }

   void thread::step_timer()
   {
      if(m_ptimera == NULL)
         return;
      m_ptimera->check();
   }

   WINBOOL thread::CreateThread(DWORD dwCreateFlags, UINT nStackSize,
      LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
#ifndef _MT
   dwCreateFlags;
   nStackSize;
   lpSecurityAttrs;

   return FALSE;
#else
   ENSURE(m_hThread == NULL);  // already created?

   // setup startup structure for thread initialization
   _AFX_THREAD_STARTUP startup;
   startup.bError = FALSE;
   startup.pfnNewHandler = NULL;
   //memset(&startup, 0, sizeof(startup));
   startup.pThreadState = AfxGetThreadState();
   startup.pThread = this;
   startup.m_pthread = NULL;
   startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
 startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
   startup.dwCreateFlags = dwCreateFlags;
   if (startup.hEvent == NULL || startup.hEvent2 == NULL)
   {
      TRACE(::radix::trace::category_AppMsg, 0, "Warning: CreateEvent failed in thread::CreateThread.\n");
      if (startup.hEvent != NULL)
         ::CloseHandle(startup.hEvent);
      if (startup.hEvent2 != NULL)
         ::CloseHandle(startup.hEvent2);
      return FALSE;
   }

#ifdef _WIN32
//   m_thread = ::CreateThread(NULL, 0, StartThread, this, 0, &m_dwThreadId);
   // create the thread (it may or may not start to run)
   m_hThread = (HANDLE)(ulong_ptr)_beginthreadex(lpSecurityAttrs, nStackSize,
      &_AfxThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
#else
   pthread_attr_t attr;

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
   if (pthread_create(&m_thread,&attr, StartThread,this) == -1)
   {
      perror("thread: create failed");
      SetRunning(false);
   }
//   pthread_attr_destroy(&attr);
#endif

   if (m_hThread == NULL)
      return FALSE;

   // start the thread just for ca2 API initialization
   VERIFY(ResumeThread() != (DWORD)-1);
   VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
   ::CloseHandle(startup.hEvent);

   // if created suspended, suspend it until resume thread wakes it up
   if (dwCreateFlags & CREATE_SUSPENDED)
      VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);

   // if error during startup, shut things down
   if (startup.bError)
   {
      VERIFY(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0);
      ::CloseHandle(m_hThread);
      m_hThread = NULL;
      ::CloseHandle(startup.hEvent2);
      return FALSE;
   }

   // allow thread to continue, once resumed (it may already be resumed)
   ::SetEvent(startup.hEvent2);
   return TRUE;
#endif //!_MT
}

void thread::Delete()
{

   // delete thread if it is auto-deleting
   if(m_bAutoDelete)
   {
      if(m_pappDelete != NULL)
         delete m_pappDelete;
      m_evFinish.SetEvent();
      ::radix::thread * pthread = dynamic_cast < ::radix::thread * > (m_p);
      if(pthread->m_peventReady != NULL)
      {
         ::SetEvent((HANDLE) pthread->m_peventReady);
      }
      if(m_peventReady != NULL)
      {
         ::SetEvent((HANDLE) m_peventReady);
      }
      //pthread->::ca::smart_pointer < ::ca::thread >::m_p = NULL;
      gen::del(m_p);
//      delete_this();
   }
   else
   {
      m_hThread = NULL;
      m_evFinish.SetEvent();
   }
}

   /////////////////////////////////////////////////////////////////////////////
   // thread default implementation

   bool thread::PreInitInstance()
 {
   return true;
}

   bool thread::initialize_instance()
   {
      ASSERT_VALID(this);

      return true;   // by default enter run loop
   }

   // main running routine until thread exits
   int thread::run()
   {

      ASSERT_VALID(this);
//      _AFX_THREAD_STATE* pState = AfxGetThreadState();

      // for tracking the idle time state
      WINBOOL bIdle = TRUE;
      LONG lIdleCount = 0;
      ::radix::application * pappThis1 = dynamic_cast < ::radix::application * > (this);
      ::radix::application * pappThis2 = dynamic_cast < ::radix::application * > (m_p);

      // acquire and dispatch messages until a WM_QUIT message is received.
      MSG msg;
      while(m_bRun)
      {
         // phase1: check to see if we can do idle work
         while (bIdle &&
            !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
         {
            // call on_idle while in bIdle state
            if (!on_idle(lIdleCount++))
               bIdle = FALSE; // assume "no idle" state
            step_timer();
            m_p->m_dwAlive = m_dwAlive = ::GetTickCount();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = m_dwAlive;
            }
         }

         // phase2: pump messages while available
         do
         {

            // pump message, but quit on WM_QUIT
            if (!pump_message())
            {
               try
               {
                  return exit();
               }
               catch(...)
               {
                  return -1;
               }
            }

            // reset "no idle" state after pumping "normal" message
            //if (is_idle_message(&m_msgCur))
            if (is_idle_message(&msg))
            {
               bIdle = TRUE;
               lIdleCount = 0;
            }

            step_timer();
            m_p->m_dwAlive = m_dwAlive = ::GetTickCount();
            if(pappThis1 != NULL)
            {
               pappThis1->m_dwAlive = m_dwAlive;
            }
            if(pappThis2 != NULL)
            {
               pappThis2->m_dwAlive = m_dwAlive;
            }
         }
         while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE) != FALSE);

      }

      return 0;
   }

   WINBOOL thread::is_idle_message(gen::signal_object * pobj)
   {
      return AfxInternalIsIdleMessage(pobj);
   }

   WINBOOL thread::is_idle_message(LPMSG lpmsg)
   {
      return AfxInternalIsIdleMessage(lpmsg);
   }

   void thread::delete_temp()
   {

      m_pmapHGDIOBJ->delete_temp();
      m_pmapHDC->delete_temp();
      window::DeleteTempMap();

   }


   int thread::exit_instance()
    {
      ASSERT_VALID(this);


      try
      {
   #ifdef DEBUG
         // Check for missing LockTempMap calls
         if(m_nTempMapLock != 0)
         {
            TRACE(::radix::trace::category_AppMsg, 0, "Warning: Temp ::collection::map lock count non-zero (%ld).\n", m_nTempMapLock);
         }
         LockTempMaps();
         UnlockTempMaps(-1);
   #endif
      }
      catch(...)
      {
      }


      try
      {
         if(m_puiptra != NULL)
         {
            CSingleLock sl(&m_mutexUiPtra, TRUE);
            ::user::LPWndArray * puiptra = m_puiptra;
            m_puiptra = NULL;
            for(int i = 0; i < puiptra->get_size(); i++)
            {
               ::user::interaction * pui = puiptra->element_at(i);
               if(pui->m_pthread != NULL)
               {
                  if(LNX_THREAD(pui->m_pthread) == this
                  || LNX_THREAD(pui->m_pthread->m_p) == LNX_THREAD(m_p)
                  || LNX_THREAD(pui->m_pthread) == LNX_THREAD(m_p))
                  {
                     pui->m_pthread = NULL;
                  }
               }
            }
            delete puiptra;
            sl.Unlock();
         }
      }
      catch(...)
      {
      }

      try
      {
         ::user::interaction::timer_array * ptimera = m_ptimera;
         m_ptimera = NULL;
         delete ptimera;
      }
      catch(...)
      {
      }



      int nResult = (int)AfxGetCurrentMessage()->wParam;  // returns the value from PostQuitMessage
      return nResult;
   }

   WINBOOL thread::on_idle(LONG lCount)
   {
      ASSERT_VALID(this);

   #if defined(DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
      // check ca2 API's allocator (before idle)
      if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
         ASSERT(AfxCheckMemory());
   #endif

      if(lCount <= 0 && m_puiptra != NULL)
      {
         for(int i = 0; i < m_puiptra->get_count(); i++)
         {
            ::user::interaction* pui = m_puiptra->element_at(i);
            try
            {
               if (pui != NULL && pui->IsWindowVisible())
               {
                  /*AfxCallWndProc(pMainWnd, pMainWnd->get_handle(),
                     WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
                  pui->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
               /*   pui->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
                     (WPARAM)TRUE, 0, TRUE, TRUE);*/
               }
            }
            catch(...)
            {
            }
         }


         // send WM_IDLEUPDATECMDUI to the main window
         /*
         ::user::interaction* pMainWnd = GetMainWnd();
         if (pMainWnd != NULL && pMainWnd->IsWindowVisible())
         {
            /*AfxCallWndProc(pMainWnd, pMainWnd->get_handle(),
               WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
           /* pMainWnd->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
            pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
               (WPARAM)TRUE, 0, TRUE, TRUE);
         }
         */
         // send WM_IDLEUPDATECMDUI to all frame windows
         /* linux __MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
         frame_window* pFrameWnd = pState->m_frameList;
         while (pFrameWnd != NULL)
         {
            if (pFrameWnd->get_handle() != NULL && pFrameWnd != pMainWnd)
            {
               if (pFrameWnd->m_nShowDelay == SW_HIDE)
                  pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
               if (pFrameWnd->IsWindowVisible() ||
                  pFrameWnd->m_nShowDelay >= 0)
               {
                  AfxCallWndProc(pFrameWnd, pFrameWnd->get_handle(),
                     WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
                  pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
                     (WPARAM)TRUE, 0, TRUE, TRUE);
               }
               if (pFrameWnd->m_nShowDelay > SW_HIDE)
                  pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
               pFrameWnd->m_nShowDelay = -1;
            }
            pFrameWnd = pFrameWnd->m_pNextFrameWnd;
         }*/
      }
      else if (lCount >= 0)
      {
/*         __MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
         if (pState->m_nTempMapLock == 0)
         {
            // free temp maps, OLE DLLs, etc.
            AfxLockTempMaps(dynamic_cast < ::radix::application * > (m_p->m_papp));
            AfxUnlockTempMaps(dynamic_cast < ::radix::application * > (m_p->m_papp));
         }*/
      }

   #if defined(DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
      // check ca2 API's allocator (after idle)
      if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
         ASSERT(AfxCheckMemory());
   #endif

      return lCount < 0;  // nothing more to do if lCount >= 0
   }

   ::user::lnx::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      UNREFERENCED_PARAMETER(uiMessage);
      UNREFERENCED_PARAMETER(uiCode);
      return ::user::lnx::message::PrototypeNone;
   }


   void thread::DispatchThreadMessageEx(gen::signal_object * pobj)
   {
      SCAST_PTR(user::lnx::message::base, pbase, pobj);
      if(pbase->m_uiMessage == WM_APP + 1984 && pbase->m_wparam == 77)
      {
         gen::scoped_ptr < message > spmessage(pbase->m_lparam);
         spmessage->send();
         pbase->m_bRet = true;
         return;
      }
   /*   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
      const __MSGMAP_ENTRY* lpEntry;

      for (/* pMessageMap already init'ed *//*; pMessageMap->pfnGetBaseMap != NULL;
         pMessageMap = (*pMessageMap->pfnGetBaseMap)())
      {
         // Note: catch not so common but fatal mistake!!
         //       // BEGIN_MESSAGE_MAP(CMyThread, CMyThread)

         ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
         if (pMsg->message < 0xC000)
         {
            // constant window message
            if ((lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries,
               pMsg->message, 0, 0)) != NULL)
               goto LDispatch;
         }
         else
         {
            // registered windows message
            lpEntry = pMessageMap->lpEntries;
            while ((lpEntry = AfxFindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
            {
               UINT* pnID = (UINT*)(lpEntry->nSig);
               ASSERT(*pnID >= 0xC000);
                  // must be successfully registered
               if (*pnID == pMsg->message)
                  goto LDispatch;
               lpEntry++;      // keep looking past this one
            }
         }
      }
      return FALSE;

   LDispatch:
      union MessageMapFunctions mmf;
      mmf.pfn = lpEntry->pfn;

      // always posted, so return value is meaningless

      (this->*mmf.pfn_THREAD)(pMsg->wParam, pMsg->lParam);*/

      LRESULT lresult;
      SignalPtrArray signalptra;
      m_signala.GetSignalsByMessage(signalptra, pbase->m_uiMessage, 0, 0);
      for(int i = 0; i < signalptra.get_size(); i++)
      {
         Signal & signal = *signalptra[i];
         gen::signal * psignal = signal.m_psignal;
         ::user::lnx::message::e_prototype eprototype = signal.m_eprototype;
         if(eprototype == ::user::lnx::message::PrototypeNone)
         {
            //::user::lnx::message::base base(get_app());
            pbase->m_psignal = psignal;
            lresult = 0;
            //base.set(pmsg->message, pmsg->wParam, pmsg->lParam, lresult);
            psignal->emit(pbase);
            if(pbase->m_bRet)
               return;
         }
         break;
      }
      pbase->m_bRet = true;
   }

   void thread::pre_translate_message(gen::signal_object * pobj)
   {
      ASSERT_VALID(this);
      return AfxInternalPreTranslateMessage(pobj);
   }

   void thread::ProcessWndProcException(base_exception* e, gen::signal_object * pobj)
   {
      return AfxInternalProcessWndProcException(e, pobj);
   }

   __STATIC inline WINBOOL IsEnterKey(gen::signal_object * pobj)
   {
      SCAST_PTR(user::lnx::message::base, pbase, pobj);
      return pbase->m_uiMessage == WM_KEYDOWN && pbase->m_wparam == VK_RETURN;
   }

   __STATIC inline WINBOOL IsButtonUp(gen::signal_object * pobj)
   {
      SCAST_PTR(user::lnx::message::base, pbase, pobj);
      return pbase->m_uiMessage == WM_LBUTTONUP;
   }

   void thread::ProcessMessageFilter(int code, gen::signal_object * pobj)
   {

      if(pobj == NULL)
         return;   // not handled

      SCAST_PTR(::user::lnx::message::base, pbase, pobj);

      frame_window* pTopFrameWnd;
      ::user::interaction* pMainWnd;
      ::user::interaction* pMsgWnd;
      switch (code)
      {
      case MSGF_DDEMGR:
         // Unlike other WH_MSGFILTER codes, MSGF_DDEMGR should
         //  never call the next hook.
         // By returning FALSE, the message will be dispatched
         //  instead (the default behavior).
         return;

      case MSGF_MENU:
         pMsgWnd = window::from_handle(pbase->m_hwnd);
         if (pMsgWnd != NULL)
         {
            pTopFrameWnd = pMsgWnd->GetTopLevelFrame();
            if (pTopFrameWnd != NULL && pTopFrameWnd->IsTracking() &&
               pTopFrameWnd->m_bHelpMode)
            {
               pMainWnd = AfxGetMainWnd();
               if ((GetMainWnd() != NULL) && (IsEnterKey(pbase) || IsButtonUp(pbase)))
               {
                  pMainWnd->SendMessage(WM_COMMAND, ID_HELP);
                  pbase->m_bRet = true;
                  return;
               }
            }
         }
         // fall through...

      case MSGF_DIALOGBOX:    // handles message boxes as well.
         pMainWnd = AfxGetMainWnd();
         if (code == MSGF_DIALOGBOX && m_puiActive != NULL &&
            pbase->m_uiMessage >= WM_KEYFIRST && pbase->m_uiMessage <= WM_KEYLAST)
         {
            // need to translate messages for the in-place container
            _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
            ENSURE(pThreadState);

            if (pThreadState->m_bInMsgFilter)
               return;
            pThreadState->m_bInMsgFilter = TRUE;    // avoid reentering this code
            if (m_puiActive->IsWindowEnabled())
            {
               pre_translate_message(pobj);
               if(pobj->m_bRet)
               {
                  pThreadState->m_bInMsgFilter = FALSE;
                  return;
               }
            }
            pThreadState->m_bInMsgFilter = FALSE;    // ok again
         }
         break;
      }
      // default to not handled
   }

   /////////////////////////////////////////////////////////////////////////////
   // Access to GetMainWnd() & m_pActiveWnd

   ::user::interaction* thread::GetMainWnd()
   {
      if (m_puiActive != NULL)
         return m_puiActive;    // probably in-place active

      // when not inplace active, just return main window
      if (m_puiMain != NULL)
         return m_puiMain;

      return System.get_active_guie();
   }

   /////////////////////////////////////////////////////////////////////////////
   // thread implementation helpers

   WINBOOL thread::pump_message()
   {
      try
      {
         MSG msg;
         if(!::GetMessage(&msg, NULL, NULL, NULL))
         {
            TRACE(::radix::trace::category_AppMsg, 1, "thread::pump_message - Received WM_QUIT.\n");
            m_nDisablePumpCount++; // application must die
            // Note: prevents calling message loop things in 'exit_instance'
            // will never be decremented
            return FALSE;
         }

         if(m_nDisablePumpCount != 0)
         {
            TRACE(::radix::trace::category_AppMsg, 0, "Error: thread::pump_message called when not permitted.\n");
            ASSERT(FALSE);
         }

         _AfxTraceMsg("pump_message", &msg);

         if(msg.message != WM_KICKIDLE)
         {
            {
               ::ca::smart_pointer < ::user::lnx::message::base > spbase;

               spbase(get_base(&msg));

               if(m_p != NULL)
               {
                  m_p->pre_translate_message(spbase);
                  if(spbase->m_bRet)
                     return TRUE;
               }

               System.pre_translate_message(spbase);
               if(spbase->m_bRet)
                  return TRUE;

               if(!Application.is_system())
               {
                  Application.pre_translate_message(spbase);
                  if(spbase->m_bRet)
                     return TRUE;
               }

               AfxPreTranslateMessage(spbase);
               if(spbase->m_bRet)
                  return TRUE;

               spbase.destroy();
            }
            {
               ::TranslateMessage(&msg);
               ::DispatchMessage(&msg);
            }
         }
         return TRUE;
      }
      catch(const ::ca::exception & e)
      {
         if(on_run_exception((::ca::exception &) e))
            return TRUE;
         // get_app() may be it self, it is ok...
         if(App(get_app()).final_handle_exception((::ca::exception & ) e))
               return TRUE;
         return FALSE;
      }
   }


   /////////////////////////////////////////////////////////////////////////////
   // thread diagnostics

#ifdef DEBUG
   void thread::assert_valid() const
   {
      command_target::assert_valid();
   }
   void thread::dump(dump_context & dumpcontext) const
  {
   command_target::dump(dumpcontext);
   _AFX_THREAD_STATE *pState = AfxGetThreadState();

   dumpcontext << "m_pThreadParams = " << m_pThreadParams;
   dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
   dumpcontext << "\nm_bAutoDelete = " << m_bAutoDelete;
   dumpcontext << "\nm_hThread = " << (void *)m_hThread;
   dumpcontext << "\nm_nThreadID = " << m_nThreadID;
   dumpcontext << "\nm_nDisablePumpCount = " << pState->m_nDisablePumpCount;
   if (AfxGetThread() == this)
      dumpcontext << "\nm_pMainWnd = " << m_puiMain;

   dumpcontext << "\nm_msgCur = {";
   dumpcontext << "\n\thwnd = " << (void *)pState->m_msgCur.hwnd;
   dumpcontext << "\n\tmessage = " << (UINT)pState->m_msgCur.message;
   dumpcontext << "\n\twParam = " << (UINT)pState->m_msgCur.wParam;
   dumpcontext << "\n\tlParam = " << (void *)pState->m_msgCur.lParam;
   dumpcontext << "\n\ttime = " << pState->m_msgCur.time;
   dumpcontext << "\n\tpt = " << point(pState->m_msgCur.pt);
   dumpcontext << "\n}";

   dumpcontext << "\nm_pThreadParams = " << m_pThreadParams;
   dumpcontext << "\nm_pfnThreadProc = " << (void *)m_pfnThreadProc;
   dumpcontext << "\nm_ptCursorLast = " << pState->m_ptCursorLast;
   dumpcontext << "\nm_nMsgLast = " << pState->m_nMsgLast;

   dumpcontext << "\n";
}
#endif

   bool thread::post_message(::user::interaction * pguie, UINT uiMessage, WPARAM wparam, LPARAM lparam)
   {
      if(m_hThread == NULL)
         return false;
      message * pmessage = new message;
      pmessage->m_pguie       = pguie;
      pmessage->m_uiMessage   = uiMessage;
      pmessage->m_wparam      = wparam;
      pmessage->m_lparam      = lparam;
      return PostThreadMessage(WM_APP + 1984, 77, (LPARAM) pmessage) != FALSE;
   }


   bool thread::on_run_exception(::ca::exception & e)
   {
      UNREFERENCED_PARAMETER(e);
      return false;
   }


   void thread::message_handler(gen::signal_object * pobj)
   {
      SCAST_PTR(user::lnx::message::base, pbase, pobj);
      // special message which identifies the window as using AfxWndProc
      if(pbase->m_uiMessage == WM_QUERYAFXWNDPROC)
      {
         pbase->set_lresult(0);
         return;
      }

      // all other messages route through message ::collection::map
      ::ca::window * pwindow = ::lnx::window::FromHandlePermanent(pbase->m_hwnd);

      ASSERT(pwindow == NULL || LNX_WINDOW(pwindow)->get_handle() == pbase->m_hwnd);

      if(pwindow == NULL || LNX_WINDOW(pwindow)->get_handle() != pbase->m_hwnd)
      {
         pbase->set_lresult(::DefWindowProc(pbase->m_hwnd, pbase->m_uiMessage, pbase->m_wparam, pbase->m_lparam));
         return;
      }

      _AFX_THREAD_STATE* pThreadState = _afxThreadState.get_data();
      MSG oldState = pThreadState->m_lastSentMsg;   // save for nesting
      pThreadState->m_lastSentMsg.hwnd       = pbase->m_hwnd;
      pThreadState->m_lastSentMsg.message    = pbase->m_uiMessage;
      pThreadState->m_lastSentMsg.wParam     = pbase->m_wparam;
      pThreadState->m_lastSentMsg.lParam     = pbase->m_lparam;

      _AfxTraceMsg("message_handler", pobj);

      // Catch exceptions thrown outside the scope of a callback
      // in debug builds and warn the ::fontopus::user.
      try
      {

         // special case for WM_INITDIALOG
         rect rectOld;
         DWORD dwStyle = 0;
         if(pbase->m_uiMessage == WM_INITDIALOG)
            _AfxPreInitDialog(pwindow, &rectOld, &dwStyle);

         // delegate to object's message_handler
         if(pwindow->m_pguie != NULL && pwindow->m_pguie != pwindow)
         {
            pwindow->m_pguie->message_handler(pobj);
         }
         else
         {
            pwindow->message_handler(pobj);
         }

         // more special case for WM_INITDIALOG
         if(pbase->m_uiMessage == WM_INITDIALOG)
            _AfxPostInitDialog(pwindow, rectOld, dwStyle);
      }
      catch(const ::ca::exception & e)
      {
         if(App(get_app()).on_run_exception((::ca::exception &) e))
            goto run;
         if(App(get_app()).final_handle_exception((::ca::exception &) e))
            goto run;
         AfxPostQuitMessage(-1);
         pbase->set_lresult(-1);
         return;
      }
      catch(base_exception * pe)
      {
         AfxProcessWndProcException(pe, pbase);
         TRACE(::radix::trace::category_AppMsg, 0, "Warning: Uncaught exception in message_handler (returning %ld).\n", pbase->get_lresult());
         pe->Delete();
      }
   run:
      pThreadState->m_lastSentMsg = oldState;
   }


   thread::operator HANDLE() const
   { return this == NULL ? NULL : m_hThread; }
   WINBOOL thread::SetThreadPriority(int nPriority)
   { ASSERT(m_hThread != NULL); return ::SetThreadPriority(m_hThread, nPriority); }
   int thread::GetThreadPriority()
   { ASSERT(m_hThread != NULL); return ::GetThreadPriority(m_hThread); }
   DWORD thread::ResumeThread()
   { ASSERT(m_hThread != NULL); return ::ResumeThread(m_hThread); }
   DWORD thread::SuspendThread()
   { ASSERT(m_hThread != NULL); return ::SuspendThread(m_hThread); }
   WINBOOL thread::PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam)
   { ASSERT(m_hThread != NULL); return ::PostThreadMessage(m_nThreadID, message, wParam, lParam); }

   void thread::set_os_data(void * pvoidOsData)
   {
      m_hThread = (HANDLE) pvoidOsData;
   }

   void thread::set_os_int(int_ptr iData)
   {
      m_nThreadID = (dword_ptr) iData;
   }

   void thread::message_window_message_handler(gen::signal_object * pobj)
   {
   }


   CLASS_DECL_lnx ::ca::thread * get_thread()
   {
      ::lnx::thread * pwinthread = AfxGetThread();
      if(pwinthread == NULL)
         return NULL;
      return pwinthread->m_p;
   }


   void thread::LockTempMaps()
   {
      ++m_nTempMapLock;
   }
   WINBOOL thread::UnlockTempMaps(WINBOOL bDeleteTemp)
   {
      if (m_nTempMapLock != 0 && --m_nTempMapLock == 0)
   {
      if (bDeleteTemp)
      {
         // clean up temp objects
         m_pmapHGDIOBJ->delete_temp();
         m_pmapHDC->delete_temp();
         window::DeleteTempMap();
      }



#ifndef _AFX_PORTABLE
      ::radix::application * papp = dynamic_cast < ::radix::application * > (get_app());
      _AFX_THREAD_STATE* pThreadState = _afxThreadState.GetDataNA();
      if( pThreadState != NULL )
      {
         // restore safety pool after temp objects destroyed
         if(papp != NULL &&
             (pThreadState->m_pSafetyPoolBuffer == NULL ||
             _msize(pThreadState->m_pSafetyPoolBuffer) < papp->m_nSafetyPoolSize) &&
            papp->m_nSafetyPoolSize != 0)
         {
            // attempt to restore the safety pool to its max size
            size_t nOldSize = 0;
            if (pThreadState->m_pSafetyPoolBuffer != NULL)
            {
               nOldSize = _msize(pThreadState->m_pSafetyPoolBuffer);
               free(pThreadState->m_pSafetyPoolBuffer);
            }

            // undo handler trap for the following allocation
            WINBOOL bEnable = AfxEnableMemoryTracking(FALSE);
            try
            {
               pThreadState->m_pSafetyPoolBuffer = malloc(papp->m_nSafetyPoolSize);
               if (pThreadState->m_pSafetyPoolBuffer == NULL)
               {
//                  TRACE(::radix::trace::category_AppMsg, 0, "Warning: failed to reclaim %d bytes for primitive::memory safety pool.\n",
  //                   pApp->m_nSafetyPoolSize);
                  // at least get the old buffer back
                  if (nOldSize != 0)
                  {
                     //get it back
                     pThreadState->m_pSafetyPoolBuffer = malloc(nOldSize);
                     ASSERT(pThreadState->m_pSafetyPoolBuffer != NULL);
                  }
               }
            }
            catch( base_exception * )
            {
               AfxEnableMemoryTracking(bEnable);
               throw;
            }
            AfxEnableMemoryTracking(bEnable);
         }
      }
#endif  // !_AFX_PORTABLE
   }
   // return TRUE if temp maps still locked
      return m_nTempMapLock != 0;
   }

   int thread::thread_entry(::ca::thread_startup * pstartup)
   {

      _AFX_THREAD_STARTUP* pStartup = (_AFX_THREAD_STARTUP*)pstartup;
      ASSERT(pStartup != NULL);
      ASSERT(pStartup->pThreadState != NULL);
      ASSERT(pStartup->pThread != NULL);
      ASSERT(!pStartup->bError);

      ::lnx::thread* pThread = pStartup->pThread;

      ::radix::application* papp = dynamic_cast < ::radix::application * > (get_app());
      m_evFinish.ResetEvent();
      _001InstallMessageHandling(pThread);
      m_p->_001InstallMessageHandling(pThread);

      ::ca::window threadWnd;

      m_ptimera            = new ::user::interaction::timer_array;
      m_puiptra            = new user::LPWndArray;
      m_bRun               = true;

      m_ptimera->m_papp    = m_papp;
      m_puiptra->m_papp    = m_papp;


      if(!initialize_message_window(get_app(), ""))
         return -1;




      return 0;   // not reached
   }

   int thread::main()
   {

/*      _AFX_THREAD_STARTUP* pStartup = (_AFX_THREAD_STARTUP*)pstartup;
      ASSERT(pStartup != NULL);
      ASSERT(pStartup->pThreadState != NULL);
      ASSERT(pStartup->pThread != NULL);
      ASSERT(!pStartup->bError);*/

      if(!m_p->PreInitInstance())
      {
         return 0;
      }

      // first -- check for simple worker thread
      DWORD nResult = 0;
      if (m_pfnThreadProc != NULL)
      {
         nResult = (*m_pfnThreadProc)(m_pThreadParams);
      }
      // else -- check for thread with message loop
      else if (!m_p->initialize_instance())
      {
         try
         {
            nResult = exit();
         }
         catch(...)
         {
            nResult = (DWORD) -1;
         }
      }
      else
      {
         // will stop after PostQuitMessage called
         ASSERT_VALID(this);
         se_translator::attach();
   run:
         try
         {
            try
            {
               nResult = m_p->run();
            }
            catch(const ::ca::exception & e)
            {
               if(on_run_exception((::ca::exception &) e))
                  goto run;
               if(App(get_app()).final_handle_exception((::ca::exception & ) e))
                  goto run;
               try
               {
                  nResult = exit();
               }
               catch(...)
               {
                  nResult = (DWORD) -1;
               }
            }
         }
         catch(...)
         {
         }
         // let se_translator run undefinetely
         //se_translator::detach();
      }



      return 0;   // not reached
   }

   int thread::thread_term(int nResult)
   {
      try
      {
         finalize_message_window();
      }
      catch(...)
      {
      }

      try
      {
         // cleanup and shutdown the thread
//         threadWnd.Detach();
         AfxEndThread(dynamic_cast < ::radix::application * > (m_papp), nResult);
      }
      catch(...)
      {
      }
      return nResult;
   }

		///  \brief		starts thread on first call
		void thread::start ()
		{
		   pthread_mutex_unlock(&startMutex_);
		}

/**
* \file		src/lib/pal/src/linux/thread_linux.cpp
* \brief	Platform independent threads and synchronization objects (linux version)
* \author	Thomas Nass
*/

#include "internal_linux.hpp"

#include <iostream>
#include <string>

#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

namespace gen { namespace pal {

	namespace internal
	{
		Globals g_globals;

//		inline const char *b2s(bool b) { return b ? "true" : "false"; }

		//-----------------------------------------------------------------------------------------
		thread * ThisThread ()	// returns a pointer to the thread class of the calling thread
		{
			pthread_t myId = pthread_self();
			g_globals.mutex_.lock();
			thread * thisThread = g_globals.threadMap_[myId];
			if (!thisThread)
			{
				// thread must be an "external" pthread (e.g. created by a linked SIP-stack)
				// => get instance of ExternalThread-class from pool and add it temporarily to the global thread map
				if (!g_globals.externalThreadPool_.is_empty())
				{
					ExternalThread *extThread = g_globals.externalThreadPool_.front();
					g_globals.externalThreadPool_.pop_front();
					extThread->SetId(myId);
					g_globals.threadMap_[myId] = thisThread = extThread;
				}
				else
					thisThread = new ExternalThread;	// this will also set the thread ID and add it to the global thread map
			}
			g_globals.mutex_.unlock();
			return thisThread;
		}

		//-----------------------------------------------------------------------------------------
		FileDescWaiterThread::FileDescWaiterThread()
		{
			if ( pipe(fileDescs_) )
				;	// TO DO: throw
			int flags = fcntl( fileDescs_[1],F_GETFL );
			fcntl( fileDescs_[1], F_SETFL, flags|O_NONBLOCK );
			FD_ZERO(&fdRead);
			FD_ZERO(&fdWrite);
			FD_SET(fileDescs_[0], &fdRead);
			//written_=read_=0;
			start();
		}

		//-----------------------------------------------------------------------------------------
		FileDescWaiterThread::~FileDescWaiterThread()
		{
			FileDescWaiterThreadCommand command(FileDescWaiterThreadCommand::terminate);
			write(fileDescs_[1], &command, sizeof command);
			pal::wait(*this);
			close(fileDescs_[0]);
			close(fileDescs_[1]);
						//raise( SIGSEGV );
		}

		//-----------------------------------------------------------------------------------------
		unsigned FileDescWaiterThread::operator() ()
		{
			fd_evmap_t Map;

			for ( ;; ) {	// loop can only be stopped by "terminate" command
				mutex_.lock();
				fd_set actRead	= fdRead;
				fd_set actWrite = fdWrite;
				Map 			= evSetMap;
				mutex_.unlock();

				int	fdHigh 	= Map.is_empty() ? fileDescs_[0] : Map.rbegin()->first;
				int nSelect = select(fdHigh + 1, &actRead, &actWrite, 0, 0);
				if ( nSelect <= 0 )	// timeout: 0, error: -1
					continue;
				if ( FD_ISSET(fileDescs_[0], &actRead) ) {
					// read command from pipe
					FileDescWaiterThreadCommand command[ 5 ];
					int rc = 0;
					if ( ( rc = read(fileDescs_[0], &command, 5 * sizeof( FileDescWaiterThreadCommand ) ) ) == -1 )
						continue;
					//read_++;
					for ( unsigned int i = 0; i < ( rc/sizeof( FileDescWaiterThreadCommand ) ); i++ )
						if ( command[i]() == FileDescWaiterThreadCommand::terminate )
							return 0;
					if ( !--nSelect )
						continue;
				}
				g_globals.mutex_.lock();
				for ( itMap_t it=Map.begin(), ie=Map.end(); nSelect && it!=ie;  ) {
					int fd 		= it->first;
					bool bRead  = FD_ISSET(fd,&actRead);
					bool bWrite = FD_ISSET(fd,&actWrite);
					if ( bRead || bWrite ) {
						fd_evset_t& evSet = it->second;
						for ( itEv_t itE=evSet.begin(), itEnd=evSet.end(); itE != itEnd; ) {
							FileDescEvent *ev = *itE++;
							if ( !(ev->read_ ^ bRead) ) {	//	both true or both false
								ev->set();
								ev->set_active(false);	// prevent call to RemoveEvent for event already erased here
								evSet.erase(ev);
							}
						}
						mutex_.lock();
						if ( bRead ) {
							FD_CLR(fd, &fdRead);
							--nSelect;
						}
						if ( bWrite ) {
							FD_CLR(fd, &fdWrite);
							--nSelect;
						}
						++it;	// must increment before erase
						if ( evSet.is_empty() )
							evSetMap.erase(fd);
						mutex_.unlock();
					} else
						++it;
				}
				g_globals.mutex_.unlock();
			}
		}

		//-----------------------------------------------------------------------------------------
		void FileDescWaiterThread::ModifyEvent_(FileDescEvent *event, bool add /*= true*/)
		{
			int	fd = event->fileDesc_;

			mutex_.lock();
			if ( add ) {
				evSetMap[fd].insert(event);
				FD_SET(fd, event->read_ ? &fdRead : &fdWrite);
			} else {
				itMap_t it = evSetMap.find(fd);
				if ( it != evSetMap.end() ) {
					fd_evset_t& evSet = it->second;
					evSet.erase(event);
					unsigned nRead = 0;
					for ( itEv_t itE=evSet.begin(); itE!=evSet.end(); ++itE )
						nRead += (*itE)->read_;
					if ( !nRead )
						FD_CLR(fd, &fdRead);
					if ( evSet.size() == nRead )
						FD_CLR(fd, &fdWrite);
					if ( evSet.is_empty() )
						evSetMap.erase(fd);
				}
			}
			mutex_.unlock();

			// trigger thread
			FileDescWaiterThreadCommand command(FileDescWaiterThreadCommand::events_changed);
			int tries=30;
			while ( tries > 0 ) {
				if ( ::write(fileDescs_[1], &command, sizeof command) < 0 )
				{
					tries--;
					usleep(100);
//					if ( errno == EAGAIN && tries == 0 )  {
//						void *mythis = this;
//						char buffer[1000];
//						int rc = read(fileDescs_[0],buffer,1000);
//						strcpy(0,"coredump");;
//					}
				}
				else
					tries = 0;
			}
//			written_++;
			//if ( written_ - read_ > 200 )
			//	strcpy(0,"coredump");
		}

	} // namespace internal

	wait_result wait (WaitableItem & waitableItem, const boost::posix_time::time_duration& duration/*=InfiniteTime*/)
	{
		return internal::ThisThread()->wait(waitableItem, duration);
	}

	wait_result wait (size_t numberOfItems, WaitableItem * waitableItems[], const boost::posix_time::time_duration& duration/*=InfiniteTime*/, bool waitForAll /*= false*/)
	{
		return internal::ThisThread()->wait(numberOfItems, waitableItems, duration, waitForAll);
	}

	void ExternalThread::endWait()
	{
		internal::g_globals.threadMap_.erase(thread_);
		internal::g_globals.externalThreadPool_.push_back(this);
	}

	thread::thread()
		: WaitableItem(false, false), isMainThread_(false), isExternalThread_(false)
	{
		pthread_attr_t Attributes;

		if (   pthread_attr_init(&Attributes)
			|| pthread_attr_setdetachstate(&Attributes, PTHREAD_CREATE_DETACHED)
			|| pthread_cond_init(&wakeUpCondition_, 0)
			|| pthread_mutex_init(&startMutex_, 0)
			|| pthread_mutex_lock(&startMutex_)
			|| pthread_create(&thread_, &Attributes, reinterpret_cast<void * (*)(void *)>(thread::startExecution_), this))
			; // throw ...

		// Scheduling policy is set to round robin and priority to normal
		struct sched_param param = {0};
		param.sched_priority = pal::thread_priority_normal;
		pthread_setschedparam( thread_, SCHED_RR, &param );

		internal::g_globals.mutex_.lock();
		internal::g_globals.threadMap_[thread_] = this;
		internal::g_globals.mutex_.unlock();
	}

	thread::thread(bool isMainThread, bool isExternalThread)
		: WaitableItem(false, false), isMainThread_(isMainThread), isExternalThread_(isExternalThread)
	{
		if (pthread_cond_init(&wakeUpCondition_, 0))
			; // throw ...

		thread_ = pthread_self();
		internal::g_globals.mutex_.lock();
		internal::g_globals.threadMap_[thread_] = this;
		internal::g_globals.mutex_.unlock();
	}

	wait_result thread::wait (WaitableItem & item, const boost::posix_time::time_duration& duration/*=InfiniteTime*/)
	{
		WaitableItem * itemPtr = &item;
		return /*thread::*/wait (1, &itemPtr, duration);
	}

	wait_result thread::wait (size_t numberOfItems, WaitableItem * items[], const boost::posix_time::time_duration& duration/*=InfiniteTime*/, bool waitForAll /*= false*/)
	{
		if (pthread_self() != thread_)
			// do not call endWait() here
			return wait_result(wait_result::BadThread);

		internal::g_globals.mutex_.lock();

		if (!numberOfItems)
		{
			endWait();
			internal::g_globals.mutex_.unlock();
			return wait_result(wait_result::Failure);
		}

		waitForAllItems_ = waitForAll;

		if (!waitForAllItems_)
		{
			for (size_t i=0; i < numberOfItems; ++i)
				if (*items[i])
				{
					items[i]->threadTriggered_();
					endWait();
					internal::g_globals.mutex_.unlock();
					return wait_result(wait_result::Event0 + i);
				}


			if (duration.total_nanoseconds() == 0)
			{
				endWait();
				internal::g_globals.mutex_.unlock();
				return wait_result(wait_result::Timeout);
			}
		}

		// add all events we wait for to itemsToWaitFor_ and the thread to WaitingThreads_ of every event we wait for
		for (size_t i=0; i < numberOfItems; ++i)
		{
			itemsToWaitFor_.push_back(items[i]);
			items[i]->addThread_(*this);
		}

		if (waitForAllItems_ && checkAllItems_())
		{
			cleanUpItems_();
			endWait();
			internal::g_globals.mutex_.unlock();
			return wait_result(wait_result::Event0);
		}
		else if (!duration.total_nanoseconds())
		{
			cleanUpItems_();
			endWait();
			internal::g_globals.mutex_.unlock();
			return wait_result(wait_result::Timeout);
		}

		waitResult_ = wait_result::Timeout;

		// activate all events
		std::list<WaitableItem *>::iterator it(itemsToWaitFor_.begin()), end(itemsToWaitFor_.end());
		for (; it != end; ++it)
			(*it)->init_wait();	// activate event

		// The cleanUpItems_-member-function of the thread-class will set waitResult_ before setting the wakeUpCondition_
		if (duration.is_pos_infinity())
			while (pthread_cond_wait(&wakeUpCondition_, &internal::g_globals.mutex_()) == EINTR)
				;	// nothing to do
		else
		{
			timeval tv;
			gettimeofday(&tv, 0);
			struct timespec Timeout = {tv.tv_sec + duration.total_seconds(), tv.tv_usec * 1000 + duration.total_nanoseconds() % 1000000000};
			if (Timeout.tv_nsec >= 1000000000)
			{
				Timeout.tv_nsec -= 1000000000;
				++Timeout.tv_sec;
			}
			while (pthread_cond_timedwait(&wakeUpCondition_, &internal::g_globals.mutex_(), &Timeout) == EINTR)
				;	// nothing to do
		}

		if (waitResult_ == wait_result::Timeout)
			cleanUpItems_();

		endWait();
		internal::g_globals.mutex_.unlock();
		return wait_result(waitResult_);
	}

	void thread::cleanUpItems_ (WaitableItem * signallingItem /* = 0 */)	// must be called with g_globals locked !!!
	{
		// remove thread from WaitingThreads_ of all events in itemsToWaitFor_
		// except the signalling event (because it is iterating over the list and will remove it itself) !!!
		std::list<WaitableItem *>::iterator it(itemsToWaitFor_.begin()), end(itemsToWaitFor_.end());

		for (int i=0 ; it != end; ++it, ++i)
		{
			if (*it != signallingItem)
				(*it)->removeThread_(*this);

			if (**it && waitResult_ == wait_result::Timeout)
				waitResult_ = wait_result::Event0 + i;
		}

		itemsToWaitFor_.clear();

		if (signallingItem) {
			pthread_cond_signal(&wakeUpCondition_);
        }
	}

	bool thread::checkAllItems_ (WaitableItem * signallingItem /* = 0 */)	// must be called with g_globals locked !!!
	{
		// remove thread from WaitingThreads_ of all events in itemsToWaitFor_
		// except the signalling event (because it is iterating over the list and will remove it itself) !!!
		std::list<WaitableItem *>::iterator it(itemsToWaitFor_.begin()), end(itemsToWaitFor_.end());

		for ( ; it != end; ++it )
			if ( !**it ) {
				(*it)->init_wait();	// reactivate event
				return false;
			}

		// all events signaled => inform all events except signalling event
		for (it = itemsToWaitFor_.begin(); it != end; ++it)
			if (*it != signallingItem)
				(*it)->threadTriggered_();

		return true;
	}

	void thread::set_priority(int priority)
	{
		struct sched_param param = {0};

		// Test routine
		/*int policy;
		memset( &param, 0, sizeof( param ) );
		pthread_getschedparam( thread_, &policy, &param );
		std::cout << "thread::set_priority: thread has policy " << policy << " and priority " << param.sched_priority << '\n';
		std::cout << "thread::set_priority: Priority to set " << priority << '\n';*/

		param.sched_priority = priority;
		/*int result = */pthread_setschedparam( thread_, SCHED_RR, &param );

		// TODO: We would need superuser privileges to change priority
		/*if ( result )
		{
			switch ( result )
			{
			case EPERM:
				std::cout << "thread::set_priority: You do not have superuser privileges\n";
				break;
			case ESRCH:
				std::cout << "thread::set_priority: thread does not exist\n";
				break;
			case EFAULT:
				std::cout << "thread::set_priority: Illegal parameter address\n";
				break;
			case EINVAL:
				std::cout << "thread::set_priority: Priority inconsistent with policy\n";
				break;
			default:
				std::cout << "thread::set_priority: Unknown error\n";
			}
		}*/
	}

	int thread::priority()
	{
		struct sched_param param = {0};
		int policy;

		/*int result = */pthread_getschedparam( thread_, &policy, &param );

		// TODO: We have to decide how to react on errors
		/*if ( result )
		{
			switch ( result )
			{
			case ESRCH:
				std::cout << "thread::priority: thread does not exist\n";
				break;
			case EFAULT:
				std::cout << "thread::priority: Illegal parameter address\n";
				break;
			case EINVAL:
				std::cout << "thread::priority: Priority inconsistent with policy\n";
				break;
			default:
				std::cout << "thread::priority: Unknown error\n";
			}
		}*/

		return param.sched_priority;
	}

	thread::~thread()
	{
		internal::g_globals.mutex_.lock();
		cleanUpItems_();
		internal::g_globals.threadMap_.erase(thread_);
		internal::g_globals.mutex_.unlock();

		if (!isMainThread_)
			pthread_mutex_destroy(&startMutex_);
		pthread_cond_destroy(&wakeUpCondition_);
	}

	unsigned long MillisecondCounter ()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	void thread::wait()
	{

	   event_base::wait();
	}


	bool thread::has_message()
	{

	   throw "how to know?";

	}

//} } // namespace gen { namespace pal {
//
//
// EoF
///////


} // namespace lnx




WINBOOL CLASS_DECL_lnx AfxInternalPumpMessage();
LRESULT CLASS_DECL_lnx AfxInternalProcessWndProcException(base_exception*, const MSG* pMsg);
void AfxInternalPreTranslateMessage(gen::signal_object * pobj);
WINBOOL AfxInternalIsIdleMessage(gen::signal_object * pobj);
WINBOOL AfxInternalIsIdleMessage(LPMSG lpmsg);


/*thread* CLASS_DECL_lnx System.GetThread()
{
// check for current thread in module thread state
__MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
//thread* pThread = pState->m_pCurrentWinThread;
return pThread;
}

MSG* CLASS_DECL_lnx AfxGetCurrentMessage()
{
_AFX_THREAD_STATE* pState = AfxGetThreadState();
ASSERT(pState);
return &(pState->m_msgCur);
}

WINBOOL CLASS_DECL_lnx AfxInternalPumpMessage()
{
_AFX_THREAD_STATE *pState = AfxGetThreadState();

if (!::GetMessage(&(pState->m_msgCur), NULL, NULL, NULL))
{
#ifdef DEBUG
TRACE(::radix::trace::category_AppMsg, 1, "thread::pump_message - Received WM_QUIT.\n");
pState->m_nDisablePumpCount++; // application must die
#endif
// Note: prevents calling message loop things in 'exit_instance'
// will never be decremented
return FALSE;
}

#ifdef DEBUG
if (pState->m_nDisablePumpCount != 0)
{
TRACE(::radix::trace::category_AppMsg, 0, "Error: thread::pump_message called when not permitted.\n");
ASSERT(FALSE);
}
#endif

#ifdef DEBUG
_AfxTraceMsg("pump_message", &(pState->m_msgCur));
#endif

// process this message

if (pState->m_msgCur.message != WM_KICKIDLE && !AfxPreTranslateMessage(&(pState->m_msgCur)))
{
::TranslateMessage(&(pState->m_msgCur));
::DispatchMessage(&(pState->m_msgCur));
}
return TRUE;
}

WINBOOL CLASS_DECL_lnx AfxPumpMessage()
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->pump_message();
else
return AfxInternalPumpMessage();
}

LRESULT CLASS_DECL_lnx AfxInternalProcessWndProcException(base_exception*, const MSG* pMsg)
{
if (pMsg->message == WM_CREATE)
{
return -1;  // just fail
}
else if (pMsg->message == WM_PAINT)
{
// force validation of ::ca::window to prevent getting WM_PAINT again
ValidateRect(pMsg->hwnd, NULL);
return 0;
}
return 0;   // sensible default for rest of commands
}

LRESULT CLASS_DECL_lnx AfxProcessWndProcException(base_exception* e, const MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->ProcessWndProcException( e, pMsg );
else
return AfxInternalProcessWndProcException( e, pMsg );
}
WINBOOL AfxInternalPreTranslateMessage(MSG* pMsg)
{
//   ASSERT_VALID(this);

thread *pThread = System.GetThread();
if( pThread )
{
// if this is a thread-message, short-circuit this function
if (pMsg->hwnd == NULL && pThread->DispatchThreadMessageEx(pMsg))
return TRUE;
}

// walk from target to main ::ca::window
::user::interaction* pMainWnd = System.GetMainWnd();
/* trans   if (::ca::window::WalkPreTranslateTree(pMainWnd->GetSafeHwnd(), pMsg))
return TRUE; */

// in case of modeless dialogs, last chance route through main
//   ::ca::window's accelerator table
/*   if (pMainWnd != NULL)
{
::ca::window * pWnd = ::lnx::window::from_handle(pMsg->hwnd);
if (pWnd != NULL && LNX_WINDOW(pWnd)->GetTopLevelParent() != pMainWnd)
return pMainWnd->pre_translate_message(pMsg);
}

return FALSE;   // no special processing
}

WINBOOL __cdecl AfxPreTranslateMessage(MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->pre_translate_message( pMsg );
else
return AfxInternalPreTranslateMessage( pMsg );
}

WINBOOL AfxInternalIsIdleMessage(MSG* pMsg)
{
// Return FALSE if the message just dispatched should _not_
// cause on_idle to be run.  Messages which do not usually
// affect the state of the ::fontopus::user interface and happen very
// often are checked for.

// redundant WM_MOUSEMOVE and WM_NCMOUSEMOVE
if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE)
{
// mouse move at same position as last mouse move?
_AFX_THREAD_STATE *pState = AfxGetThreadState();
if (pState->m_ptCursorLast == pMsg->pt && pMsg->message == pState->m_nMsgLast)
return FALSE;

pState->m_ptCursorLast = pMsg->pt;  // remember for next time
pState->m_nMsgLast = pMsg->message;
return TRUE;
}

// WM_PAINT and WM_SYSTIMER (caret blink)
return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}

WINBOOL __cdecl AfxIsIdleMessage(MSG* pMsg)
{
thread *pThread = System.GetThread();
if( pThread )
return pThread->is_idle_message( pMsg );
else
return AfxInternalIsIdleMessage( pMsg );
}

/*
thread* CLASS_DECL_lnx AfxBeginThread(::ca::type_info pThreadClass,
int nPriority, UINT nStackSize, DWORD dwCreateFlags,
LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
#ifndef _MT
pThreadClass;
nPriority;
nStackSize;
dwCreateFlags;
lpSecurityAttrs;

return NULL;
#else
ASSERT(pThreadClass != NULL);
ASSERT(pThreadClass->IsDerivedFrom(&typeid(thread)));

thread* pThread = dynamic_cast < thread * > (App(get_app()).alloc(pThreadClass));
if (pThread == NULL)
AfxThrowMemoryException();
ASSERT_VALID(pThread);

pThread->m_pThreadParams = NULL;
if(pThread->Begin(
nPriority,
nStackSize,
dwCreateFlags,
lpSecurityAttrs))
return pThread;
else
return NULL;
#endif //!_MT
}*/

/*
void CLASS_DECL_lnx AfxEndThread(UINT nExitCode, WINBOOL bDelete)
{
#ifndef _MT
nExitCode;
bDelete;
#else
// remove current thread object from primitive::memory
__MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
// thread* pThread = pState->m_pCurrentWinThread;
if (pThread != NULL)
{
ASSERT_VALID(pThread);
ASSERT(pThread != &System);

if (bDelete)
pThread->Delete();
// pState->m_pCurrentWinThread = NULL;
}

// allow cleanup of any thread local objects
AfxTermThread();

// allow C-runtime to cleanup, and exit the thread
_endthreadex(nExitCode);
#endif //!_MT
}

/////////////////////////////////////////////////////////////////////////////
// Global functions for thread initialization and thread cleanup

LRESULT CALLBACK _AfxMsgFilterHook(int code, WPARAM wParam, LPARAM lParam);

void CLASS_DECL_lnx AfxInitThread()
{
if (!afxContextIsDLL)
{
// set message filter proc
_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
ASSERT(pThreadState->m_hHookOldMsgFilter == NULL);
pThreadState->m_hHookOldMsgFilter = ::SetWindowsHookEx(WH_MSGFILTER,
_AfxMsgFilterHook, NULL, ::GetCurrentThreadId());
}
}





WINBOOL thread::CreateThread(DWORD dwCreateFlags, UINT nStackSize,
LPSECURITY_ATTRIBUTES lpSecurityAttrs)
{
#ifndef _MT
dwCreateFlags;
nStackSize;
lpSecurityAttrs;

return FALSE;
#else
ENSURE(m_hThread == NULL);  // already created?

// setup startup structure for thread initialization
_AFX_THREAD_STARTUP startup; memset(&startup, 0, sizeof(startup));
startup.pThreadState = AfxGetThreadState();
startup.pThread = this;
startup.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
startup.hEvent2 = ::CreateEvent(NULL, TRUE, FALSE, NULL);
startup.dwCreateFlags = dwCreateFlags;
if (startup.hEvent == NULL || startup.hEvent2 == NULL)
{
TRACE(::radix::trace::category_AppMsg, 0, "Warning: CreateEvent failed in thread::CreateThread.\n");
if (startup.hEvent != NULL)
::CloseHandle(startup.hEvent);
if (startup.hEvent2 != NULL)
::CloseHandle(startup.hEvent2);
return FALSE;
}

#ifdef _WIN32
//   m_thread = ::CreateThread(NULL, 0, StartThread, this, 0, &m_dwThreadId);
// create the thread (it may or may not start to run)
m_hThread = (HANDLE)(ulong_ptr)_beginthreadex(lpSecurityAttrs, nStackSize,
&_AfxThreadEntry, &startup, dwCreateFlags | CREATE_SUSPENDED, (UINT*)&m_nThreadID);
#else
pthread_attr_t attr;

pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
if (pthread_create(&m_thread,&attr, StartThread,this) == -1)
{
perror("thread: create failed");
SetRunning(false);
}
//   pthread_attr_destroy(&attr);
#endif

if (m_hThread == NULL)
return FALSE;

// start the thread just for ca2 API initialization
VERIFY(ResumeThread() != (DWORD)-1);
VERIFY(::WaitForSingleObject(startup.hEvent, INFINITE) == WAIT_OBJECT_0);
::CloseHandle(startup.hEvent);

// if created suspended, suspend it until resume thread wakes it up
if (dwCreateFlags & CREATE_SUSPENDED)
VERIFY(::SuspendThread(m_hThread) != (DWORD)-1);

// if error during startup, shut things down
if (startup.bError)
{
VERIFY(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0);
::CloseHandle(m_hThread);
m_hThread = NULL;
::CloseHandle(startup.hEvent2);
return FALSE;
}

// allow thread to continue, once resumed (it may already be resumed)
::SetEvent(startup.hEvent2);
return TRUE;
#endif //!_MT
}

void thread::Delete()
{

// delete thread if it is auto-deleting
if(m_bAutoDelete)
{
m_evFinish.SetEvent();
delete this;
}
else
{
m_hThread = NULL;
m_evFinish.SetEvent();
}
}

/////////////////////////////////////////////////////////////////////////////
// thread default implementation

bool thread::PreInitInstance()
{
return true;
}

bool thread::initialize_instance()
{
ASSERT_VALID(this);

return true;   // by default enter run loop
}

// main running routine until thread exits
int thread::run()
{
ASSERT_VALID(this);
_AFX_THREAD_STATE* pState = AfxGetThreadState();

// for tracking the idle time state
WINBOOL bIdle = TRUE;
LONG lIdleCount = 0;

// acquire and dispatch messages until a WM_QUIT message is received.
while(m_bRun)
{
// phase1: check to see if we can do idle work
while (bIdle &&
!::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE))
{
// call on_idle while in bIdle state
if (!on_idle(lIdleCount++))
bIdle = FALSE; // assume "no idle" state
}

// phase2: pump messages while available
do
{
// pump message, but quit on WM_QUIT
if (!pump_message())
return exit_instance();

// reset "no idle" state after pumping "normal" message
//if (is_idle_message(&m_msgCur))
if (is_idle_message(&(pState->m_msgCur)))
{
bIdle = TRUE;
lIdleCount = 0;
}


} while (::PeekMessage(&(pState->m_msgCur), NULL, NULL, NULL, PM_NOREMOVE));

m_ptimera->check();
}
}

WINBOOL thread::is_idle_message(MSG* pMsg)
{
return AfxInternalIsIdleMessage(pMsg);
}

int thread::exit_instance()
{
ASSERT_VALID(this);
ASSERT(&System != this);

for(int i = 0; i < m_puieptra->get_count(); i++)
{
m_puieptra->element_at(i)->m_pthread = NULL;
}

delete m_ptimera;
delete m_puieptra;

int nResult = (int)AfxGetCurrentMessage()->wParam;  // returns the value from PostQuitMessage
return nResult;
}

WINBOOL thread::on_idle(LONG lCount)
{
ASSERT_VALID(this);

#if defined(DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
// check ca2 API's allocator (before idle)
if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
ASSERT(AfxCheckMemory());
#endif

if (lCount <= 0)
{
// send WM_IDLEUPDATECMDUI to the main ::ca::window
::user::interaction* pMainWnd = GetMainWnd();
if (pMainWnd != NULL && pMainWnd->IsWindowVisible())
{
/*AfxCallWndProc(pMainWnd, pMainWnd->get_handle(),
WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);*/
/*       pMainWnd->SendMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
(WPARAM)TRUE, 0, TRUE, TRUE);
}
// send WM_IDLEUPDATECMDUI to all frame windows
/* linux __MODULE_THREAD_STATE* pState = _AFX_CMDTARGET_GETSTATE()->m_thread;
frame_window* pFrameWnd = pState->m_frameList;
while (pFrameWnd != NULL)
{
if (pFrameWnd->get_handle() != NULL && pFrameWnd != pMainWnd)
{
if (pFrameWnd->m_nShowDelay == SW_HIDE)
pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
if (pFrameWnd->IsWindowVisible() ||
pFrameWnd->m_nShowDelay >= 0)
{
AfxCallWndProc(pFrameWnd, pFrameWnd->get_handle(),
WM_IDLEUPDATECMDUI, (WPARAM)TRUE, 0);
pFrameWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
(WPARAM)TRUE, 0, TRUE, TRUE);
}
if (pFrameWnd->m_nShowDelay > SW_HIDE)
pFrameWnd->ShowWindow(pFrameWnd->m_nShowDelay);
pFrameWnd->m_nShowDelay = -1;
}
pFrameWnd = pFrameWnd->m_pNextFrameWnd;
}*/
/*}
else if (lCount >= 0)
{
__MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
if (pState->m_nTempMapLock == 0)
{
// free temp maps, OLE DLLs, etc.
AfxLockTempMaps();
AfxUnlockTempMaps();
}
}

#if defined(DEBUG) && !defined(_AFX_NO_DEBUG_CRT)
// check ca2 API's allocator (after idle)
if (_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) & _CRTDBG_CHECK_ALWAYS_DF)
ASSERT(AfxCheckMemory());
#endif

return lCount < 0;  // nothing more to do if lCount >= 0
}

::user::lnx::message::e_prototype thread::GetMessagePrototype(UINT uiMessage, UINT uiCode)
{
return ::user::lnx::message::PrototypeNone;
}


WINBOOL thread::DispatchThreadMessageEx(MSG* pmsg)
{
if(pmsg->message == WM_APP + 1984 && pmsg->wParam == 77)
{
gen::scoped_ptr < lnx::message > spmessage(pmsg->lParam);
spmessage->send();
return TRUE;
}
/*   const __MSGMAP* pMessageMap; pMessageMap = GetMessageMap();
const __MSGMAP_ENTRY* lpEntry;

for (/* pMessageMap already init'ed *//*; pMessageMap->pfnGetBaseMap != NULL;
pMessageMap = (*pMessageMap->pfnGetBaseMap)())
{
// Note: catch not so common but fatal mistake!!
//       // BEGIN_MESSAGE_MAP(CMyThread, CMyThread)

ASSERT(pMessageMap != (*pMessageMap->pfnGetBaseMap)());
if (pMsg->message < 0xC000)
{
// constant ::ca::window message
if ((lpEntry = AfxFindMessageEntry(pMessageMap->lpEntries,
pMsg->message, 0, 0)) != NULL)
goto LDispatch;
}
else
{
// registered windows message
lpEntry = pMessageMap->lpEntries;
while ((lpEntry = AfxFindMessageEntry(lpEntry, 0xC000, 0, 0)) != NULL)
{
UINT* pnID = (UINT*)(lpEntry->nSig);
ASSERT(*pnID >= 0xC000);
// must be successfully registered
if (*pnID == pMsg->message)
goto LDispatch;
lpEntry++;      // keep looking past this one
}
}
}
return FALSE;

LDispatch:
union MessageMapFunctions mmf;
mmf.pfn = lpEntry->pfn;

// always posted, so return value is meaningless

(this->*mmf.pfn_THREAD)(pMsg->wParam, pMsg->lParam);*/

/*LRESULT lresult;
SignalPtrArray signalptra;
m_signala.GetSignalsByMessage(signalptra, pmsg->message, 0, 0);
for(int i = 0; i < signalptra.get_size(); i++)
{
Signal & signal = *signalptra[i];
gen::signal * psignal = signal.m_psignal;
::user::lnx::message::e_prototype eprototype = signal.m_eprototype;
if(eprototype == ::user::lnx::message::PrototypeNone)
{
::user::lnx::message::base base;
base.m_psignal = psignal;
lresult = 0;
base.set(pmsg->message, pmsg->wParam, pmsg->lParam, lresult);
psignal->emit(&base);
if(base.m_bRet)
return true;
}
break;
}
return true;
}

WINBOOL thread::pre_translate_message(gen::signal_object * pobj)
{
ASSERT_VALID(this);
return AfxInternalPreTranslateMessage( pMsg );
}

LRESULT thread::ProcessWndProcException(base_exception* e, const MSG* pMsg)
{
return AfxInternalProcessWndProcException( e, pMsg );
}
*/

/////////////////////////////////////////////////////////////////////////////
// Message Filter processing (WH_MSGFILTER)

LRESULT CALLBACK _AfxMsgFilterHook(int code, WPARAM wParam, LPARAM lParam)
{
   ::radix::thread* pthread;
   if (afxContextIsDLL || (code < 0 && code != MSGF_DDEMGR) || (pthread = dynamic_cast < ::radix::thread * > (::lnx::get_thread())) == NULL)
   {
      return ::CallNextHookEx(_afxThreadState->m_hHookOldMsgFilter, code, wParam, lParam);
   }
   ASSERT(pthread != NULL);
   ::ca::smart_pointer < ::user::lnx::message::base > spbase;
   spbase(pthread->get_base((LPMSG)lParam));
   pthread->ProcessMessageFilter(code, spbase);
   LRESULT lresult = spbase->m_bRet ? 1 : 0;
   return lresult;
}

__STATIC WINBOOL CLASS_DECL_lnx IsHelpKey(LPMSG lpMsg)
// return TRUE only for non-repeat F1 keydowns.
{
   return lpMsg->message == WM_KEYDOWN &&
      lpMsg->wParam == VK_F1 &&
      !(HIWORD(lpMsg->lParam) & KF_REPEAT) &&
      GetKeyState(VK_SHIFT) >= 0 &&
      GetKeyState(VK_CONTROL) >= 0 &&
      GetKeyState(VK_MENU) >= 0;
}

__STATIC inline WINBOOL IsEnterKey(LPMSG lpMsg)
{ return lpMsg->message == WM_KEYDOWN && lpMsg->wParam == VK_RETURN; }

__STATIC inline WINBOOL IsButtonUp(LPMSG lpMsg)
{ return lpMsg->message == WM_LBUTTONUP; }
