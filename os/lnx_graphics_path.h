#pragma once


namespace lnx
{


   class CLASS_DECL_c graphics_path :
      virtual public ::ca::graphics_path
   {
   public:


      class CLASS_DECL_c arc
      {
      public:


         double   m_xCenter;
         double   m_yCenter;
         double   m_dRadiusX;
         double   m_dRadiusY;
         double   m_dAngle1;
         double   m_dAngle2;


      };

      class CLASS_DECL_c move
      {
      public:

         double   m_x;
         double   m_y;

      };


      class CLASS_DECL_c line
      {
      public:

         double   m_x;
         double   m_y;

      };


      class CLASS_DECL_c element
      {
      public:


            enum e_type
            {
               type_begin,
               type_arc,
               type_line,
               type_move,
               type_close,
            };


            union
            {


               arc      m_arc;
               move     m_move;
               line     m_line;

            };

            e_type m_etype;


      };

      simple_array < element >      m_elementa;
      bool                          m_bFill;
      ::ca::e_fill_mode             m_efillmode;
      bool                          m_bHasPoint;
      point                         m_pt;


      graphics_path(::ca::application * papp);
      virtual ~graphics_path();


      virtual bool has_current_point();
      virtual point last_point();

      virtual bool begin_figure(bool bFill, ::ca::e_fill_mode efillmode);
      virtual bool end_figure(bool bClose);

      virtual bool add_arc(const RECT & rect, int iStart, int iAngle);
      virtual bool add_line(int x, int y);

      virtual bool add_move(int x, int y);



   };









} // namespace lnx



