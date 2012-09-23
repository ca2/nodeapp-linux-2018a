#pragma once

CLASS_DECL_LNX WINBOOL vfxFullPath(wchar_t * lpszPathOut, const wchar_t * lpszFileIn);
CLASS_DECL_LNX UINT vfxGetFileName(const wchar_t * lpszPathName, wchar_t * lpszTitle, UINT nMax);
CLASS_DECL_LNX void vfxGetModuleShortFileName(HINSTANCE hInst, string & strShortName);
CLASS_DECL_LNX void vfxGetModuleShortFileName(HINSTANCE hInst, string& strShortName);

class lnx_fileSystem :
   virtual public ex1::file_system
{
public:
   lnx_fileSystem(::ca::application * papp);
   virtual ~lnx_fileSystem();

   virtual bool FullPath(string & str, const char * lpszFileIn);
   virtual UINT GetFileName(const char * lpszPathName, string & str);
   virtual void GetModuleShortFileName(HINSTANCE hInst, string & strShortName);

};
