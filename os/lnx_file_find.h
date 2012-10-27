#pragma once


#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


namespace lnx
{


   class CLASS_DECL_lnx file_find : public ::radix::object
   {
   // ementation
   protected:


      struct stat    m_st;
      dirent *       m_pentFound;
      dirent *       m_pentNext;
      DIR *          m_pdir;
      bool           m_bGotLast;
      string         m_strRoot;
      string         m_strCriteria;
      char           m_chDirSeparator;     // not '\\' for Internet classes


   public:


      file_find();
      virtual ~file_find();


      // Attributes
      int64_t get_length() const;
      virtual string GetFileName() const;
      virtual string GetFilePath() const;
      virtual string GetFileTitle() const;
      virtual string GetFileURL() const;
      virtual string GetRoot() const;

      virtual bool GetLastWriteTime(FILETIME* pTimeStamp) const;
      virtual bool GetLastAccessTime(FILETIME* pTimeStamp) const;
      virtual bool GetCreationTime(FILETIME* pTimeStamp) const;
      virtual bool GetLastWriteTime(::datetime::time& refTime) const;
      virtual bool GetLastAccessTime(::datetime::time& refTime) const;
      virtual bool GetCreationTime(::datetime::time& refTime) const;

//      virtual bool MatchesMask(DWORD dwMask) const;

      virtual bool IsDots() const;
      // these aren't virtual because they all use MatchesMask(), which is
      bool IsReadOnly() const;
      bool IsDirectory() const;
      bool IsCompressed() const;
      bool IsSystem() const;
      bool IsHidden() const;
      bool IsTemporary() const;
      bool IsNormal() const;
      bool IsArchived() const;

      // Operations
      void close();
      virtual bool FindFile(const char * pstrName = NULL, DWORD dwUnused = 0);
      virtual bool FindNextFile();

   protected:
      virtual void CloseContext();


   public:

      void dump(dump_context & dumpcontext) const;
      void assert_valid() const;


   };

   inline bool file_find::IsReadOnly() const
   {

      return MatchesMask(FILE_ATTRIBUTE_READONLY);

   }

   inline bool file_find::IsDirectory() const
   {

      return MatchesMask(FILE_ATTRIBUTE_DIRECTORY);

   }

   inline bool file_find::IsCompressed() const
   {

      return MatchesMask(FILE_ATTRIBUTE_COMPRESSED);

   }

   inline bool file_find::IsSystem() const
   {

      return MatchesMask(FILE_ATTRIBUTE_SYSTEM);

   }

   inline bool file_find::IsHidden() const
   {

      return MatchesMask(FILE_ATTRIBUTE_HIDDEN);

   }

   inline bool file_find::IsTemporary() const
   {

      return MatchesMask(FILE_ATTRIBUTE_TEMPORARY);

   }

   inline bool file_find::IsNormal() const
   {

      return MatchesMask(FILE_ATTRIBUTE_NORMAL);

   }

   inline bool file_find::IsArchived() const
   {

      return MatchesMask(FILE_ATTRIBUTE_ARCHIVE);

   }



} // namespace win




