#include "nodeapp/os/linux.h"

const char * g_psz_br_init_symbol_app = "";

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
