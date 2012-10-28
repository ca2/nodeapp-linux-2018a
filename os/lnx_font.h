#pragma once


namespace lnx
{


   class CLASS_DECL_lnx font :
      virtual public ::ca::font
   {
   public:


      font(::ca::application * papp);
      virtual ~font();


      virtual void * get_os_data() const;

      virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace win



