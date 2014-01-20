#pragma once


namespace lnx
{


   class CLASS_DECL_LINUX crypto :
      virtual public ::crypto::crypto
   {
   public:


      crypto(sp(base_application) papp);
      virtual ~crypto();


      virtual bool encrypt(primitive::memory & storageEncrypt, const primitive::memory & storageDecrypt, const char * pszSalt);
      virtual bool decrypt(primitive::memory & storageDecrypt, const primitive::memory & storageEncrypt, const char * pszSalt);


      virtual string get_crypt_key_file_path();



   };


} // namespace lnx



