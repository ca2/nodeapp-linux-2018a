#include "nodeapp/os/lnx.h"


const char * g_psz_br_init_symbol_app = "";


int32_t main(int32_t argc, char * argv[])
{
    br_init_set_symbol(g_psz_br_init_symbol_app);
   return __lnx_main(argc, argv);
}


