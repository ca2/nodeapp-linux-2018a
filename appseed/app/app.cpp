#include "app/appseed/core/core.h"

const char * g_psz_br_init_symbol_app = "";


int32_t CLASS_DECL_LINUX __lnx_main(int32_t argc, char * argv[]);


int32_t main(int32_t argc, char * argv[])
{

   br_init_set_symbol(g_psz_br_init_symbol_app);

   return __lnx_main(argc, argv);

}


bool CLASS_DECL_LINUX __init_current_state_app()
{
   sp(::base::application) pApp = __get_module_state()->m_pCurrentWinApp;
   if (pApp != NULL && !pApp->m_pplaneapp->initialize_instance())
   {
      // Init Failed
      try
      {
         pApp->m_pplaneapp->exit();
      }
      catch(...)
      {
      }
      __lnx_term();
      return FALSE;
   }
   return TRUE;
}




int32_t CLASS_DECL_LINUX __lnx_main(int32_t argc, char * argv[])
{

   if(!XInitThreads())
      return -1;


   c_xstart();



   XSetErrorHandler(xlib_error_handler);


//   ::CoInitialize(NULL);
//{
//int * pi = NULL;
//*pi = 0;
//}

//throw todo(::get_thread_app());

   if(!main_initialize())
      return -1;


//   _set_purecall_handler(_ca2_purecall);

   ::plane::system * psystem = new ::plane::system();

    psystem->::exception::translator::attach();

           //sigset_t set;
           //sigemptyset(&set);
           //sigaddset(&set, SIGSEGV);
           //sigprocmask(SIG_BLOCK, &set, ::null());

//{
//int * pi = NULL;
//*pi = 0;
//}


//   ASSERT(hPrevInstance == NULL);

   int32_t nReturnCode = 0;


   ::linux::main_init_data * pinitmaindata  = new ::linux::main_init_data;


   pinitmaindata->m_hInstance             = NULL;
   pinitmaindata->m_hPrevInstance         = NULL;
   if(argc > 0)
   {
      pinitmaindata->m_vssCommandLine     = argv[0];
   }
   bool bColon = false;
   for(int32_t i = 1; i < argc; i++)
   {
      if(bColon || (bColon = (strcmp(argv[i], ":") == 0)))
      {
         pinitmaindata->m_vssCommandLine     += " ";
         pinitmaindata->m_vssCommandLine     += argv[i];

      }
      else
      {
         pinitmaindata->m_vssCommandLine     += " \"";
         pinitmaindata->m_vssCommandLine     += argv[i];
         pinitmaindata->m_vssCommandLine     += "\"";
      }
   }
   pinitmaindata->m_nCmdShow              = SW_SHOW;


   psystem->init_main_data(pinitmaindata);

   //MessageBox(NULL, "box1", "box1", MB_ICONINFORMATION);

   //::draw2d_xlib::factory_exchange f(psystem);

   nReturnCode = psystem->main();

   try
   {
      main_finalize();
   }
   catch(...)
   {
   }

   try
   {
      __lnx_term();
   }
   catch(...)
   {
   }


   try
   {
      delete psystem;
   }
   catch(...)
   {
   }

   psystem = NULL;


   try
   {
//      delete __get_module_state()->m_pmapHWND;
   }
   catch(...)
   {
   }
/*   try
   {
      delete __get_module_state()->m_pmapHDC;
   }
   catch(...)
   {
   }*/
/*   try
   {
      delete __get_module_state()->m_pmapHGDIOBJ;
   }
   catch(...)
   {
   }*/
//      delete __get_module_state()->m_pmapHMENU;

   try
   {
      __get_module_state()->m_pmapHWND     = NULL;
   }
   catch(...)
   {
   }
/*   try
   {
      __get_module_state()->m_pmapHDC      = NULL;
   }
   catch(...)
   {
   }*/
/*   try
   {
      __get_module_state()->m_pmapHGDIOBJ  = NULL;
   }
   catch(...)
   {
   }*/


//   set_heap_mutex(NULL);

   return nReturnCode;


}



int32_t CLASS_DECL_LINUX ca2_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char * lpCmdLine, int32_t nCmdShow)
{
   try
   {

//      _set_purecall_handler(_ca2_purecall);

      ::plane::system * psystem = new ::plane::system();


      //Sleep(30 * 1000);


      ASSERT(hPrevInstance == NULL);

      int32_t nReturnCode = 0;

      ::linux::main_init_data * pinitmaindata  = new ::linux::main_init_data;

      pinitmaindata->m_hInstance             = hInstance;
      pinitmaindata->m_hPrevInstance         = hPrevInstance;
      pinitmaindata->m_vssCommandLine        = lpCmdLine;
      pinitmaindata->m_nCmdShow              = nCmdShow;


      psystem->init_main_data(pinitmaindata);


      nReturnCode = psystem->main();


      __lnx_term();


      try
      {
         delete psystem;
      }
      catch(...)
      {
      }

      psystem = NULL;

      try
      {
//         delete __get_module_state()->m_pmapHWND;
      }
      catch(...)
      {
      }
/*      try
      {
         delete __get_module_state()->m_pmapHDC;
      }
      catch(...)
      {
      }*/
/*      try
      {
         delete __get_module_state()->m_pmapHGDIOBJ;
      }
      catch(...)
      {
      }*/
//      delete __get_module_state()->m_pmapHMENU;
      try
      {
         __get_module_state()->m_pmapHWND     = NULL;
      }
      catch(...)
      {
      }
/*      try
      {
         __get_module_state()->m_pmapHDC      = NULL;
      }
      catch(...)
      {
      }
      try
      {
         __get_module_state()->m_pmapHGDIOBJ  = NULL;
      }
      catch(...)
      {
      }*/

//      set_heap_mutex(NULL);


      return nReturnCode;
   }
   catch(...)
   {
   }
   return -1;
}
