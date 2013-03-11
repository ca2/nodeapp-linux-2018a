#pragma once


namespace lnx
{


   class CLASS_DECL_lnx region :
      virtual public ::ca::region
   {
   public:

      cairo_surface_t *       m_psurface;
      rect                    m_rectBoundingBoxInternal;


// xxx     Gdiplus::Region *       m_pregion;

      //static region * PASCAL from_handle(::ca::application * papp, HRGN hRgn);
      //operator HRGN() const;

      region(::ca::application * papp);
      virtual ~region();


      //virtual void * get_os_data() const;


      /*bool CreateRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool CreateRectRgnIndirect(LPCRECT lpRect);
      bool CreateEllipticRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      bool CreateEllipticRgnIndirect(LPCRECT lpRect);
      bool CreatePolygonRgn(LPPOINT lpPoints, int32_t nCount, int32_t nMode);
      bool CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts,
            int32_t nCount, int32_t nPolyFillMode);
      bool CreateRoundRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);
      bool CreateFromPath(::ca::graphics * pgraphics);
      bool CreateFromData(const XFORM* lpXForm, int32_t nCount,
         const RGNDATA* pRgnData);*/

   // Operations
      //void SetRectRgn(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
      //void SetRectRgn(LPCRECT lpRect);
      //int32_t CombineRgn(const ::ca::region* pRgn1, const ::ca::region* pRgn2, int32_t nCombineMode);
      //int32_t CopyRgn(const ::ca::region* pRgnSrc);
      //bool EqualRgn(const ::ca::region* pRgn) const;
      //int32_t OffsetRgn(int32_t x, int32_t y);
      //int32_t OffsetRgn(POINT point);
      //int32_t GetRgnBox(LPRECT lpRect) const;
      //bool PtInRegion(int32_t x, int32_t y) const;
      //bool PtInRegion(POINT point) const;
      //bool RectInRegion(LPCRECT lpRect) const;
// xxx      int32_t GetRegionData(LPRGNDATA lpRgnData, int32_t nCount) const;

      bool get(cairo_t * pdc);
      bool get_rect(cairo_t * pdc);
      bool get_oval(cairo_t * pdc);
      bool get_polygon(cairo_t * pdc);
      bool get_poly_polygon(cairo_t * pdc);
      bool get_combine(cairo_t * pdc);


      virtual void * get_os_data() const;



   };


} // namespace lnx


