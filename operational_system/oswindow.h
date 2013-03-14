#pragma once


// define portable types for 32-bit / 64-bit OS
typedef int32_t BOOL;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;


#undef FAR
#undef  NEAR
#define FAR
#define NEAR
#ifndef CONST
#define CONST               const
#endif

template < typename t1, typename t2 >
class simple_map;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;


namespace user
{


   class interaction_base;
   class interaction;


} // namespace user


class oswindow_dataptra;


class simple_event;


class simple_mutex;


typedef simple_event * HTHREAD;


class CLASS_DECL_c oswindow
{
public:


   class CLASS_DECL_c data
   {
   public:

      data();
      ~data();

      osdisplay                     m_osdisplay;
      Window                        m_window;
      Visual *                      m_pvisual;
      ::user::interaction_base *    m_pui;
      HTHREAD                       m_hthread;
      simple_map < int, LONG > *    m_plongmap;
      bool                          m_bDestroying;

   };







   data *   m_pdata;


   static oswindow_dataptra * s_pdataptra;
   static simple_mutex * s_pmutex;

   static int32_t find(Display * pdisplay, Window window);
   static int32_t find(Window window);
   static data * get(Display * pdisplay, Window window);
   static data * get(Window window);
   static Atom s_atomLongType;
   static Atom s_atomLongStyle;
   static Atom s_atomLongStyleEx;

   static Atom get_window_long_atom(int32_t nIndex);




   oswindow();
   oswindow(const ::ca::null & null);
   oswindow(Display * pdisplay, Window window, Visual * pvisual = NULL);
   oswindow(const oswindow & oswindow);
   oswindow(const void * p);
   oswindow(const LPARAM & lparam);
   oswindow(const WPARAM & wparam);



   operator void * ()
   {
      return m_pdata;
   }

   operator void * () const
   {
      return m_pdata;
   }

   static bool remove(Display * pdisplay, Window window);


   oswindow & operator = (const oswindow & window);

   bool operator == (const void * p) const
   {
      return m_pdata == p;
   }

   bool operator != (const void * p) const
   {
      return m_pdata != p;
   }

   bool operator == (const oswindow & w) const
   {
      return m_pdata == w.m_pdata;
   }

   bool operator != (const oswindow & w) const
   {
      return m_pdata != w.m_pdata;
   }

   Display * display()
   {
      return m_pdata == NULL ? NULL : m_pdata->m_osdisplay.display();
   }

   Display * display() const
   {
      return m_pdata == NULL ? NULL : m_pdata->m_osdisplay.display();
   }

   Window window()
   {
      return m_pdata == NULL ? 0 : m_pdata->m_window;
   }

   Window window() const
   {
      return m_pdata == NULL ? 0 : m_pdata->m_window;
   }

   Visual * visual()
   {
      return m_pdata == NULL ? NULL : m_pdata->m_pvisual;
   }

   Visual * visual() const
   {
      return m_pdata == NULL ? NULL : m_pdata->m_pvisual;
   }


   int32_t store_name(const char * psz);
   int32_t select_input(int32_t iInput);
   int32_t select_all_input();
   int32_t map_window();

   void set_user_interaction(::user::interaction_base * pui);
   ::user::interaction_base * get_user_interaction_base();
   ::user::interaction_base * get_user_interaction_base() const;
   ::user::interaction * get_user_interaction();
   ::user::interaction * get_user_interaction() const;


   void post_nc_destroy();


   bool is_child(oswindow oswindowCandidateChildOrDescendant); // or descendant
   oswindow get_parent();
   oswindow set_parent(oswindow oswindowNewParent);
   long get_state();
   bool is_iconic();
   bool is_window_visible();
   bool show_window(int32_t nCmdShow);
   LONG get_window_long(int32_t nIndex);
   LONG set_window_long(int32_t nIndex, LONG l);
   bool client_to_screen(LPPOINT lppoint);
   bool screen_to_client(LPPOINT lppoint);



   bool is_null() const
   {
      return m_pdata == NULL;
   }


   static oswindow defer_get(Window w);

   bool is_destroying();


};



inline bool IsChild(oswindow oswindowParent, ::oswindow oswindowCandidateChildOrDescendant)
{
   return oswindowParent.is_child(oswindowCandidateChildOrDescendant);
}

inline oswindow GetParent(::oswindow oswindow)
{
   return oswindow.get_parent();
}

inline oswindow SetParent(::oswindow oswindow, ::oswindow oswindowNewParent)
{
   return oswindow.set_parent(oswindowNewParent);
}

inline bool ShowWindow(::oswindow oswindow, int32_t nCmdShow)
{
   return oswindow.show_window(nCmdShow);
}

inline LONG GetWindowLongA(::oswindow oswindow, int32_t nIndex)
{
   return oswindow.get_window_long(nIndex);
}

inline LONG SetWindowLongA(::oswindow oswindow, int32_t nIndex, LONG l)
{
   return oswindow.set_window_long(nIndex, l);
}

inline bool ClientToScreen(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow.client_to_screen(lppoint);
}

inline bool ScreenToClient(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow.screen_to_client(lppoint);
}

inline int32_t IsIconic(::oswindow oswindow)
{
   return oswindow.is_iconic();
}

inline int32_t IsWindowVisible(::oswindow oswindow)
{
   return oswindow.is_window_visible();
}


#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA



CLASS_DECL_c bool IsWindow(oswindow oswindow);
