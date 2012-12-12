#pragma once


// define portable types for 32-bit / 64-bit OS
#include <inttypes.h>
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



typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;


class simple_event;


namespace radix
{

   class thread;

}

namespace ca
{

   class CLASS_DECL_c thread_base
   {
   public:


      ::radix::thread      * m_pthread;


      virtual simple_event * get_os_handle() const = 0;

   };

};


namespace user
{


   class interaction;


   class CLASS_DECL_c interaction_base
   {
   public:


      ::ca::thread_base *     m_pthread;
      ::user::interaction *   m_pui;


   };


} // namespace user



class oswindow_dataptra;




class CLASS_DECL_c oswindow
{
public:


   class CLASS_DECL_c data
   {
   public:

      osdisplay                     m_osdisplay;
      Window                        m_window;
      ::user::interaction_base *    m_pui;

   };

private:






   data *   m_pdata;


   static oswindow_dataptra * s_pdataptra;
   static int find(Display * pdisplay, Window window);
   static data * get(Display * pdisplay, Window window);
   static Atom s_atomLongType;
   static Atom s_atomLongStyle;
   static Atom s_atomLongStyleEx;

   static Atom get_window_long_atom(int nIndex);


public:


   oswindow();
   oswindow(const ::ca::null & null);
   oswindow(Display * pdisplay, Window window);
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

   int store_name(const char * psz);
   int select_input(int iInput);
   int select_all_input();
   int map_window();

   void set_user_interaction(::user::interaction_base * pui);
   ::user::interaction_base * get_user_interaction_base();
   ::user::interaction_base * get_user_interaction_base() const;
   ::user::interaction * get_user_interaction();
   ::user::interaction * get_user_interaction() const;


   void post_nc_destroy();


   bool is_child(oswindow oswindowCandidateChildOrDescendant); // or descendant
   oswindow get_parent();
   bool show_window(int nCmdShow);
   LONG get_window_long(int nIndex);
   LONG set_window_long(int nIndex, LONG l);
   bool client_to_screen(LPPOINT lppoint);
   bool screen_to_client(LPPOINT lppoint);

   bool is_null() const
   {
      return m_pdata == NULL;
   }

};


inline bool IsWindow(oswindow oswindow)
{
   return oswindow.get_user_interaction() != NULL;
}

inline bool IsChild(oswindow oswindowParent, ::oswindow oswindowCandidateChildOrDescendant)
{
   return oswindowParent.is_child(oswindowCandidateChildOrDescendant);
}

inline oswindow GetParent(::oswindow oswindow)
{
   return oswindow.get_parent();
}

inline bool ShowWindow(::oswindow oswindow, int nCmdShow)
{
   return oswindow.show_window(nCmdShow);
}

inline LONG GetWindowLong(::oswindow oswindow, int nIndex)
{
   return oswindow.get_window_long(nIndex);
}

inline LONG SetWindowLong(::oswindow oswindow, int nIndex, LONG l)
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
