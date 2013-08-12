#include "framework.h"
#include <X11/cursorfont.h>
#include <dlfcn.h>
#include <link.h>

extern __thread thread_local_storage * __thread_data;

namespace lnx
{

   application::application(sp(::ca2::application) papp) :
      ca2(papp)
   {
      ::ca2::thread::m_p.create(allocer());
      ::ca2::thread::m_p->m_p = this;

      LNX_THREAD(::ca2::thread::m_p.m_p)->m_pAppThread = this;

      m_pfilemanager = NULL;



      // in non-running state until WinMain
// xxx      m_hInstance = NULL;
//      m_hLangResourceDLL = NULL;
      m_pszHelpFilePath = NULL;
      m_pszProfileName = NULL;
      m_pszRegistryKey = NULL;
//      m_pRecentFileList = NULL;
      m_pdocmanager = NULL;
// xxx       m_atomApp = m_atomSystemTopic = NULL;
      //m_lpCmdLine = NULL;
//      m_pCmdInfo = NULL;


      // initialize current printer state
// xxx       m_hDevMode = NULL;
// xxx       m_hDevNames = NULL;
      m_nNumPreviewPages = 0;     // not specified (defaults to 1)

      // other initialization
      m_bHelpMode = FALSE;
//      m_eHelpType = afxWinHelp;
      m_nSafetyPoolSize = 512;        // default size

      shell::theLinuxShell.Initialize();
   }

   application::~application()
   {

   }


   void application::_001OnFileNew()
   {
      ::ca2::application_base::m_p->_001OnFileNew(NULL);
   }

   sp(::user::document_interface) application::_001OpenDocumentFile(var varFile)
   {
      return ::ca2::application_base::m_p->_001OpenDocumentFile(varFile);
   }

   void application::_001EnableShellOpen()
   {
// xxx       ASSERT(m_atomApp == NULL && m_atomSystemTopic == NULL); // do once

// xxx       m_atomApp            = ::GlobalAddAtomW(::ca2::international::utf8_to_unicode(m_strAppName));
// xxx       m_atomSystemTopic    = ::GlobalAddAtomW(L"system");
   }

   bool application::_001OnDDECommand(const char * lpcsz)
   {
      UNREFERENCED_PARAMETER(lpcsz);
      return FALSE;
   }

   HINSTANCE application::GetHinstance()
   {
      return NULL;
   }

   string application::get_version()
   {

      throw todo(get_app());

/*      char lpszModuleFilePath[MAX_PATH + 1];
      GetModuleFileName(NULL, lpszModuleFilePath, MAX_PATH + 1);

      DWORD dw;

      DWORD dwResSize = GetFileVersionInfoSize(
         lpszModuleFilePath,
         &dw);


      if(dwResSize > 0)
      {
         LPVOID lpdata = new BYTE[dwResSize];
         if(GetFileVersionInfo(
            lpszModuleFilePath,
            0,
            dwResSize,
            lpdata))
         {
            UINT cbTranslate;
            struct LANGANDCODEPAGE {
               WORD wLanguage;
               WORD wCodePage;
            } *lpTranslate;

            // read the list of languages and code pages.

            VerQueryValue(lpdata,
               TEXT("\\VarFileInfo\\Translation"),
               (LPVOID*)&lpTranslate,
               &cbTranslate);

            string strKey;
            //for( i=0; i < (cbTranslate/sizeof(struct LANGANDCODEPAGE)); i++ )
            for(int32_t i=0; i < 1; i++ )
            {
               LPTSTR lpsz;
               UINT uiSize;

               strKey.Format(
                  TEXT("\\StringFileInfo\\%04x%04x\\FileDescription"),
                  lpTranslate[i].wLanguage,
                  lpTranslate[i].wCodePage);




               strKey.Format(
                  TEXT("\\StringFileInfo\\%04x%04x\\FileVersion"),
                  lpTranslate[i].wLanguage,
                  lpTranslate[i].wCodePage);

               // Retrieve file description for language and code page "i".
               VerQueryValue(lpdata,
                  (LPTSTR) (const char *) strKey,
                  (LPVOID *)&lpsz,
                  &uiSize);


               string strVersion(lpsz, uiSize);

               return strVersion;
            }
         }
         delete [] (LPBYTE) lpdata;
      }
*/

      return "";

   }


   bool application::Ex2OnAppInstall()
   {
#ifdef IMPLEMENT_VISTA_TOOLS
      if(VistaTools::IsVista())
      {
         if(VistaTools::IsElevated() != S_OK)
         {
            TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
            return false;
         }
      }
#endif
      return true;
   }


   bool application::Ex2OnAppUninstall()
   {

      throw todo(get_app());

// xxx       if(VistaTools::IsVista())
// xxx       {
// xxx          if(VistaTools::IsElevated() != S_OK)
// xxx          {
// xxx             TRACE0("Error! Installing application ( : (un)install run parameter ) without Elevation (required since Vista Windows version)");
// xxx             return false;
// xxx          }
// xxx       }
      return true;
   }

   bool application::DeferRegisterClass(LONG fToRegister, const char ** ppszClass)
   {

      throw todo(get_app());
// xxx       return __end_defer_register_class(fToRegister, ppszClass);
      return false;
   }


   void application::LockTempMaps()
   {
      LNX_THREAD(::ca2::thread::m_p.m_p)->LockTempMaps();
   }

   bool application::UnlockTempMaps(bool bDeleteTemp)
   {
      return LNX_THREAD(::ca2::thread::m_p.m_p)->UnlockTempMaps(bDeleteTemp);
   }


   void application::TermThread(HINSTANCE hInstTerm)
   {
/*      try
      {
   #ifdef DEBUG
         // check for missing ::ca2::LockTempMap calls
         if (__get_module_thread_state()->m_pCurrentWinThread->m_nTempMapLock != 0)
         {
            TRACE(::ca2::trace::category_AppMsg, 0, "Warning: Temp ::collection::map lock count non-zero (%ld).\n",
               __get_module_thread_state()->m_pCurrentWinThread->m_nTempMapLock);
         }
   #endif
         ::ca2::LockTempMaps(::ca2::smart_pointer < ::ca2::application_base >::m_p);
         ::ca2::UnlockTempMaps(::ca2::smart_pointer < ::ca2::application_base >::m_p, -1);
      }
      catch( base_exception* e )
      {
         e->Delete();
      }*/

      try
      {
         // cleanup thread local tooltip ::ca2::window
         if (hInstTerm == NULL)
         {
//            __MODULE_THREAD_STATE* pModuleThreadState = __get_module_thread_state();
         }
      }
      catch( base_exception* e )
      {
         e->Delete();
      }

      try
      {
         // cleanup the rest of the thread local data
         if (__thread_data != NULL)
            __thread_data->delete_data();
      }
      catch( base_exception* e )
      {
         e->Delete();
      }
   }


   const char * application::RegisterWndClass(UINT nClassStyle, HCURSOR hCursor, HBRUSH hbrBackground, HICON hIcon)
   {
      return __register_window_class(nClassStyle, hCursor, hbrBackground, hIcon);
   }




   // application
   HCURSOR application::LoadCursor(const char * lpszResourceName) const
   {
      return NULL;
   }

   HCURSOR application::LoadCursor(UINT nIDResource) const
   {
      return NULL;
   }

   HCURSOR application::LoadStandardCursor(const char * lpszCursorName) const
   {
// xxx       return ::LoadCursor(NULL, lpszCursorName);
return NULL;
   }

   HCURSOR application::LoadOEMCursor(UINT nIDCursor) const
   {

// xxx       return ::LoadCursor(NULL, MAKEINTRESOURCE(nIDCursor));
return NULL;

   }

   HICON application::LoadIcon(const char * lpszResourceName) const
   {
      return NULL;
   }

   HICON application::LoadIcon(UINT nIDResource) const
   {
      return NULL;
   }

   HICON application::LoadStandardIcon(const char * lpszIconName) const
   {
// xxx  return ::LoadIcon(NULL, lpszIconName);
return NULL;
   }

   HICON application::LoadOEMIcon(UINT nIDIcon) const
   {
// xxx       return ::LoadIcon(NULL, MAKEINTRESOURCE(nIDIcon));
return NULL;
   }





   /*void application::construct(__THREADPROC pfnThreadProc, LPVOID pParam)
   {
      ::win::thread::construct(pfnThreadProc, pParam);
   }

   void * application::get_os_data()
   {
      return ::win::thread::get_os_data();
   }

   int_ptr application::get_os_int()
   {
      return ::win::thread::get_os_int();
   }


   bool application::Begin(int32_t nPriority, UINT nStackSize,
                           DWORD dwCreateFlags, LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return ::win::thread::Begin(nPriority, nStackSize, dwCreateFlags, lpSecurityAttrs);
   }

   bool application::create_thread(DWORD dwCreateFlags, UINT nStackSize,
                                  LPSECURITY_ATTRIBUTES lpSecurityAttrs)
   {
      return ::win::thread::create_thread(dwCreateFlags, nStackSize, lpSecurityAttrs);
   }



   int32_t application::GetThreadPriority()
   {
      return ::win::thread::GetThreadPriority();
   }
   bool application::SetThreadPriority(int32_t nPriority)
   {
      return ::win::thread::SetThreadPriority(nPriority);
   }

   // Operations
   DWORD application::SuspendThread()
   {
      return ::win::thread::SuspendThread();
   }
   DWORD application::ResumeThread()
   {
      return ::win::thread::ResumeThread();
   }
   bool application::PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam)
   {
      return ::win::thread::post_thread_message(message, wParam, lParam);
   }
   bool application::post_message(sp(::user::interaction) pguie, UINT message, WPARAM wParam, LPARAM lParam)
   {
      return ::win::thread::post_message(pguie, message, wParam, lParam);
   }

   bool application::PreInitInstance()
   {
      return ::win::thread::PreInitInstance();
   }

   // called when occurs an standard_exception exception in run
   // return true to call run again
   bool application::on_run_exception(::ca2::exception & e)
   {
      return ::win::thread::on_run_exception(e);
   }

   // Overridables
   // thread initialization
   bool application::initialize_instance()
   {
      return ::win::thread::initialize_instance();
   }

   ::ca2::message::e_prototype application::GetMessagePrototype(UINT uiMessage, UINT uiCode)
   {
      return ::win::thread::GetMessagePrototype(uiMessage, uiCode);
   }
post_thread_message
   // running and idle processing
   int32_t application::run()
   {
      return ::win::thread::run();
   }
   bool application::pre_translate_message(::ca2::signal_object * pobj)
   {
      return ::win::thread::pre_translate_message(pMsg);
   }
   bool application::pump_message()    // low level message pump
   {
      return ::win::thread::pump_message();
   }
   bool application::on_idle(LONG lCount) // return TRUE if more idle processing
   {
      return ::win::thread::on_idle(lCount);
   }
   bool application::is_idle_message(MESSAGE* pMsg)  // checks for special messages
   {
      return ::win::thread::is_idle_message(pMsg);
   }
*/
   bool application::process_initialize()
   {
      if(::ca2::application_base::m_p->is_system())
      {
/*
if(__get_module_state()->m_pmapHWND == NULL)
         {
            __get_module_state()->m_pmapHWND = new hwnd_map;
            __get_module_state()->m_pmutexHwnd = new mutex();
         }
         if(__get_module_state()->m_pmapHDC == NULL)
         {
            __get_module_state()->m_pmapHDC = new hdc_map;
         }
         if(__get_module_state()->m_pmapHGDIOBJ == NULL)
         {
            __get_module_state()->m_pmapHGDIOBJ = new hgdiobj_map;
         }
         if(__get_module_state()->m_pmapHMENU == NULL)
         {
            __get_module_state()->m_pmapHMENU = new hmenu_map;
         }
*/
      }


      return true;
   }

   bool application::initialize1()
   {

      ::ca2::thread::m_p->set_run();

      return true;

   }


   bool application::initialize2()
   {
      return true;
   }

   bool application::initialize3()
   {
      return true;
   }

   // thread termination
   int32_t application::exit_instance() // default will 'delete this'
   {

      // avoid calling CloseHandle() on our own thread handle
      // during the thread destructor
      ::ca2::thread::m_p->set_os_data(NULL);

      LNX_THREAD(::ca2::thread::m_p.m_p)->m_bRun = false;
      //LNX_THREAD(::ca2::application_base::m_p->::ca2::thread_sp::m_p)->m_bRun = false;

      int32_t iRet = ::ca2::application::exit_instance();

      //::ca2::smart_pointer<::ca2::application>::destroy();



      return iRet;
   }
/*
   // Advanced: exception handling
   LRESULT application::ProcessWndProcException(base_exception* e, const MESSAGE* pMsg)
   {
      return   ::win::thread::ProcessWndProcException(e, pMsg);
   }


   // Advanced: handling messages sent to message filter hook
   bool application::ProcessMessageFilter(int32_t code, LPMESSAGE lpMsg)
   {
      return  ::win::thread::ProcessMessageFilter(code, lpMsg);
   }


   // Advanced: access to GetMainWnd()
   sp(::user::interaction) application::GetMainWnd()
   {
      return ::win::thread::GetMainWnd();
   }



   void application::assert_valid() const
   {
      ::win::thread::assert_valid();
   }

   void application::dump(dump_context & dumpcontext) const
   {
      ::win::thread::dump(dumpcontext);
   }


   void application::CommonConstruct()
   {
      ::win::thread::CommonConstruct();
   }
   void application::Delete()
   {
      ::win::thread::Delete();
   }
   // 'delete this' only if m_bAutoDelete == TRUE

   bool application::DispatchThreadMessageEx(MESSAGE* msg)  // helper
   {
      return ::win::thread::DispatchThreadMessageEx(msg);
   }*/

/*   ::draw2d::graphics * application::graphics_from_os_data(void * pdata)
   {
      return ::win::graphics::from_handle((HDC) pdata);
   }*/

   void application::ShowWaitCursor(bool bShow)
   {

      mutex_lock mlUser(user_mutex(), true);

      mutex_lock mlOsWindow(*::oswindow_data::s_pmutex, true);

      unsigned int uiShape;

      if(bShow)
      {

         uiShape = XC_watch;

      }
      else
      {

         uiShape = XC_arrow;

      }

      for(int i = 0; i < ::oswindow_data::s_pdataptra->get_count(); i++)
      {

         oswindow window = ::oswindow_data::s_pdataptra->element_at(i);

         if(window->m_bMessageOnlyWindow)
            continue;

         if(window->display() == NULL)
            continue;

         Cursor cursor = XCreateFontCursor(window->display(), uiShape);

         XDefineCursor(window->display(), window->window(), cursor);

      }

   }

   sp(::ca2::window) application::window_from_os_data(void * pdata)
   {
      return ::lnx::window::from_handle((oswindow) pdata);
   }

   sp(::ca2::window) application::window_from_os_data_permanent(void * pdata)
   {
      sp(::ca2::window) pwnd = ::lnx::window::FromHandlePermanent((oswindow) pdata);
      if(pwnd != NULL)
         return pwnd;
      user::interaction_ptr_array wndptra = System.frames();
      for(int32_t i = 0; i < wndptra.get_count(); i++)
      {
         if(wndptra[i].get_safe_handle() == (oswindow) pdata)
         {
            return wndptra[i].get_wnd();
         }
      }
      return NULL;
   }

   ::ca2::thread * application::GetThread()
   {
      if(__get_thread() == NULL)
         return NULL;
      else
         return dynamic_cast < ::ca2::thread * > (__get_thread()->m_p.m_p);
   }

   void application::set_thread(::ca2::thread * pthread)
   {
      __set_thread(pthread);
   }

   ///////////////////////////////////////////////////////////////////////////
   // application Initialization

   void application::SetCurrentHandles()
   {
      //ASSERT(this == afxCurrentWinApp);
      //if(afxCurrentAppName != NULL)
        // return;
      //ASSERT(afxCurrentAppName == NULL);


      // Note: there are a number of _tcsdup (aka _strdup) calls that are
      // made here for the exe path, help file path, etc.  In previous
      // versions of ca2 API, this primitive::memory was never freed.  In this and future
      // versions this primitive::memory is automatically freed during application's
      // destructor.  If you are freeing the primitive::memory yourself, you should
      // either remove the code or set the pointers to NULL after freeing
      // the primitive::memory.

      // get path of executable
   /*   char szBuff[_MAX_PATH];
      DWORD dwRet = ::GetModuleFileName(m_hInstance, szBuff, _MAX_PATH);
      ASSERT( dwRet != 0 && dwRet != _MAX_PATH );
      if( dwRet == 0 || dwRet == _MAX_PATH )
         throw user_exception();*/

      /*
      LPTSTR lpszExt = ::PathFindExtension(szBuff);
      ASSERT(lpszExt != NULL);
      if( lpszExt == NULL )
         throw user_exception();

      ASSERT(*lpszExt == '.');
      *lpszExt = 0;       // no suffix
      */

      string strExeName;
      //string strTitle = System.load_string("System.title");
      // get the exe title from the full path name [no extension]
      strExeName = System.get_module_title();

      __get_module_state()->m_lpszCurrentAppName = strdup(m_strAppName);

      // initialize thread state
      __MODULE_STATE* pModuleState = __get_module_state();
      ENSURE(pModuleState);
      if(pModuleState->m_pCurrentWinApp == NULL)
      {
         __MODULE_THREAD_STATE* pThreadState = pModuleState->m_thread;
         ENSURE(pThreadState);
//         ASSERT(System.GetThread() == NULL);
         pThreadState->m_pCurrentWinThread = dynamic_cast < class ::lnx::thread * > (::ca2::thread::m_p.m_p);
  //       ASSERT(System.GetThread() == this);

         // initialize application state
         //ASSERT(afxCurrentWinApp == NULL); // only one application object please
         pModuleState->m_pCurrentWinApp = dynamic_cast < application * > (this);
         //ASSERT(&System == this);
      }


//      dynamic_cast < ::lnx::thread * > ((smart_pointer < ::ca2::application >::m_p->::ca2::thread_sp::m_p))->m_hThread = __get_thread()->m_hThread;
  //    dynamic_cast < ::lnx::thread * > ((smart_pointer < ::ca2::application >::m_p->::ca2::thread_sp::m_p))->m_nThreadID = __get_thread()->m_nThreadID;
      dynamic_cast < class ::lnx::thread * > (::ca2::thread::m_p.m_p)->m_hThread      =  ::GetCurrentThread();


   }

   sp(::ca2::window) application::FindWindow(const char * lpszClassName, const char * lpszWindowName)
   {
      return window::FindWindow(lpszClassName, lpszWindowName);
   }

   sp(::ca2::window) application::FindWindowEx(oswindow hwndParent, oswindow hwndChildAfter, const char * lpszClass, const char * lpszWindow)
   {
      return window::FindWindowEx(hwndParent, hwndChildAfter, lpszClass, lpszWindow);
   }


   void application::get_time(struct timeval *p)
   {
   #ifdef _WIN32
      FILETIME ft; // Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
      GetSystemTimeAsFileTime(&ft);
      uint64_t tt;
      memcpy(&tt, &ft, sizeof(tt));
      tt /= 10; // make it usecs
      p->tv_sec = (long)tt / 1000000;
      p->tv_usec = (long)tt % 1000000;
   #else
      gettimeofday(p, NULL);
   #endif
   }

   void application::set_env_var(const string & var,const string & value)
   {
   #if (defined(SOLARIS8) || defined(SOLARIS))
      {
         static std::collection::map<string, char *> vmap;
         if (vmap.find(var) != vmap.end())
         {
            delete[] vmap[var];
         }
         vmap[var] = new char[var.get_length() + 1 + value.get_length() + 1];
         sprintf(vmap[var], "%s=%s", var, value);
         putenv( vmap[var] );
      }
   #elif defined _WIN32
      {
         string slask = var + "=" + value;
         _putenv( (const char *)slask);
      }
   #else
      setenv(var, value, 1);
   #endif
   }

   uint32_t application::get_thread_id()
   {
      return ::pthread_self();
   }



   bool application::set_main_init_data(::ca2::main_init_data * pdata)
   {

      m_pmaininitdata = (::lnx::main_init_data *) pdata;

      if(m_pmaininitdata != NULL && ::ca2::application_base::m_p->is_system())
      {
         if(!win_init(m_pmaininitdata))
            return false;
      }

      return true;

   }

   bool application::win_init(main_init_data * pdata)
   {
         ASSERT(pdata->m_hPrevInstance == NULL);

         HINSTANCE hInstance        = pdata->m_hInstance;
//         HINSTANCE hPrevInstance    = pdata->m_hPrevInstance;
         string strCmdLine          = pdata->m_vssCommandLine;
         UINT nCmdShow              = pdata->m_nCmdShow;

         // handle critical errors and avoid Windows message boxes
// xxx         SetErrorMode(SetErrorMode(0) | SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);

         // set resource handles
         __MODULE_STATE* pModuleState = __get_module_state();
         pModuleState->m_hCurrentInstanceHandle = hInstance;
         pModuleState->m_hCurrentResourceHandle = hInstance;
         pModuleState->CreateActivationContext();

         // fill in the initial state for the application
         // Windows specific initialization (not done if no application)
// xxx         m_hInstance = hInstance;
// xxx          (dynamic_cast < sp(::ca2::application) >(m_papp))->m_hInstance = hInstance;
         //hPrevInstance; // Obsolete.
         m_strCmdLine = strCmdLine;
         m_nCmdShow = nCmdShow;
         //pApp->SetCurrentHandles();
         SetCurrentHandles();

         // initialize thread specific data (for main thread)
         if (!afxContextIsDLL)
            __init_thread();

         // Initialize ::ca2::window::m_pfnNotifyWinEvent
      /*   HMODULE hModule = ::GetModuleHandle("user32.dll");
         if (hModule != NULL)
         {
            ::ca2::window::m_pfnNotifyWinEvent = (::ca2::window::PFNNOTIFYWINEVENT)::GetProcaddress(hModule, "NotifyWinEvent");
         }*/

      return true;

   }


   bool application::update_module_paths()
   {

         {

            if(!br_init_lib(NULL))
               return false;

            char * lpszModuleFolder = br_find_exe_dir(NULL);

            if(lpszModuleFolder == NULL)
               return false;

            m_strModuleFolder = lpszModuleFolder;

            free(lpszModuleFolder);

         }

         {

            void * handle = dlopen("libca.so", RTLD_NOW);

            if(handle == NULL)
            {

               m_strCa2ModuleFolder = m_strModuleFolder;

            }
            else
            {

                link_map * plm;

                dlinfo(handle, RTLD_DI_LINKMAP, &plm);

                m_strCa2ModuleFolder = ::dir::name(plm->l_name);

                if(m_strCa2ModuleFolder.is_empty() || m_strCa2ModuleFolder[0] != '/')
                {

                    m_strCa2ModuleFolder = m_strModuleFolder;

                }

                dlclose(handle);

            }

         }

		return true;

   }

   string application::draw2d_get_default_library_name()
   {

         return "draw2d_cairo";

   }

} // namespace lnx
