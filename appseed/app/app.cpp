#include "framework.h"




/////////////////////////////////////////////////////////////////////////////
// export WinMain to force linkage to this module
extern int CLASS_DECL_win __win_main(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int nCmdShow);

extern "C" int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
   __in LPTSTR lpCmdLine, int nCmdShow)
{
   // call shared/exported WinMain
   return __win_main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
