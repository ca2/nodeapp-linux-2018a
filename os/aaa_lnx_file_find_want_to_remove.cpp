#include "framework.h"


namespace lnx
{


   file_find::file_find()
   {
      m_pentFound = NULL;
      m_pentNext = NULL;
      m_pdir = NULL;
      m_chDirSeparator = '\\';
   }

   file_find::~file_find()
   {
      close();
   }

   void file_find::close()
   {
      if (m_pentFound != NULL)
      {
         delete m_pentFound;
         m_pentFound = NULL;
      }

      if (m_pentNext != NULL)
      {
         delete m_pentNext;
         m_pentNext = NULL;
      }

      if (m_pdir != NULL)
      {
         CloseContext();
         m_pdir = NULL;
      }
   }

   void file_find::CloseContext()
   {
      ::closedir(m_pdir);
      return;
   }

   bool file_find::FindFile(const char * pszName, DWORD dwUnused)
   {

      close();
      m_bGotLast = FALSE;

      if (pszName == NULL)
         pszName = "*.*";

      string strName(pszName);

      if(!::dir::is(strName))
      {

         strName = System.dir().name(strName);

         if(!::dir::is(strName))
            return false;

      }

      m_pdir = opendir(strName);

      if(m_pdir == NULL)
         return false;




      wstring wstrRoot;

      wchar_t * pstrRoot = wstrRoot.alloc(_MAX_PATH);
      const wchar_t * pstr = ::lnx::shell::_fullpath(pstrRoot, wstring(strName), _MAX_PATH);

      // passed name isn't a valid path but was found by the API
      ASSERT(pstr != NULL);
      if (pstr == NULL)
      {
         m_strRoot.Empty();
         close();
         ::SetLastError(ERROR_INVALID_NAME);
         return FALSE;
      }
      else
      {
         // find the last forward or backward whack
         wchar_t * pstrBack  = wcsrchr(pstrRoot, '\\');
         wchar_t * pstrFront = wcsrchr(pstrRoot, '/');

         if (pstrFront != NULL || pstrBack != NULL)
         {
            if (pstrFront == NULL)
               pstrFront = pstrRoot;
            if (pstrBack == NULL)
               pstrBack = pstrRoot;

            // from the start to the last whack is the root

            if (pstrFront >= pstrBack)
               *pstrFront = '\0';
            else
               *pstrBack = '\0';
         }
         wstrRoot.release_buffer();
      }
      m_strRoot = gen::international::unicode_to_utf8(wstrRoot);
      return TRUE;
   }
/*
   bool file_find::MatchesMask(DWORD dwMask) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL)
         return (!!(((m_pentFound)->  dwMask));
      else
         return FALSE;
   }
*/
   bool file_find::GetLastAccessTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pentFound)->ftLastAccessTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastWriteTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT(pTimeStamp != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pentFound)->ftLastWriteTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetCreationTime(FILETIME* pTimeStamp) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL && pTimeStamp != NULL)
      {
         *pTimeStamp = ((LPWIN32_FIND_DATAW) m_pentFound)->ftCreationTime;
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastAccessTime(::datetime::time& refTime) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pentFound)->ftLastAccessTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetLastWriteTime(::datetime::time& refTime) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pentFound)->ftLastWriteTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::GetCreationTime(::datetime::time& refTime) const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL)
      {
         refTime = ::datetime::time(((LPWIN32_FIND_DATAW) m_pentFound)->ftCreationTime);
         return TRUE;
      }
      else
         return FALSE;
   }

   bool file_find::IsDots() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      // return TRUE if the file name is "." or ".." and
      // the file is a directory

      bool bResult = FALSE;
      if (m_pentFound != NULL && IsDirectory())
      {
         LPWIN32_FIND_DATAW pFindData = (LPWIN32_FIND_DATAW) m_pentFound;
         if (pFindData->cFileName[0] == '.')
         {
            if (pFindData->cFileName[1] == '\0' ||
               (pFindData->cFileName[1] == '.' &&
               pFindData->cFileName[2] == '\0'))
            {
               bResult = TRUE;
            }
         }
      }

      return bResult;
   }

   bool file_find::FindNextFile()
   {
      ASSERT(m_pdir != NULL);

      if (m_pdir == NULL)
         return FALSE;
      if (m_pentFound == NULL)
         m_pentFound = new WIN32_FIND_DATAW;

      ASSERT_VALID(this);

      WIN32_FIND_DATAW * pTemp = m_pentFound;
      m_pentFound = m_pentNext;
      m_pentNext = pTemp;

      return ::win::shell::FindNextFile(m_pdir, (LPWIN32_FIND_DATAW) m_pentNext);
   }

   string file_find::GetFileURL() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      string strResult(L"file://");
      strResult += GetFilePath();
      return strResult;
   }

   string file_find::GetRoot() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);
      return m_strRoot;
   }

   string file_find::GetFilePath() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      string strResult = GetRoot();
      if (strResult[strResult.get_length()-1] != '\\' &&
         strResult[strResult.get_length()-1] != '/')
         strResult += m_chDirSeparator;
      strResult += GetFileName();
      return strResult;
   }

   string file_find::GetFileTitle() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      string strFullName = GetFileName();
      string strResult;

      _splitpath(strFullName, NULL, NULL, strResult.GetBuffer(_MAX_PATH), NULL);
      strResult.ReleaseBuffer();
      return strResult;
   }

   string file_find::GetFileName() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      string ret;

      if (m_pentFound != NULL)
      {
         gen::international::unicode_to_utf8(ret, ((LPWIN32_FIND_DATAW) m_pentFound)->cFileName);
      }
      return ret;
   }

   int64_t file_find::get_length() const
   {
      ASSERT(m_pdir != NULL);
      ASSERT_VALID(this);

      if (m_pentFound != NULL)
         return ((LPWIN32_FIND_DATAW) m_pentFound)->nFileSizeLow +
         ((int64_t)(((LPWIN32_FIND_DATAW) m_pentFound)->nFileSizeHigh) << 32);
      else
         return 0;
   }


   void file_find::dump(dump_context & dumpcontext) const
   {
      ::radix::object::dump(dumpcontext);
      dumpcontext << "\nm_hContext = " << (UINT) m_pdir;
   }

   void file_find::assert_valid() const
   {
      // if you trip the ASSERT in the else side, you've called
      // a get() function without having done at least one
      // FindNext() call

      if (m_pdir == NULL)
         ASSERT(m_pentFound == NULL && m_pentNext == NULL);
      else
         ASSERT(m_pentFound != NULL && m_pentNext != NULL);

   }


} // namespace win




