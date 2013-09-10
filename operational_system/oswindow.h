#pragma once


// define portable types for 32-bit / 64-bit OS
typedef int32_t BOOL;
typedef uint8_t BYTE;
//typedef uint16_t WORD;
typedef uint32_t DWORD;
//typedef int32_t LONG;


#undef FAR
#undef  NEAR
#define FAR
#define NEAR
#ifndef CONST
#define CONST               const
#endif


#ifdef __cplusplus

template < typename t1, typename t2 >
class simple_map;



struct hthread;

namespace ca2
{


   class thread_base;


} // namespace ca2


//class oswindow_data;


//typedef oswindow_data * oswindow;


namespace user
{


   class interaction_base;
   class interaction;


} // namespace user


class oswindow_dataptra;


class simple_event;


class simple_mutex;


//typedef hthread * HTHREAD;


class CLASS_DECL_c oswindow_data
{
public:


   osdisplay                     m_osdisplay;
   Window                        m_window;
   Visual *                      m_pvisual;
   bool                          m_bMessageOnlyWindow;
   ::user::interaction_base *    m_pui;
   HTHREAD                       m_hthread;
   simple_map < int, LONG > *    m_plongmap;
   bool                          m_bDestroying;


   static oswindow_dataptra * s_pdataptra;
   static simple_mutex * s_pmutex;

   static Atom s_atomLongType;
   static Atom s_atomLongStyle;
   static Atom s_atomLongStyleEx;

   static Atom get_window_long_atom(int32_t nIndex);




   oswindow_data();
   oswindow_data(::user::interaction_base * puibaseMessageOnlyWindow);
   oswindow_data(const void * p);
   oswindow_data(const LPARAM & lparam);
   oswindow_data(const WPARAM & wparam);


   ~oswindow_data();



   operator void * ()
   {
      return this;
   }

   operator void * () const
   {
      return (void *) this;
   }

   oswindow_data & operator = (const oswindow_data & window);

   bool operator == (const void * p) const
   {
      return this == p;
   }

   bool operator != (const void * p) const
   {
      return this != p;
   }

   Display * display()
   {
      return this == NULL ? NULL : m_osdisplay->display();
   }

   Display * display() const
   {
      return this == NULL ? NULL : m_osdisplay->display();
   }

   Window window()
   {
      return this == NULL ? None : m_window;
   }

   Window window() const
   {
      return this == NULL ? None : m_window;
   }

   Visual * visual()
   {
      return this == NULL ? NULL : m_pvisual;
   }

   Visual * visual() const
   {
      return this == NULL ? NULL : m_pvisual;
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


   bool is_child(oswindow oswindowcandidateChildOrDescendant); // or descendant
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
      return this == NULL;
   }


   bool is_destroying();


};


CLASS_DECL_c int32_t oswindow_find_message_only_window(::user::interaction_base * puibaseMessageWindow);
CLASS_DECL_c int32_t oswindow_find(Display * pdisplay, Window window);
CLASS_DECL_c int32_t oswindow_find(Window window);
CLASS_DECL_c oswindow_data * oswindow_get_message_only_window(::user::interaction_base * puibaseMessageWindow);
CLASS_DECL_c oswindow_data * oswindow_get(Display * pdisplay, Window window, Visual * pvisual = NULL);
CLASS_DECL_c oswindow_data * oswindow_get(Window window);
CLASS_DECL_c oswindow oswindow_defer_get(Window w);
CLASS_DECL_c bool oswindow_remove(Display * pdisplay, Window window);
CLASS_DECL_c bool oswindow_remove_message_only_window(::user::interaction_base * puibaseMessageOnlyWindow);




#define HWND_MESSAGE ((::oswindow_data *) (int_ptr) 1)

inline bool IsChild(oswindow oswindowParent, ::oswindow oswindowcandidateChildOrDescendant)
{
   return oswindowParent->is_child(oswindowcandidateChildOrDescendant);
}

inline oswindow GetParent(::oswindow oswindow)
{
   return oswindow->get_parent();
}

inline oswindow SetParent(::oswindow oswindow, ::oswindow oswindowNewParent)
{
   return oswindow->set_parent(oswindowNewParent);
}

inline bool ShowWindow(::oswindow oswindow, int32_t nCmdShow)
{
   return oswindow->show_window(nCmdShow);
}

inline LONG GetWindowLongA(::oswindow oswindow, int32_t nIndex)
{
   return oswindow->get_window_long(nIndex);
}

inline LONG SetWindowLongA(::oswindow oswindow, int32_t nIndex, LONG l)
{
   return oswindow->set_window_long(nIndex, l);
}

inline bool ClientToScreen(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow->client_to_screen(lppoint);
}

inline bool ScreenToClient(::oswindow oswindow, LPPOINT lppoint)
{
   return oswindow->screen_to_client(lppoint);
}

inline int32_t IsIconic(::oswindow oswindow)
{
   return oswindow->is_iconic();
}

inline int32_t IsWindowVisible(::oswindow oswindow)
{
   return oswindow->is_window_visible();
}


#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA



CLASS_DECL_c bool IsWindow(oswindow oswindow);


#endif
