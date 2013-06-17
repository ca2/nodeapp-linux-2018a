#include "framework.h"


namespace lnx
{


   factory_exchange::factory_exchange(sp(::ca2::application) papp) :
      ca2(papp)
   {

      System.factory().cloneable_large <stdio_file             >  (System. type_info < ::ca2::text_file         > ());
      System.factory().cloneable_large < file                  >  (System. type_info < ::ca2::file              > ());
      System.factory().cloneable_large < file_set              >  (System. type_info < ::ca2::file_set          > ());
      System.factory().cloneable < file_system                 >  (System. type_info < ::ca2::file_system       > (), 1);
      System.factory().cloneable < application                 >  (System. type_info < ::ca2::application_base  > (), 1);
      System.factory().cloneable < dir                         >  (System. type_info < ::ca2::dir::system        > (), 1);
      System.factory().cloneable_large < dib                   >  (System. type_info < ::ca2::dib                > ());
      System.factory().cloneable_large < graphics              >  (System. type_info < ::ca2::graphics           > ());
      System.factory().cloneable_large < bitmap                >  (System. type_info < ::ca2::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System. type_info < ::ca2::pen                > ());
      System.factory().cloneable_large < brush                 >  (System. type_info < ::ca2::brush              > ());
      System.factory().cloneable_large < palette               >  (System. type_info < ::ca2::palette            > ());
      System.factory().cloneable_large < region                >  (System. type_info < ::ca2::region             > ());
      System.factory().cloneable_large < font                  >  (System. type_info < ::ca2::font               > ());
      System.factory().cloneable_large < graphics_path         >  (System. type_info < ::ca2::graphics_path      > ());
      System.factory().creatable < window_draw                 >  (System. type_info < ::ca2::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System. type_info < ::ca2::thread             > ());
      System.factory().creatable_large < window                >  (System. type_info < ::ca2::window             > ());
      System.factory().creatable < os                          >  (System. type_info < ::ca2::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System. type_info < ::ca2::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System. type_info < ::ca2::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System. type_info < ::ca2::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System. type_info < ::ca2::ip_enum           > ());

      ::lnx::dib::s_initialize();

   }


   factory_exchange::~factory_exchange()
   {

   }


} // namespace win


extern "C"
void ca2_factory_exchange(sp(::ca2::application) papp)
{
   lnx::factory_exchange factoryexchange(papp);
}


