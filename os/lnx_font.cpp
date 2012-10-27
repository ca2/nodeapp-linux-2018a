#include "framework.h"


namespace lnx
{


   font::font(::ca::application * papp) :
   ca(papp)
   {

      m_bUpdated  = false;

   }


   font::~font()
   {

   }

//   font::operator HFONT() const
  // {
    //  return (HFONT)(this == NULL ? NULL : get_handle());
   //}
   //font* PASCAL font::from_handle(::ca::application * papp, HFONT hFont)
   //{
     // return dynamic_cast < font * > (::win::graphics_object::from_handle(papp, hFont));
   //}
   bool font::CreateFontIndirect(const LOGFONT* lpLogFont)
   {

      m_strFontFamilyName = lpLogFont->lfFaceName;
      if(lpLogFont->lfHeight < 0)
         m_dFontSize = (int) abs(lpLogFont->lfHeight);
      else
         m_dFontSize = lpLogFont->lfHeight / 10.0;
      m_eunitFontSize = ::ca::unit_point;
      m_bBold = lpLogFont->lfWeight >= 500;
      m_bItalic = lpLogFont->lfItalic != FALSE;
      m_bStrikeout = lpLogFont->lfStrikeOut != FALSE;

      return TRUE;
      //return Attach(::CreateFontIndirect(lpLogFont));
   }
   bool font::CreateFont(int nHeight, int nWidth, int nEscapement,
      int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
      BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
      BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
      const char * lpszFacename)
   {
      return FALSE;
      //return Attach(::CreateFont(nHeight, nWidth, nEscapement,
   //nOrientation, nWeight, bItalic, bUnderline, cStrikeOut,
   //nCharSet, nOutPrecision, nClipPrecision, nQuality,
   //nPitchAndFamily, lpszFacename));
   }

   int font::GetLogFont(LOGFONT* pLogFont)
   {

      return 0;
   //   return ::GetObject(get_handle(), sizeof(LOGFONT), pLogFont);

   }


      /////////////////////////////////////////////////////////////////////////////

   void font::construct(const ::ca::font & fontParam)
      {
         class font & font = const_cast < ::win::font & > (dynamic_cast < const ::win::font & > (fontParam));
         if(font.m_pfont == NULL)
         {
            if(m_pfont != NULL)
            {
               delete m_pfont;
               m_pfont = NULL;
            }
         }
         else
         {
            m_pfont = font.m_pfont->Clone();
         }
//         if(get_handle() != NULL)
  //          delete_object();
    //     if(font.get_handle() != NULL)
      /*   {
            LOGFONT lf;
            memset(&lf, 0, sizeof(lf));
            font.GetLogFont(&lf);
            CreateFontIndirect(&lf);
         }*/
      }


      void font::dump(dump_context & dumpcontext) const
      {
         ::ca::graphics_object::dump(dumpcontext);

/*         if (get_handle() == NULL)
            return;

         if (!afxData.bWin95 && ::GetObjectType(get_handle()) != OBJ_FONT)
         {
            // not a valid GDI object
            dumpcontext << "has ILLEGAL HFONT!";
            return;
         }

         LOGFONT lf;
         VERIFY(GetObject(sizeof(lf), &lf));
         dumpcontext << "lf.lfHeight = " << lf.lfHeight;
         dumpcontext << "\nlf.lfWidth = " << lf.lfWidth;
         dumpcontext << "\nlf.lfEscapement = " << lf.lfEscapement;
         dumpcontext << "\nlf.lfOrientation = " << lf.lfOrientation;
         dumpcontext << "\nlf.lfWeight = " << lf.lfWeight;
         dumpcontext << "\nlf.lfItalic = " << (int)lf.lfItalic;
         dumpcontext << "\nlf.lfUnderline = " << (int)lf.lfUnderline;
         dumpcontext << "\nlf.lfStrikeOut = " << (int)lf.lfStrikeOut;
         dumpcontext << "\nlf.lfCharSet = " << (int)lf.lfCharSet;
         dumpcontext << "\nlf.lfOutPrecision = " << (int)lf.lfOutPrecision;
         dumpcontext << "\nlf.lfClipPrecision = " << (int)lf.lfClipPrecision;
         dumpcontext << "\nlf.lfQuality = " << (int)lf.lfQuality;
         dumpcontext << "\nlf.lfPitchAndFamily = " << (int)lf.lfPitchAndFamily;
         dumpcontext << "\nlf.lfFaceName = " << (const char *)lf.lfFaceName;

         dumpcontext << "\n";*/
      }


   /////////////////////////////////////////////////////////////////////////////
   // out-of-line ::ca::brush, font, etc. helpers

   // nPointSize is actually scaled 10x
   bool font::CreatePointFont(int nPointSize, const char * lpszFaceName, ::ca::graphics * pgraphics)
   {
      ASSERT(__is_valid_string(lpszFaceName));

      LOGFONT logFont;
      memset(&logFont, 0, sizeof(LOGFONT));
      logFont.lfCharSet = DEFAULT_CHARSET;
      logFont.lfHeight = nPointSize;
      ::gen::strncpy_s(logFont.lfFaceName, _countof(logFont.lfFaceName), lpszFaceName, _TRUNCATE);

      return CreatePointFontIndirect(&logFont, pgraphics);
   }

   // pLogFont->nHeight is interpreted as PointSize * 10
   bool font::CreatePointFontIndirect(const LOGFONT* lpLogFont, ::ca::graphics * pgraphics)
   {
      return ::ca::font::CreatePointFontIndirect(lpLogFont, pgraphics);
     /* ASSERT(__is_valid_address(lpLogFont, sizeof(LOGFONT), FALSE));
      HDC hDC;
      if (pgraphics != NULL)
      {
         ASSERT_VALID(pgraphics);
         ASSERT((dynamic_cast<::win::graphics * >(pgraphics))->get_handle2() != NULL);
         hDC = (dynamic_cast<::win::graphics * >(pgraphics))->get_handle2();
      }
      else
         hDC = ::GetDC(NULL);

      // convert nPointSize to logical units based on pgraphics
      LOGFONT logFont = *lpLogFont;
      POINT pt;
      // 72 points/inch, 10 decipoints/point
      pt.y = ::MulDiv(::GetDeviceCaps(hDC, LOGPIXELSY), logFont.lfHeight, 720);
      pt.x = 0;
      ::DPtoLP(hDC, &pt, 1);
      POINT ptOrg = { 0, 0 };
      ::DPtoLP(hDC, &ptOrg, 1);
      logFont.lfHeight = -abs(pt.y - ptOrg.y);

      if (pgraphics == NULL)
         ReleaseDC(NULL, hDC);

      return CreateFontIndirect(&logFont);*/
   }


   void * font::get_os_data() const
   {

      if(m_pfont == NULL || !m_bUpdated)
      {
         if(m_pfont != NULL)
         {
            try
            {
               delete ((font *) this)->m_pfont;
            }
            catch(...)
            {
            }
            ((font *) this)->m_pfont = NULL;
         }


         int iStyle = 0;

         if(m_bBold)
         {
            iStyle |= (int) Gdiplus::FontStyleBold;
         }

         if(m_bItalic)
         {
            iStyle |= (int) Gdiplus::FontStyleItalic;
         }

         if(m_bUnderline)
         {
            iStyle |= (int) Gdiplus::FontStyleUnderline;
         }

         if(m_bStrikeout)
         {
            iStyle |= (int) Gdiplus::FontStyleStrikeout;
         }

         Gdiplus::Unit unit;

         switch(m_eunitFontSize)
         {
         case ::ca::unit_pixel:
            unit = Gdiplus::UnitPixel;
            break;
         case ::ca::unit_point:
            unit = Gdiplus::UnitPoint;
            break;
         default:
            unit = Gdiplus::UnitPoint;
            break;
         };

         //retry_single_lock slGdiplus(&System.s_mutexGdiplus, millis(1), millis(1));

         ((font *) this)->m_pfont = new Gdiplus::Font(
               gen::international::utf8_to_unicode(m_strFontFamilyName),
               (Gdiplus::REAL) m_dFontSize,
               iStyle,
               unit);

      }


      if(m_pfont != NULL)
      {
         ((font *) this)->m_bUpdated = true;
      }

      return (void *) (Gdiplus::Font *) m_pfont;

   }

} // namespace lnx




