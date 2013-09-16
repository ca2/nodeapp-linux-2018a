#include "framework.h"


CLASS_DECL_ca int32_t ca2_main();

//CLASS_DECL_ca extern fixed_alloc_array * g_pfixedallocaWstring;

//CLASS_DECL_ca fixed_alloc_array * new_wstring_manager();

void CLASS_DECL_lnx __cdecl _ca2_purecall()
{
    throw simple_exception(::get_thread_app());
}


void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers);


int xlib_error_handler(Display * d, XErrorEvent * e)
{

   if(e->request_code == 12) //
   {

      if(e->error_code == BadValue)
      {
      }

   }
   char sz[1024];
   XGetErrorText(d, e->error_code, sz, sizeof(sz));
   fputs(sz, stderr);

   abort();

}


//sp(base_application)     win_application_create(sp(base_application) pappSystem, const char * pszId);
//sp(base_application)     win_instantiate_application(sp(base_application) pappSystem, const char * pszId);
//int32_t                     win_application_procedure(sp(base_application) pappSystem)
//UINT c_cdecl          win_application_thread_proc(LPVOID);
/////////////////////////////////////////////////////////////////////////////
// Standard WinMain implementation
//  can be replaced as long as '::ca2::WinInit' is called first

extern char gen_InitAppState;

CLASS_DECL_lnx bool __initialize(bool bDLL, DWORD dwVersion);

int32_t CLASS_DECL_lnx __lnx_main(int32_t argc, char * argv[])
{

   if(!XInitThreads())
      return -1;


   gen_InitAppState = (char)(__initialize(FALSE, 1));

//   UNREFERENCED_PARAMETER(lpCmdLine);

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


   ::lnx::main_init_data * pinitmaindata  = new ::lnx::main_init_data;


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


   set_heap_mutex(NULL);

   return nReturnCode;


}


int32_t CLASS_DECL_lnx ca2_main(HINSTANCE hInstance, HINSTANCE hPrevInstance, const char * lpCmdLine, int32_t nCmdShow)
{
   try
   {

//      _set_purecall_handler(_ca2_purecall);

      ::plane::system * psystem = new ::plane::system();


      //Sleep(30 * 1000);


      ASSERT(hPrevInstance == NULL);

      int32_t nReturnCode = 0;

      ::lnx::main_init_data * pinitmaindata  = new ::lnx::main_init_data;

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

      set_heap_mutex(NULL);


      return nReturnCode;
   }
   catch(...)
   {
   }
   return -1;
}



void __cdecl _null_se_translator(uint32_t uiCode, EXCEPTION_POINTERS * ppointers)
{
   UNREFERENCED_PARAMETER(uiCode);
   UNREFERENCED_PARAMETER(ppointers);
}
