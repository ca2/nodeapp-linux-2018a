#pragma once


namespace lnx
{

   class CLASS_DECL_lnx factory_exchange :
      virtual public ::ca::object
   {
   public:
      factory_exchange(::ca::application * papp);
      virtual ~factory_exchange();
   };


} // namespace lnx

