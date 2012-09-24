#pragma once


namespace lnx
{


   class CLASS_DECL_lnx os :
      public ::ca::os
   {
   public:


      os(::ca::application * papp);
      virtual ~os();


      virtual bool reboot();
      virtual bool shutdown(bool bPowerOff);

      virtual void terminate_processes_by_title(const char * pszName);
      virtual string get_module_path(HMODULE hmodule);
      virtual bool get_pid_by_path(const char * pszName, DWORD & dwPid);
      virtual bool get_pid_by_title(const char * pszName, DWORD & dwPid);
      virtual void get_all_processes(dword_array & dwa);
      virtual string get_process_path(DWORD dwPid);


   };


} // namespace lnx




