#include "framework.h"


namespace lnx
{


   factory_exchange::factory_exchange(::ca::application * papp) :
      ca(papp)
   {
      System.factory().cloneable_large <stdio_file          >  (System. type_info < ::ex1::text_file         > ());
      System.factory().cloneable_large < file               >  (System. type_info < ::ex1::file              > ());
      System.factory().cloneable_large < file_set            >  (System. type_info < ::ex2::file_set          > ());
      System.factory().cloneable < file_system               >  (System. type_info < ::ex1::file_system       > (), 1);
      System.factory().cloneable < WinResource                 >  (System. type_info < ::ex1::resource          > (), 1);
      System.factory().cloneable < application                 >  (System. type_info < ::ex2::application       > (), 1);
      System.factory().cloneable < dir                         >  (System. type_info < ::ca::dir::system        > (), 1);
      System.factory().cloneable_large < dib                   >  (System. type_info < ::ca::dib                > ());
      System.factory().cloneable_large < graphics              >  (System. type_info < ::ca::graphics           > ());
      System.factory().cloneable_large < bitmap                >  (System. type_info < ::ca::bitmap             > ());
      System.factory().cloneable_large < pen                   >  (System. type_info < ::ca::pen                > ());
      System.factory().cloneable_large < brush                 >  (System. type_info < ::ca::brush              > ());
      System.factory().cloneable_large < palette               >  (System. type_info < ::ca::palette            > ());
      System.factory().cloneable_large < region                >  (System. type_info < ::ca::region             > ());
      System.factory().cloneable_large < font                  >  (System. type_info < ::ca::font               > ());
      System.factory().cloneable_large < graphics_path         >  (System. type_info < ::ca::graphics_path      > ());
      System.factory().creatable < window_draw                 >  (System. type_info < ::ca::window_draw        > (), 1);
      System.factory().creatable_large < thread                >  (System. type_info < ::ca::thread             > ());
      System.factory().creatable_large < window                >  (System. type_info < ::ca::window             > ());
//xxx      System.factory().cloneable_small < draw_dib              >  (System. type_info < ::ca::draw_dib           > ());
      System.factory().creatable < os                          >  (System. type_info < ::ca::os                 > (), 1);
      System.factory().creatable < port_forward                >  (System. type_info < ::ca4::port_forward      > (), 1);
      System.factory().creatable_small < copydesk              >  (System. type_info < ::ca4::copydesk          > (), 1);
      System.factory().creatable_small < crypt                 >  (System. type_info < ::ca4::crypt             > (), 1);
      System.factory().creatable_small < ip_enum               >  (System. type_info < ::ca4::ip_enum           > ());
      ::lnx::dib::s_initialize();
   }


   factory_exchange::~factory_exchange()
   {
   }


} // namespace win


extern "C"
void ca2_factory_exchange(::ca::application * papp)
{
   lnx::factory_exchange factoryexchange(papp);
}


