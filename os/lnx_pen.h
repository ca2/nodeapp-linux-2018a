#pragma once



namespace lnx
{


   class CLASS_DECL_lnx pen :
      virtual public ::lnx::graphics_object,
      virtual public ::ca::pen
   {
   public:



      //int                  m_iStock;
      //int                  m_iStyle;
      //int                  m_iWidth;
      //COLORREF             m_cr;


      pen(::ca::application * papp);
      virtual ~pen();
      /*virtual void construct(int nPenStyle, double nWidth, COLORREF crColor);
      virtual void construct(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
      bool CreatePen(int nPenStyle, double nWidth, COLORREF crColor);
      bool CreatePen(int nPenStyle, double nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);*/


//      virtual void * get_os_data() const;


      virtual void dump(dump_context & dumpcontext) const;

   };


} // namespace lnx





