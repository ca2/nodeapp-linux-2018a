#include "framework.h"


namespace lnx
{


   crypt::crypt(::ca::application * papp) :
      ca(papp),
      ::ca4::crypt(papp)
   {
   }


   crypt::~crypt()
   {
   }


   string crypt::get_crypt_key_file_path()
   {

      return System.dir().path(getenv("home"), ".ca2/cryptkey");

   }

/*
   bool crypt::decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt)
   {

      string str = defer_get_cryptkey();

      if(str.is_empty())
         return false;

      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if(pszSalt == NULL)
         pszSalt = "";

      DATA_BLOB DataSalt;
      primitive::memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (DWORD) memorySalt.get_size();

      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = storageEncrypt.get_data();
      DataIn.cbData = (DWORD) storageEncrypt.get_size();

      wchar_t * lpwsz = NULL;

      //--------------------------------------------------------------------
      //  Begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if(CryptUnprotectData(
         &DataIn,
         NULL, // A description string
                                            // to be included with the
                                            // encrypted data.
         &DataSalt,                               // Optional entropy not used.
         NULL,                               // Reserved.
         NULL,                               // Pass NULL for the
                                            // prompt structure.
         0,
         &DataOut))
      {
         TRACE("crypt::decrypt The encryption phase worked. \n");
         storageDecrypt.allocate(DataOut.cbData);
         memcpy(storageDecrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(lpwsz);
         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         TRACELASTERROR();
         TRACE("crypt::decrypt Decryption error! (1)");
         return false;
      }
   }

   bool crypt::encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt)
   {


      string str = defer_get_cryptkey();

      if(str.is_empty())
         return false;

      DATA_BLOB DataIn;
      DATA_BLOB DataOut;

      if(pszSalt == NULL)
         pszSalt = "";

      DATA_BLOB DataSalt;
      primitive::memory memorySalt;
      memorySalt.from_string(pszSalt);
      DataSalt.pbData = memorySalt.get_data();
      DataSalt.cbData = (DWORD) memorySalt.get_size();


      //--------------------------------------------------------------------
      // Initialize the DataIn structure.

      DataIn.pbData = (BYTE *) storageDecrypt.get_data();
      DataIn.cbData = (DWORD) storageDecrypt.get_size();

//      wchar_t * lpwsz = NULL;

      //--------------------------------------------------------------------
      //  Begin protect phase. Note that the encryption key is created
      //  by the function and is not passed.

      if(CryptProtectData(
           &DataIn,
           NULL, // A description string
                                               // to be included with the
                                               // encrypted data.
           &DataSalt,                               // Optional entropy not used.
           NULL,                               // Reserved.
           NULL,                               // Pass NULL for the
                                               // prompt structure.
           0,
           &DataOut))
      {
         TRACE("crypt::encrypt The encryption phase worked. \n");
         storageEncrypt.allocate(DataOut.cbData);
         memcpy(storageEncrypt.get_data(), DataOut.pbData, DataOut.cbData);
         LocalFree(DataOut.pbData);
         return true;
      }
      else
      {
         TRACE("crypt::encrypt Encryption error! (1)");
          return false;
      }

   }
*/

} // namespace win


