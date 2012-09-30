#pragma once


namespace lnx
{


   class CLASS_DECL_lnx folder_watch :
      virtual public ::ex2::folder_watch,
      virtual public ::radix::thread
   {
   public:


      folder_watch(::ca::application * papp);

      virtual bool watch(const char * pszPath);

      virtual e_action translate_os_action(int iAction);

      virtual int run();


   };


} // namespace lnx



