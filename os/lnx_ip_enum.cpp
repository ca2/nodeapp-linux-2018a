#include "framework.h"
#include <netdb.h>


namespace lnx
{


   ip_enum::ip_enum(::ca::application * papp) :
      ca(papp),
      ::ca4::ip_enum(papp)
   {

   }


   ip_enum::~ip_enum()
   {

   }


   ::count ip_enum::enumerate(ip_array & ipa)
   {
      //get this machines host name
      char szHostname[256];
      if (gethostname(szHostname, sizeof(szHostname)))
      {
         TRACE("Failed in call to gethostname, errno returns %d\n", errno);
         return FALSE;
      }

      //get host information from the host name
      hostent * phostent = gethostbyname(szHostname);
      if (phostent == NULL)
      {
         TRACE("Failed in call to gethostbyname, errno returns %d\n", errno);
         return FALSE;
      }

      //check the length of the IP adress
      if (phostent->h_length != 4)
      {
         TRACE("IP address returned is not 32 bits !!\n");
         return FALSE;
      }

      //call the virtual callback function in a loop

      int nAdapter = 0;

      while(phostent->h_addr_list[nAdapter])
      {

         in_addr addr;

         memcpy(&addr.s_addr, phostent->h_addr_list[nAdapter], phostent->h_length);

         ipa.add(ip_item(nAdapter, addr));

         nAdapter++;

      }

      return TRUE;
   }

   ::count ip_enum::enumerate(stringa & stra)
   {

      ip_array ipa;

      ::count c = enumerate(ipa);

      for(int i = 0; i < ipa.get_count(); i++)
      {

         stra.add(inet_ntoa(ipa[i].m_addr));

      }

      return c;

   }


} // namespace win


