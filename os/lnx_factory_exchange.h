#pragma once


namespace lnx
{

   class CLASS_DECL_lnx factory_exchange :
      virtual public ::ca2::object
   {
   public:
      factory_exchange(sp(::ca2::application) papp);
      virtual ~factory_exchange();
   };


} // namespace lnx

