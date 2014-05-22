#include "app/appseed/core/core.h"

const char * g_psz_br_init_symbol_app = "";


int32_t CLASS_DECL_LINUX __lnx_main(int32_t argc, char * argv[]);


int32_t main(int32_t argc, char * argv[])
{

   ::base::static_start::init();

   br_init_set_symbol(g_psz_br_init_symbol_app);

   return __lnx_main(argc, argv);

}

int xlib_error_handler(Display * d, XErrorEvent * e);


int32_t CLASS_DECL_LINUX __lnx_main(int32_t argc, char * argv[])
{

   if(!XInitThreads())
      return -1;

   c_xstart();

   XSetErrorHandler(xlib_error_handler);

   if(!defer_core_init())
      return -1;

   ::plane::system * psystem = new ::plane::system();

    psystem->::exception::translator::attach();

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

   nReturnCode = psystem->main();

   try
   {

      delete psystem;

   }
   catch(...)
   {

   }

   psystem = NULL;


   defer_core_term();

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


      try
      {
         delete psystem;
      }
      catch(...)
      {
      }

      psystem = NULL;

      defer_base_term();

      return nReturnCode;
   }
   catch(...)
   {
   }
   return -1;
}
