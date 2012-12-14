#pragma once


//CLASS_DECL_lnx WINBOOL vfxFullPath(wchar_t * lpszPathOut, const wchar_t * lpszFileIn);
//CLASS_DECL_lnx UINT vfxGetFileName(const wchar_t * lpszPathName, wchar_t * lpszTitle, UINT nMax);
//CLASS_DECL_lnx void vfxGetModuleShortFileName(HINSTANCE hInst, string & strShortName);
//CLASS_DECL_lnx void vfxGetModuleShortFileName(HINSTANCE hInst, string& strShortName);


namespace lnx
{


   class file_system :
      virtual public ex1::file_system
   {
   public:


      file_system(::ca::application * papp);
      virtual ~file_system();


//    virtual bool FullPath(string & str, const char * lpszFileIn);
//    virtual UINT GetFileName(const char * lpszPathName, string & str);
//    virtual void GetModuleShortFileName(HINSTANCE hInst, string & strShortName);


   };


} // namespace lnx




