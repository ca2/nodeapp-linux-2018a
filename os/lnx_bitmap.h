#pragma once


namespace lnx
{


   class CLASS_DECL_lnx bitmap :
      virtual public ::ca::bitmap
   {
   public:


      cairo_surface_t *       m_psurface;
      simple_memory           m_mem;
      ::size                  m_size;


      bitmap(::ca::application * papp);
      virtual ~bitmap();


      void * get_os_data() const;


      bool LoadBitmap(const char * lpszResourceName);
      bool LoadBitmap(UINT nIDResource);
      bool LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC_
   #ifndef ___NO_AFXCMN_SUPPORT
// xxx      bool LoadMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
   #endif
      bool CreateBitmap(::ca::graphics * pdc, int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void * lpBits);
      bool CreateBitmapIndirect(::ca::graphics * pdc, LPBITMAP lpBitmap);
      bool CreateCompatibleBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      bool CreateDiscardableBitmap(::ca::graphics * pgraphics, int nWidth, int nHeight);
      bool CreateDIBSection(::ca::graphics * pdc, const BITMAPINFO * lpbmi, UINT usage, void **ppvBits, HANDLE hSection, DWORD offset);
      bool CreateDIBitmap(::ca::graphics * pdc, const BITMAPINFOHEADER *pbmih, DWORD flInit, const void *pjBits, const BITMAPINFO *pbmi, UINT iUsage);


      int GetBitmap(BITMAP* pBitMap);


      DWORD SetBitmapBits(DWORD dwCount, const void * lpBits);
      DWORD GetBitmapBits(DWORD dwCount, LPVOID lpBits) const;
      class size SetBitmapDimension(int nWidth, int nHeight);
      class size GetBitmapDimension() const;

      virtual void dump(dump_context & dumpcontext) const;


      virtual bool Attach(void * posdata);


      void destroy();


   };


} // namespace lnx


