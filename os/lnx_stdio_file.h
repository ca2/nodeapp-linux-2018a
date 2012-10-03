#pragma once


/////////////////////////////////////////////////////////////////////////////
// STDIO file implementation


namespace lnx
{


   class stdio_file :
      virtual public ::lnx::file,
      virtual public ex1::text_file
   {
   public:


      FILE* m_pStream;    // stdio FILE
      // m_hFile from base class is _fileno(m_pStream)



      stdio_file(::ca::application * papp);
      virtual ~stdio_file();

      virtual void write_string(const char * lpsz);
      virtual char * read_string(char * lpsz, UINT nMax);
      virtual UINT read_string(string & rString);


      void dump(dump_context & dumpcontext) const;
      virtual file_position get_position() const;
      virtual bool open(const char * lpszFileName, UINT nOpenFlags);
      virtual ::primitive::memory_size read(void * lpBuf, ::primitive::memory_size nCount);
      virtual void write(const void * lpBuf, ::primitive::memory_size nCount);
      virtual file_position seek(file_offset lOff, ::ex1::e_seek nFrom);
      virtual void Abort();
      virtual void Flush();
      virtual void close();
      virtual file_size get_length() const;

      // Unsupported APIs
      virtual ex1::file * Duplicate() const;
      virtual void LockRange(file_position dwPos, file_size dwCount);
      virtual void UnlockRange(file_position dwPos, file_size dwCount);


   };



} // namespace lnx

