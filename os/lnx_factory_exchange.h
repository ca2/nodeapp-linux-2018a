#pragma once


namespace lnx
{

   class CLASS_DECL_lnx factory_exchange :
      virtual public ::object
   {
   public:
      factory_exchange(sp(base_application) papp);
      virtual ~factory_exchange();
   };


} // namespace lnx

