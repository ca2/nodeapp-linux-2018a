#pragma once


namespace lnx
{


   class CLASS_DECL_LNX font :
      virtual public ::ca::font
   {
   public:


      //Gdiplus::Font *   m_pfont;
   vsstring                m_strFamily;
   int                     m_iWeight;
   int                     m_iSize;
   XFontSet                m_fontset;
   bool                    m_bUpdated;
   Display *               m_pdisplay;
   int                     m_iAscent;
   int                     m_iDescent;



      font(::ca::application * papp);
      virtual ~font();


      virtual void * get_os_data() const;


      virtual void construct(const ::ca::font & fontParam);

      virtual bool CreateFontIndirect(const LOGFONT* lpLogFont);
      virtual bool CreateFont(int nHeight, int nWidth, int nEscapement,
            int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
            BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
            BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
            const char * lpszFacename);
      virtual bool CreatePointFont(int nPointSize, const char * lpszFaceName, ::ca::graphics * pgraphics = NULL);
      virtual bool CreatePointFontIndirect(const LOGFONT* lpLogFont, ::ca::graphics * pgraphics = NULL);

      virtual int GetLogFont(LOGFONT* pLogFont);

         virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace win



