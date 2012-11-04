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


namespace user
{

   class interaction;

} // namespace user

class oswindow_dataptra;

class CLASS_DECL_c oswindow
{
public:


   class CLASS_DECL_c data
   {
   public:

      Display *               m_pdisplay;
      Window                  m_window;
      ::user::interaction *   m_pui;

   };

private:






   data *   m_pdata;


   static oswindow_dataptra * s_pdataptra;
   static int find(Display * pdisplay, Window window);
   static data * get(Display * pdisplay, Window window);



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
      return m_pdata == NULL ? NULL : m_pdata->m_pdisplay;
   }

   Display * display() const
   {
      return m_pdata == NULL ? NULL : m_pdata->m_pdisplay;
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

   void set_user_interaction(::user::interaction * pui);
   ::user::interaction * get_user_interaction();
   ::user::interaction * get_user_interaction() const;


   void post_nc_destroy();

};


inline bool IsWindow(oswindow oswindow)
{
   return oswindow.get_user_interaction() != NULL;
}
