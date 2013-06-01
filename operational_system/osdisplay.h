#pragma once


class osdisplay_dataptra;
class simple_mutex;


class CLASS_DECL_c xdisplay
{
public:

    Display *   m_pdisplay;
    bool        m_bLocked;
    bool        m_bOwn;

    xdisplay();
    xdisplay(Display * pdisplay, bool bInitialLock = true);
    ~ xdisplay();

    bool open(char * display_name, bool bInitialLock = true);

    void lock();
    void unlock();


    bool close();

    operator Display *()
    {
        return m_pdisplay;
    }


    Window default_root_window();

    int default_screen();

};




class CLASS_DECL_c osdisplay_data
{
public:



   Display *               m_pdisplay;
   Atom                    m_atomLongType;
   Atom                    m_atomLongStyle;
   Atom                    m_atomLongStyleEx;


   static osdisplay_dataptra * s_pdataptra;
   static simple_mutex * s_pmutex;



   osdisplay_data();
   //osdisplay_data(Display * pdisplay);




   Display * display()
   {
      return this == NULL ? NULL : m_pdisplay;
   }

   Display * display() const
   {
      return this == NULL ? NULL : m_pdisplay;
   }

   Atom atom_long_type()
   {
      return this == NULL ? 0 : m_atomLongType;
   }

   Atom atom_long_style()
   {
      return this == NULL ? 0 : m_atomLongStyle;
   }

   Atom atom_long_style_ex()
   {
      return this == NULL ? 0 : m_atomLongStyleEx;
   }

   bool is_null() const
   {
      return this == NULL;
   }

   Atom get_window_long_atom(int32_t nIndex);

};


CLASS_DECL_c int32_t osdisplay_find(Display * pdisplay);
CLASS_DECL_c osdisplay_data * osdisplay_get(Display * pdisplay);
CLASS_DECL_c bool osdisplay_remove(Display * pdisplay);


typedef osdisplay_data * osdisplay;

