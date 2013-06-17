#pragma once


namespace lnx
{


   class CLASS_DECL_lnx font :
      virtual public ::ca2::font
   {
   public:


      font(sp(::ca2::application) papp);
      virtual ~font();


      virtual void * get_os_data() const;

      virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace lnx



