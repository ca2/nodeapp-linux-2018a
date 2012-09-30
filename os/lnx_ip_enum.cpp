#include "framework.h"


namespace win
{


   ip_enum::ip_enum(::ca::application * papp) :
      ca(papp),
      ::ca4::ip_enum(papp)
   {

      m_bWinsockInitialized = false;

      //Initialise the winsock stack
      WORD wVersionRequested = MAKEWORD(1, 1);
      WSADATA wsaData;
      int err = WSAStartup(wVersionRequested, &wsaData);
      if (err != 0)
      {
         TRACE("Failed in call to WSAStartup, return value was %d\n", err);
         throw not_supported_exception();
      }

      //remember that we have opened winsock
      m_bWinsockInitialized =  true;

      //Code requires at least Winsock 1.1
      if ((LOBYTE(wsaData.wVersion) != 1) || (HIBYTE(wsaData.wVersion) != 1))
      {
         TRACE("Failed to find a usable winsock stack which supports Winsock 1.1\n");
         throw not_supported_exception();
      }
   }


   ip_enum::~ip_enum()
   {
      //release our use of winsock stack if we successfully initialised it
      if (m_bWinsockInitialized)
         WSACleanup();
   }


   ::count ip_enum::enumerate(ip_array & ipa)
   {
      //get this machines host name
      char szHostname[256];
      if (gethostname(szHostname, sizeof(szHostname)))
      {
         TRACE("Failed in call to gethostname, WSAGetLastError returns %d\n", WSAGetLastError());
         return FALSE;
      }

      //get host information from the host name
      HOSTENT* pHostEnt = gethostbyname(szHostname);
      if (pHostEnt == NULL)
      {
         TRACE("Failed in call to gethostbyname, WSAGetLastError returns %d\n", WSAGetLastError());
         return FALSE;
      }

      //check the length of the IP adress
      if (pHostEnt->h_length != 4)
      {
         TRACE("IP address returned is not 32 bits !!\n");
         return FALSE;
      }

      //call the virtual callback function in a loop

      int nAdapter = 0;

      while(pHostEnt->h_addr_list[nAdapter])
      {

         in_addr addr;

         CopyMemory(&addr.S_un.S_addr, pHostEnt->h_addr_list[nAdapter], pHostEnt->h_length);

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


