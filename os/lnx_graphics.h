#pragma once


namespace lnx
{

   class brush;
   class pen;
   class font;

   class CLASS_DECL_lnx graphics :
      virtual public ::ca::graphics
   {
      // // DECLARE_DYNCREATE(::ca::graphics_sp)
   public:


      cairo_t *               m_pdc; // Cairo drawing context
      int                     m_iType;
      bool                    m_bForeColor;
      bool                    m_bBackColor;
      long unsigned int       m_uiForeColor;
      long unsigned int       m_uiBackColor;
      COLORREF                m_crTextColor;
      //xxx ::Gdiplus::GraphicsPath *     m_ppath;
      //xxx ::Gdiplus::GraphicsPath *     m_ppathPaint;
      //xxx HDC                           m_hdc;

      // advanced use and implementation
      bool                          m_bPrinting;



      graphics();
      graphics(::ca::application * papp);
      virtual ~graphics();


/* xxx      Gdiplus::Graphics & g()
      {
         if(m_pgraphics == NULL)
         {
            m_pgraphics = ::Gdiplus::Graphics::FromHDC(get_handle1());
         }
         return *m_pgraphics;
      }*/

      //::ca::window * GetWindow() const;

      //static ::ca::graphics * PASCAL from_handle(HDC hDC);
      //static void PASCAL DeleteTempMap();
      //bool Attach(HDC hdc);   // Attach/Detach affects only the Output DC
      //HDC Detach();

      //virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC
      //virtual void SetOutputDC(HDC hDC);  // Set the Output DC
      //virtual void ReleaseAttribDC();     // Release the Attribute DC
      //virtual void ReleaseOutputDC();     // Release the Output DC

      bool IsPrinting() const;            // TRUE if being used for printing

      ::ca::pen & GetCurrentPen() const;
      ::ca::brush & GetCurrentBrush() const;
      ::ca::palette & GetCurrentPalette() const;
      ::ca::font & GetCurrentFont() const;
      ::ca::bitmap & GetCurrentBitmap() const;


      //pen *       lnx_pen();
      //brush *     lnx_brush();
      //font *      lnx_font();

      // for bidi and mirrored localization
      DWORD GetLayout() const;
      DWORD SetLayout(DWORD dwLayout);


      virtual double get_dpix() const;

   // Constructors
      bool CreateDC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData);
      bool CreateIC(const char * lpszDriverName, const char * lpszDeviceName, const char * lpszOutput, const void * lpInitData);
      bool CreateCompatibleDC(::ca::graphics * pgraphics);

      bool DeleteDC();

   // Device-Context Functions
      virtual int SaveDC();
      virtual bool RestoreDC(int nSavedDC);
      int GetDeviceCaps(int nIndex) const;
      UINT SetBoundsRect(LPCRECT lpRectBounds, UINT flags);
      UINT GetBoundsRect(LPRECT lpRectBounds, UINT flags);
// xxx      bool ResetDC(const DEVMODE* lpDevMode);

   // Drawing-Tool Functions
      point GetBrushOrg() const;
      point SetBrushOrg(int x, int y);
      point SetBrushOrg(POINT point);
      int EnumObjects(int nObjectType,
            int (CALLBACK* lpfn)(LPVOID, LPARAM), LPARAM lpData);

   // Type-safe selection helpers
   public:
      virtual ::ca::graphics_object* SelectStockObject(int nIndex);
      ::ca::pen* SelectObject(::ca::pen* pPen);
      ::ca::brush* SelectObject(::ca::brush* pBrush);
      virtual ::ca::font* SelectObject(::ca::font* pFont);
      ::ca::bitmap* SelectObject(::ca::bitmap* pBitmap);
      int SelectObject(::ca::region* pRgn);       // special return for regions
      ::ca::graphics_object* SelectObject(::ca::graphics_object* pObject);
         // ::ca::graphics_object* provided so compiler doesn't use SelectObject(HGDIOBJ)

   // color and color Palette Functions
      COLORREF GetNearestColor(COLORREF crColor) const;
      ::ca::palette* SelectPalette(::ca::palette* pPalette, bool bForceBackground);
      UINT RealizePalette();
      void UpdateColors();

   // Drawing-Attribute Functions
      COLORREF GetBkColor() const;
      int GetBkMode() const;
      int GetPolyFillMode() const;
      int GetROP2() const;
      int GetStretchBltMode() const;
      COLORREF GetTextColor() const;

      virtual COLORREF SetBkColor(COLORREF crColor);
      int SetBkMode(int nBkMode);
      int SetPolyFillMode(int nPolyFillMode);
      int SetROP2(int nDrawMode);
      int SetStretchBltMode(int nStretchMode);
      virtual COLORREF SetTextColor(COLORREF crColor);

// xxx      bool GetColorAdjustment(LPCOLORADJUSTMENT lpColorAdjust) const;
// xxx      bool SetColorAdjustment(const COLORADJUSTMENT* lpColorAdjust);

   #if (_WIN32_WINNT >= 0x0500)

      COLORREF GetDCBrushColor() const;
      COLORREF SetDCBrushColor(COLORREF crColor);

      COLORREF GetDCPenColor() const;
      COLORREF SetDCPenColor(COLORREF crColor);

   #endif

      // Graphics mode
      int SetGraphicsMode(int iMode);
      int GetGraphicsMode() const;

      // World transform
      bool SetWorldTransform(const XFORM* pXform);
      bool ModifyWorldTransform(const XFORM* pXform,DWORD iMode);
      bool GetWorldTransform(XFORM* pXform) const;

      // Mapping Functions
      virtual int GetMapMode() const;
      virtual point GetViewportOrg() const;
      virtual int SetMapMode(int nMapMode);
      // Viewport Origin
      virtual point SetViewportOrg(int x, int y);
      virtual point SetViewportOrg(POINT point);
      virtual point OffsetViewportOrg(int nWidth, int nHeight);

      // Viewport Extent
      virtual size GetViewportExt() const;
      virtual size SetViewportExt(int cx, int cy);
      virtual size SetViewportExt(SIZE size);
      virtual size ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

      // Window Origin
      point GetWindowOrg() const;
      point SetWindowOrg(int x, int y);
      point SetWindowOrg(POINT point);
      point OffsetWindowOrg(int nWidth, int nHeight);

      // Window extent
      size GetWindowExt() const;
      virtual size SetWindowExt(int cx, int cy);
            size SetWindowExt(SIZE size);
      virtual size ScaleWindowExt(int xNum, int xDenom, int yNum, int yDenom);

   // Coordinate Functions
      void DPtoLP(LPPOINT lpPoints, int nCount = 1) const;
      void DPtoLP(LPRECT lpRect) const;
      void DPtoLP(LPSIZE lpSize) const;
      void LPtoDP(LPPOINT lpPoints, int nCount = 1) const;
      void LPtoDP(LPRECT lpRect) const;
      void LPtoDP(LPSIZE lpSize) const;

   // Special Coordinate Functions (useful for dealing with metafiles and OLE)
      void DPtoHIMETRIC(LPSIZE lpSize) const;
      void LPtoHIMETRIC(LPSIZE lpSize) const;
      void HIMETRICtoDP(LPSIZE lpSize) const;
      void HIMETRICtoLP(LPSIZE lpSize) const;

   // Region Functions
      bool FillRgn(::ca::region* pRgn, ::ca::brush* pBrush);
      bool FrameRgn(::ca::region* pRgn, ::ca::brush* pBrush, int nWidth, int nHeight);
      bool InvertRgn(::ca::region* pRgn);
      bool PaintRgn(::ca::region* pRgn);

   // Clipping Functions
      virtual int GetClipBox(LPRECT lpRect) const;
      virtual bool PtVisible(int x, int y) const;
            bool PtVisible(POINT point) const;
      virtual bool RectVisible(LPCRECT lpRect) const;
            int SelectClipRgn(::ca::region* pRgn);
            int ExcludeClipRect(int x1, int y1, int x2, int y2);
            int ExcludeClipRect(LPCRECT lpRect);
            int ExcludeUpdateRgn(::ca::window * pWnd);
            int IntersectClipRect(int x1, int y1, int x2, int y2);
            int IntersectClipRect(LPCRECT lpRect);
            int OffsetClipRgn(int x, int y);
            int OffsetClipRgn(SIZE size);
      int SelectClipRgn(::ca::region* pRgn, int nMode);

   // Line-Output Functions
      point GetCurrentPosition() const;
//      point MoveTo(int x, int y);
  //    point MoveTo(POINT point);
      bool LineTo(double x, double y);
    //  bool LineTo(POINT point);
      bool Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Arc(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
      bool Polyline(const POINT* lpPoints, int nCount);

      bool AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle);
      bool ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool ArcTo(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
      int GetArcDirection() const;
      int SetArcDirection(int nArcDirection);

      bool PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount);
      bool PolylineTo(const POINT* lpPoints, int nCount);
      bool PolyPolyline(const POINT* lpPoints,
         const DWORD* lpPolyPoints, int nCount);

      bool PolyBezier(const POINT* lpPoints, int nCount);
      bool PolyBezierTo(const POINT* lpPoints, int nCount);

   // Simple Drawing Functions
      void FillRect(LPCRECT lpRect, ::ca::brush* pBrush);
      void FrameRect(LPCRECT lpRect, ::ca::brush* pBrush);
      void InvertRect(LPCRECT lpRect);
      bool DrawIcon(int x, int y, ::visual::icon * picon);
      bool DrawIcon(POINT point, ::visual::icon * picon);
      bool DrawIcon(int x, int y, ::visual::icon * picon, int cx, int cy, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags);
      bool DrawState(point pt, size size, HBITMAP hBitmap, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, ::ca::bitmap* pBitmap, UINT nFlags,
         ::ca::brush* pBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, HICON hIcon, UINT nFlags,
         ::ca::brush* pBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, HBRUSH hBrush = NULL);
      bool DrawState(point pt, size size, const char * lpszText, UINT nFlags,
         bool bPrefixText = TRUE, int nTextLen = 0, ::ca::brush* pBrush = NULL);
//xxx      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
//xxx         LPARAM lData, UINT nFlags, HBRUSH hBrush = NULL);
//xxx      bool DrawState(point pt, size size, DRAWSTATEPROC lpDrawProc,
//xxx         LPARAM lData, UINT nFlags, ::ca::brush* pBrush = NULL);

   // Ellipse and Polygon Functions
      bool Chord(int x1, int y1, int x2, int y2, int x3, int y3,
         int x4, int y4);
      bool Chord(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
      void DrawFocusRect(LPCRECT lpRect);
      bool DrawEllipse(int x1, int y1, int x2, int y2);
      bool DrawEllipse(LPCRECT lpRect);
      bool FillEllipse(int x1, int y1, int x2, int y2);
      bool FillEllipse(LPCRECT lpRect);
      bool Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
      bool Pie(LPCRECT lpRect, POINT ptStart, POINT ptEnd);
      virtual bool fill_polygon(const POINTD * lpPoints, int nCount);
      virtual bool fill_polygon(const POINT* lpPoints, int nCount);
      bool Polygon(const POINT* lpPoints, int nCount);
      bool PolyPolygon(const POINT* lpPoints, const INT* lpPolyCounts, int nCount);
      bool Rectangle(int x1, int y1, int x2, int y2);
      bool Rectangle(LPCRECT lpRect);
      virtual bool DrawRectangle(int x1, int y1, int x2, int y2);
      virtual bool DrawRectangle(LPCRECT lpRect);
      virtual bool FillRectangle(int x1, int y1, int x2, int y2);
      virtual bool FillRectangle(LPCRECT lpRect);
      bool RoundRect(int x1, int y1, int x2, int y2, int x3, int y3);
      bool RoundRect(LPCRECT lpRect, POINT point);

   // Bitmap Functions
      bool PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop);
      bool BitBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc,
         int xSrc, int ySrc, DWORD dwRop);
      bool StretchBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc,
         int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop);
      COLORREF GetPixel(int x, int y) const;
      COLORREF GetPixel(POINT point) const;
      COLORREF SetPixel(int x, int y, COLORREF crColor);
      COLORREF SetPixel(POINT point, COLORREF crColor);
      bool FloodFill(int x, int y, COLORREF crColor);
      bool ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType);
      bool MaskBlt(int x, int y, int nWidth, int nHeight, ::ca::graphics * pgraphicsSrc,
         int xSrc, int ySrc, ::ca::bitmap& maskBitmap, int xMask, int yMask,
         DWORD dwRop);
      bool PlgBlt(LPPOINT lpPoint, ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc,
         int nWidth, int nHeight, ::ca::bitmap& maskBitmap, int xMask, int yMask);
      bool SetPixelV(int x, int y, COLORREF crColor);
      bool SetPixelV(POINT point, COLORREF crColor);
      bool GradientFill(TRIVERTEX* pVertices, ULONG nVertices,
        void * pMesh, ULONG nMeshElements, DWORD dwMode);
      bool TransparentBlt(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
        UINT clrTransparent);

      virtual bool alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight, ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, double dOpacity);

      /*bool alpha_blend(int xDest, int yDest, int nDestWidth, int nDestHeight,
        ::ca::graphics * pgraphicsSrc, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight,
        BLENDFUNCTION blend);*/

   // Text Functions
      virtual bool TextOut(int x, int y, const char * lpszString, int nCount);
      virtual bool TextOut(int x, int y, const string & str);
      virtual bool TextOut(double x, double y, const char * lpszString, int nCount);
      virtual bool TextOut(double x, double y, const string & str);
      virtual bool ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
               const char * lpszString, UINT nCount, LPINT lpDxWidths);
            bool ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
               const string & str, LPINT lpDxWidths);
      virtual size TabbedTextOut(int x, int y, const char * lpszString, int nCount,
               int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
            size TabbedTextOut(int x, int y, const string & str,
               int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);

      virtual int draw_text(const char * lpszString, int nCount, LPRECT lpRect, UINT nFormat);
      virtual int draw_text(const string & str, LPRECT lpRect, UINT nFormat);

      virtual int draw_text_ex(LPTSTR lpszString, int nCount, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams);
      virtual int draw_text_ex(const string & str, LPRECT lpRect, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams);

      size GetTextExtent(const char * lpszString, strsize nCount, int iIndex) const;
      size GetTextExtent(const char * lpszString, strsize nCount) const;
      size GetTextExtent(const string & str) const;
      bool GetTextExtent(sized & size, const char * lpszString, strsize nCount, int iIndex) const;
      bool GetTextExtent(sized & size, const char * lpszString, strsize nCount) const;
      bool GetTextExtent(sized & size, const string & str) const;
      size GetOutputTextExtent(const char * lpszString, strsize nCount) const;
      size GetOutputTextExtent(const string & str) const;
      size GetTabbedTextExtent(const char * lpszString, strsize nCount, int nTabPositions, LPINT lpnTabStopPositions) const;
      size GetTabbedTextExtent(const string & str, int nTabPositions, LPINT lpnTabStopPositions) const;
      size GetOutputTabbedTextExtent(const char * lpszString, strsize nCount, int nTabPositions, LPINT lpnTabStopPositions) const;
      size GetOutputTabbedTextExtent(const string & str, int nTabPositions, LPINT lpnTabStopPositions) const;
      virtual bool GrayString(::ca::brush* pBrush,
         bool (CALLBACK* lpfnOutput)(HDC, LPARAM, int), LPARAM lpData,
            int nCount, int x, int y, int nWidth, int nHeight);
      UINT GetTextAlign() const;
      UINT SetTextAlign(UINT nFlags);
      int GetTextFace(int nCount, LPTSTR lpszFacename) const;
      int GetTextFace(string & rString) const;
      bool get_text_metrics(LPTEXTMETRIC lpMetrics) const;
      bool get_output_text_metrics(LPTEXTMETRIC lpMetrics) const;
      int SetTextJustification(int nBreakExtra, int nBreakCount);
      int GetTextCharacterExtra() const;
      int SetTextCharacterExtra(int nCharExtra);

//xxx      DWORD GetCharacterPlacement(const char * lpString, int nCount, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const;
//xxx      DWORD GetCharacterPlacement(string & str, int nMaxExtent, LPGCP_RESULTS lpResults, DWORD dwFlags) const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetTextExtentExPointI(LPWORD pgiIn, int cgi, int nMaxExtent, LPINT lpnFit, LPINT alpDx, __out_opt LPSIZE lpSize) const;
      bool GetTextExtentPointI(LPWORD pgiIn, int cgi, __out_opt LPSIZE lpSize) const;

   #endif



   // Advanced Drawing
      bool DrawEdge(LPRECT lpRect, UINT nEdge, UINT nFlags);
      bool DrawFrameControl(LPRECT lpRect, UINT nType, UINT nState);

   // Scrolling Functions
      bool ScrollDC(int dx, int dy, LPCRECT lpRectScroll, LPCRECT lpRectClip,
         ::ca::region* pRgnUpdate, LPRECT lpRectUpdate);

   // font Functions
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      bool GetOutputCharWidth(UINT nFirstChar, UINT nLastChar, LPINT lpBuffer) const;
      DWORD SetMapperFlags(DWORD dwFlag);
      size GetAspectRatioFilter() const;

//xxx      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar, LPABC lpabc) const;
      DWORD GetFontData(DWORD dwTable, DWORD dwOffset, LPVOID lpData, DWORD cbData) const;
//xxx      int GetKerningPairs(int nPairs, LPKERNINGPAIR lpkrnpair) const;
//xxx      UINT GetOutlineTextMetrics(UINT cbData, LPOUTLINETEXTMETRIC lpotm) const;
//xxx      DWORD GetGlyphOutline(UINT nChar, UINT nFormat, LPGLYPHMETRICS lpgm,
     //xxx    DWORD cbBuffer, LPVOID lpBuffer, const MAT2* lpmat2) const;

//xxx      bool GetCharABCWidths(UINT nFirstChar, UINT nLastChar,
//xxx         LPABCFLOAT lpABCF) const;
      bool GetCharWidth(UINT nFirstChar, UINT nLastChar,
         float* lpFloatBuffer) const;

      DWORD GetFontLanguageInfo() const;

   #if (_WIN32_WINNT >= 0x0500)

      bool GetCharABCWidthsI(UINT giFirst, UINT cgi, LPWORD pgi, LPABC lpabc) const;
      bool GetCharWidthI(UINT giFirst, UINT cgi, LPWORD pgi, LPINT lpBuffer) const;

   #endif

   // Printer/Device Escape Functions
      virtual int Escape(int nEscape, int nCount, const char * lpszInData, LPVOID lpOutData);
      int Escape(int nEscape, int nInputSize, const char * lpszInputData, int nOutputSize, char * lpszOutputData);
      int DrawEscape(int nEscape, int nInputSize, const char * lpszInputData);

      // Escape helpers
      int StartDoc(const char * lpszDocName);  // old Win3.0 version
//xxx      int StartDoc(LPDOCINFO lpDocInfo);
      int StartPage();
      int EndPage();
      int SetAbortProc(bool (CALLBACK* lpfn)(HDC, int));
      int AbortDoc();
      int EndDoc();

   // MetaFile Functions
//xxx      bool PlayMetaFile(HMETAFILE hMF);
      bool PlayMetaFile(HENHMETAFILE hEnhMetaFile, LPCRECT lpBounds);
      bool AddMetaFileComment(UINT nDataSize, const BYTE* pCommentData);
         // can be used for enhanced metafiles only

   // Path Functions
      bool AbortPath();
      bool BeginPath();
      bool CloseFigure();
      bool EndPath();
      bool FillPath();
      bool FlattenPath();
      bool StrokeAndFillPath();
      bool StrokePath();
      bool WidenPath();

      bool draw_path(::ca::graphics_path * ppath);
      bool fill_path(::ca::graphics_path * ppath);


      float GetMiterLimit() const;
      bool SetMiterLimit(float fMiterLimit);
      int GetPath(LPPOINT lpPoints, LPBYTE lpTypes, int nCount) const;
      bool SelectClipPath(int nMode);

   // Misc Helper Functions
      static ::ca::brush* PASCAL GetHalftoneBrush(::ca::application * papp);
      void DrawDragRect(LPCRECT lpRect, SIZE size,
         LPCRECT lpRectLast, SIZE sizeLast,
         ::ca::brush* pBrush = NULL, ::ca::brush* pBrushLast = NULL);
      void FillSolidRect(const __rect64 * lpRect, COLORREF clr);
      void FillSolidRect(LPCRECT lpRect, COLORREF clr);
      void FillSolidRect(int x, int y, int cx, int cy, COLORREF clr);
      void Draw3dRect(LPCRECT lpRect, COLORREF clrTopLeft, COLORREF clrBottomRight);
      void Draw3dRect(int x, int y, int cx, int cy,
         COLORREF clrTopLeft, COLORREF clrBottomRight);




   // Implementation
   public:
      virtual void assert_valid() const;
      virtual void dump(dump_context & dumpcontext) const;

//      HGDIOBJ SelectObject(HGDIOBJ);      // do not use for regions

      virtual void set_alpha_mode(::ca::e_alpha_mode ealphamode);

      virtual void set_text_rendering(::ca::e_text_rendering etextrendering);

      virtual void * get_os_data() const;
//      virtual HDC get_handle() const;
  //    virtual HDC get_handle1() const;
    //  virtual HDC get_handle2() const;

      virtual bool attach(void * pdata);


//xxx      virtual Gdiplus::FillMode gdiplus_get_fill_mode();

      bool blur(bool bExpand, double dRadius, LPCRECT lpcrect);

   //protected:
      // used for implementation of non-virtual SelectObject calls
      //static ::ca::graphics_object* PASCAL SelectGdiObject(::ca::application * papp, HDC hDC, HGDIOBJ h);


      // platform-specific or platform-internals
      bool set(const ::ca::brush * pbrush);
      bool set(const ::ca::pen * ppen);
      bool set(const ::ca::font * pfont);
      bool set(const ::ca::graphics_path * ppath);
      bool set(const ::lnx::graphics_path::element & e);
      bool set(const ::lnx::graphics_path::arc & arc);
      bool set(const ::lnx::graphics_path::move & move);
      bool set(const ::lnx::graphics_path::line & line);
      bool fill_and_draw(::ca::brush * pbrush, ::ca::pen * ppen);
      bool fill(::ca::brush * pbrush);
      bool draw(::ca::pen * ppen);
      bool fill_and_draw();
      bool fill();
      bool draw();

   };


} // namespace win





