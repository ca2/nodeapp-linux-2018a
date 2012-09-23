#pragma once


namespace lnx
{


   class CLASS_DECL_LNX brush :
      virtual public ::ca::brush
   {
   public:

   int                  m_iStyle;
   COLORREF             m_cr;
   int                  m_iStock;


//xxx      Gdiplus::Brush * m_pbrush;


      brush(::ca::application * papp);
      //virtual void construct(COLORREF crColor);                // CreateSolidBrush
      //virtual void construct(int nIndex, COLORREF crColor);    // CreateHatchBrush
      //virtual void construct(::ca::bitmap * pbitmap);                // CreatePatternBrush
      virtual ~brush();


      virtual void * get_os_data() const;


      //bool CreateSolidBrush(COLORREF crColor);
      //bool CreateHatchBrush(int nIndex, COLORREF crColor);
      //bool CreatePatternBrush(::ca::bitmap* pBitmap);
      //bool CreateDIBPatternBrush(HGLOBAL hPackedDIB, UINT nUsage);
      //bool CreateDIBPatternBrush(const void * lpPackedDIB, UINT nUsage);
      //bool CreateSysColorBrush(int nIndex);


      virtual void dump(dump_context & dumpcontext) const;


   };


} // namespace lnx

