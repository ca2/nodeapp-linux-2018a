#pragma once


// backlink at inha-ubuntu at veriverse at Curitiba near Finados Holyday 2012-11-03 from ca.dylib/ca.so/ca.dll
namespace ca
{

   class CLASS_DECL_c null
   {
   public:


      null() {}
      null(const null &) {}
      null(null && ) {}


null & operator = (null &&)
{

return *this;
}

template < typename T >
      operator T*() { return (T *) 0; }


   };

} // namespace ca

inline ::ca::null null()
{
    return ::ca::null();
}


template < typename T >
inline bool operator != (const T t, const ::ca::null & n)
{
   return t != ((int_ptr) 0);
}

template < typename T >
inline bool operator != (const ::ca::null & n, const T t)
{
   return t != ((int_ptr) 0);
}

template < typename T >
inline bool operator == (const T t, const ::ca::null & n)
{
   return t == ((int_ptr) 0);
}

template < typename T >
inline bool operator == (const ::ca::null & n, const T t)
{
   return t == ((int_ptr) 0);
}

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



   data *   m_pdata;


   static osdisplay_dataptra * s_pdataptra;
   static simple_mutex * s_pmutex;
   static int32_t find(Display * pdisplay);
   static data * get(Display * pdisplay);



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

   Atom get_window_long_atom(int32_t nIndex);

};
