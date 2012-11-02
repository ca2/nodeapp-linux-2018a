#include "framework.h"


namespace lnx
{

   graphics_path::graphics_path(::ca::application * papp) :
      ca(papp)
   {

      m_bFill        = false;
      m_efillmode    = ::ca::fill_mode_winding;

   }

   graphics_path::~graphics_path()
   {

   }


   bool has_current_point();
   point last_point()
   {

   }



   bool graphics_path::add_arc(const RECT & rect, int iStart, int iAngle)
   {

      if(width(rect) <= 0 || height(rect) <= 0)
         return true;

      element e;

      e.m_etype               = element::type_arc;
      e.m_arc.m_xCenter       = ((double) rect.right + (double) rect.left) / 2.0;
      e.m_arc.m_yCenter       = ((double) rect.bottom + (double) rect.top) / 2.0;
      e.m_arc.m_dRadiusX      = (double) rect.right - e.m_arc.m_xCenter;
      e.m_arc.m_dRadiusY      = (double) rect.bottom - e.m_arc.m_yCenter;
      e.m_arc.m_dAngle1        = iStart * 3.1415 / 180.0;
      e.m_arc.m_dAngle2        = e.m_arc.m_dAngle1 + iAngle * 3.1415 / 180.0;

      m_elementa.add(e);

      return true;

   }

   bool graphics_path::add_move(int x, int y)
   {

      element e;

      e.m_etype               = element::type_move;
      e.m_move.m_x            = x;
      e.m_move.m_y            = y;

      m_elementa.add(e);

      return true;

   }

   bool graphics_path::add_line(int x, int y)
   {

      element e;

      e.m_etype               = element::type_line;
      e.m_line.m_x            = x;
      e.m_line.m_y            = y;

      m_elementa.add(e);

      return true;

   }

   bool graphics_path::begin_figure(bool bFill, ::ca::e_fill_mode efillmode)
   {

      m_bFill = bFill;

      m_efillmode = efillmode;


   }

   bool graphics_path::end_figure(bool bClose)
   {

      if(bClose)
      {

         element e;

         e.m_etype               = element::type_close;

         m_elementa.add(e);

      }

      return true;

   }


} // namespace lnx





