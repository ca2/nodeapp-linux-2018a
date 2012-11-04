#pragma once


// backlink at inha-ubuntu at veriverse at Curitiba near Finados Holyday 2012-11-03 from ca.dylib/ca.so/ca.dll
namespace ca
{

   class CLASS_DECL_c null
   {
   public:


      null() {}


   };

} // namespace ca




class osdisplay_dataptra;



class CLASS_DECL_c osdisplay
{
public:


   class CLASS_DECL_c data
   {
   public:

      Display *               m_pdisplay;
      Atom                    m_atomLongType;
      Atom                    m_atomLongStyle;
      Atom                    m_atomLongStyleEx;

   };

private:


   data *   m_pdata;


   static osdisplay_dataptra * s_pdataptra;
   static int find(Display * pdisplay);
   static data * get(Display * pdisplay);

public:


   osdisplay();
   osdisplay(const ::ca::null & null);
   osdisplay(Display * pdisplay);
   osdisplay(const osdisplay & osdisplay);


   static bool remove(Display * pdisplay);


   osdisplay & operator = (const osdisplay & osdisplay);

   bool operator == (const osdisplay & d) const
   {
      return m_pdata == d.m_pdata;
   }

   bool operator != (const osdisplay & d) const
   {
      return m_pdata != d.m_pdata;
   }

   Display * display()
   {
      return m_pdata == NULL ? NULL : m_pdata->m_pdisplay;
   }

   Atom atom_long_type()
   {
      return m_pdata == NULL ? 0 : m_pdata->m_atomLongType;
   }

   Atom atom_long_style()
   {
      return m_pdata == NULL ? 0 : m_pdata->m_atomLongStyle;
   }

   Atom atom_long_style_ex()
   {
      return m_pdata == NULL ? 0 : m_pdata->m_atomLongStyleEx;
   }

   bool is_null() const
   {
      return m_pdata == NULL;
   }

   Atom get_window_long_atom(int nIndex);

};
