#pragma once

WINBOOL AfxInternalPreTranslateMessage(MSG* pMsg);

namespace ca
{
   struct  thread_startup
   {
      ::ca::thread *          m_pthread;    // thread for new thread
      HANDLE hEvent;          // event triggered after success/non-success
      HANDLE hEvent2;         // event triggered after thread is resumed

      thread_startup();
      ~thread_startup();
   };

} // namespace ca

namespace lnx
{

   class CLASS_DECL_lnx thread :
      virtual public ::radix::thread,
      virtual public ::ca::message_window_simple_callback
   {
   public:


/// thread ID, used to ensure that the thread that calls one of the WaitForEvents
		/// methods is really the thread represented by the Thread class
		pthread_t			thread_;

		/// true only for the main thread that is represented by pal::MainThread
		bool				isMainThread_;
		/// true only for external threads (e.g. a thread of a linked SIP-stack that calls the pal::wait-function).
		bool				isExternalThread_;
		/// condition used to wake up the thread if an event for which the thread
		/// is waiting is signaled
		pthread_cond_t		wakeUpCondition_;
		/// ensures that the thread function is not called before the Thread class
		/// (e.g. thread_) is initialized
		pthread_mutex_t		startMutex_;
		/// result of last wait operation
		int					waitResult_;
		/// true if waiting should be done on all items
		bool				waitForAllItems_;
		/// a list of items this threads waits for
		::collection::list < waitable * >	itemsToWaitFor_;


      // list of frame_window objects for thread
      simple_list < frame_window * > m_frameList;

      // temporary/permanent ::collection::map state
      DWORD m_nTempMapLock;           // if not 0, temp maps locked
      //hmenu_map      * m_pmapHMENU;
      //hdc_map        * m_pmapHDC;
      //hgdiobj_map    * m_pmapHGDIOBJ;



      LPVOID                              m_pThreadParams; // generic parameters passed to starting function
      __THREADPROC                      m_pfnThreadProc;

      ::event                              m_evFinish;
      UINT                                m_nDisablePumpCount;
      mutex                               m_mutexUiPtra;

      ::ca::thread *                      m_pAppThread;

      UINT                                m_dwFinishTimeout;

      virtual void * get_os_data();
      virtual int_ptr get_os_int();

      void set_os_data(void * pvoidOsData);
      void set_os_int(int_ptr iData);

      virtual void set_p(::radix::thread * p);

      thread(::ca::application * papp);

      virtual void construct(__THREADPROC pfnThreadProc, LPVOID pParam);

      virtual bool Begin(::ca::e_thread_priority epriority = get_thread_priority_normal(), UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

      bool create_thread(DWORD dwCreateFlags = 0, UINT nStackSize = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL);

      virtual ::user::interaction * SetMainWnd(::user::interaction * pui);

      virtual int thread_entry(::ca::thread_startup * pstartup);
      virtual int main();
      virtual int thread_term(int nResult);


      virtual void add(::user::interaction * pui);
      virtual void remove(::user::interaction * pui);
      virtual ::count get_ui_count();
      virtual ::user::interaction * get_ui(int iIndex);
      virtual void set_timer(::user::interaction * pui, uint_ptr nIDEvent, UINT nEllapse);
      virtual void unset_timer(::user::interaction * pui, uint_ptr nIDEvent);
      virtual void set_auto_delete(bool bAutoDelete = true);
      virtual void set_run(bool bRun = true);
      virtual event & get_finish_event();
      virtual bool get_run();
      virtual ::ca::thread * get_app_thread();
      virtual ::user::interaction * get_active_ui();
      virtual ::user::interaction * set_active_ui(::user::interaction * pui);
      virtual void step_timer();


      virtual void on_delete(::ca::ca * poc);

      int GetThreadPriority();
      WINBOOL SetThreadPriority(int nPriority);

   // Operations
      DWORD SuspendThread();
      DWORD ResumeThread();
      bool PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam);
      bool post_message(::user::interaction * pguie, UINT message, WPARAM wParam, LPARAM lParam);

      virtual bool PreInitInstance();

      // called when occurs an se_exception exception in run
      // return true to call run again
      virtual bool on_run_exception(::ca::exception & e);

   // Overridables
      // thread initialization
      virtual bool initialize_instance();

      virtual gen::message::e_prototype GetMessagePrototype(UINT uiMessage, UINT uiCode);

      // running and idle processing
      virtual int run();
      virtual void pre_translate_message(gen::signal_object * pobj);
      virtual bool pump_message();     // low level message pump
      virtual bool on_idle(LONG lCount); // return TRUE if more idle processing
      virtual bool is_idle_message(gen::signal_object * pobj);  // checks for special messages
      virtual WINBOOL is_idle_message(LPMSG lpmsg);  // checks for special messages
      virtual void message_handler(gen::signal_object * pobj);

      // thread termination
      virtual int exit_instance(); // default will 'delete this'

      // Advanced: exception handling
      virtual void ProcessWndProcException(base_exception * e, gen::signal_object * pMsg);

      // Advanced: handling messages sent to message filter hook
      virtual void ProcessMessageFilter(int code, gen::signal_object * pobj);

      // Advanced: virtual access to GetMainWnd()
      virtual ::user::interaction* GetMainWnd();

   // Implementation
   public:
      virtual ~thread();
   #ifdef DEBUG
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;
   #endif
      void CommonConstruct();
      virtual void Delete();
         // 'delete this' only if m_bAutoDelete == TRUE



      virtual void DispatchThreadMessageEx(gen::signal_object * pobj);  // helper
      virtual void message_window_message_handler(gen::signal_object * pobj);

      virtual void delete_temp();


      virtual void LockTempMaps();
      virtual WINBOOL UnlockTempMaps(WINBOOL bDeleteTemp);

      operator pthread_t() const;
   };

   CLASS_DECL_lnx ::ca::thread * get_thread();

} // namespace lnx


