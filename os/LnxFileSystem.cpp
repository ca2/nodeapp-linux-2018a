#include "framework.h"
#include "lnx_fileSystem.h"


#include "lnx_file.h"

lnx_fileSystem::lnx_fileSystem(::ca::application * papp) :
   ca(papp)
{

}

lnx_fileSystem::~lnx_fileSystem()
{

}

bool lnx_fileSystem::FullPath(string &str, const char * lpszFileIn)
{
   if(::ex1::file_system::FullPath(str, lpszFileIn))
      return true;
   if(gen::str::begins_ci(lpszFileIn, "http://"))
   {
      str = lpszFileIn;
      return true;
   }
   else if(gen::str::begins_ci(lpszFileIn, "https://"))
   {
      str = lpszFileIn;
      return true;
   }
   wstring wstrFileIn;
   wstrFileIn = gen::international::utf8_to_unicode(lpszFileIn);
   wstring wstrFileOut;
   bool b = vfxFullPath(wstrFileOut.alloc(MAX_PATH * 8), wstrFileIn) != FALSE;
   if(b)
   {
      gen::international::unicode_to_utf8(str, wstrFileOut);
   }
   return b;
}


UINT lnx_fileSystem::GetFileName(const char * lpszPathName, string & str)
{
   int nMax = MAX_PATH * 8;
   wstring wstrPathName;
   wstrPathName = gen::international::utf8_to_unicode(lpszPathName);
   wstring wstrTitle;
   UINT user = vfxGetFileName(wstrPathName, wstrTitle.alloc(nMax), nMax);
   str = gen::international::unicode_to_utf8(wstrTitle);
   return user;
}

void lnx_fileSystem::GetModuleShortFileName(HINSTANCE hInst, string & strShortName)
{
   vfxGetModuleShortFileName(hInst, strShortName);
}
