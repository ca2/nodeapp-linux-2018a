/*
Module : IPENUM.H
Purpose: Interface for an ca2 API class wrapper for IP address enumeration
Created: PJN / 21-04-1998
History: None

Copyright (c) 1998 by PJ Naughter.


*/


namespace lnx
{


   class CLASS_DECL_lnx ip_enum :
      virtual public ::ca4::ip_enum
   {
   public:


      ip_enum(::ca::application * papp);
      virtual ~ip_enum();


      virtual ::count enumerate(stringa & stra);


   };


} // namespace lnx




