#pragma once

/* xxx
template <>
inline UINT HashKey < HWND >(HWND key)
{
   // default identity hash - works for most primitive values
   return (DWORD)((dword_ptr)key);
}

template <>
inline UINT HashKey < HMENU >(HMENU key)
{
   // default identity hash - works for most primitive values
   return (DWORD)((dword_ptr)key);
}

template <>
inline UINT HashKey < HDC >(HDC key)
{
   // default identity hash - works for most primitive values
   return (DWORD)((dword_ptr)key);
}

template <>
inline UINT HashKey < HGDIOBJ >(HGDIOBJ key)
{
   // default identity hash - works for most primitive values
   return (DWORD)((dword_ptr)key);
}

template <>
nline UINT HashKey < HIMAGELIST >(HIMAGELIST key)
{
   // default identity hash - works for most primitive values
   return (DWORD)((dword_ptr)key);
}


namespace win
{

   class window;
   class graphics;
   class graphics_object;

   template < int t_iHandleCount >
   class handle_base
   {
   public:
      HANDLE m_handlea[t_iHandleCount];
      static const int s_iHandleCount = t_iHandleCount;
   };

   template < class H >
   class handle1 : public handle_base < 1 >
   {
   public:
      handle1() { m_handlea[0] = NULL; }
      typedef handle_base < 1 > HANDLE_BASE_TYPE;
      inline H get_handle() const { return static_cast < H > (m_handlea[0]); }
      inline operator H () const { return get_handle(); }
      inline H set_handle(H h) { return static_cast < H > (m_handlea[0] = static_cast < HANDLE > (h)); }
   };

   template < class H1, class H2 >
   class handle2 : public handle_base < 2 >
   {
   public:
      handle2() { m_handlea[0] = NULL; m_handlea[1] = NULL; }
      typedef handle_base < 2 > HANDLE_BASE_TYPE;
      inline H1 get_os_data() const { return static_cast < H1 > (m_handlea[0]); }
      inline H2 get_handle2() const { return static_cast < H2 > (m_handlea[0]); }
      inline H1 set_handle1(H1 h) { return static_cast < H1 > (m_handlea[0] = static_cast < HANDLE > (h)); }
      inline H2 set_handle2(H2 h) { return static_cast < H2 > (m_handlea[1] = static_cast < HANDLE > (h)); }
   };

   typedef handle1 < HWND > hwnd_handle;
   typedef handle1 < HMENU > hmenu_handle;
   typedef handle2 < HDC, HDC > hdc_handle;
   typedef handle1 < HGDIOBJ > hgdiobj_handle;
   typedef handle1 < HIMAGELIST > himagelist_handle;

} // namespace win


#include "ca/radix/radix_fixed_alloc.h"

template<class TYPE>
struct ConstructDestruct
{
   static void PASCAL Construct(::radix::object* pObject)
   {
      new (pObject) TYPE;
   }
   static void PASCAL Destruct(::radix::object* pObject)
   {
      TYPE* p = dynamic_cast < TYPE * > (pObject);
      p->~TYPE();
   }
   static void PASCAL Construct(TYPE * pObject)
   {
#undef new
      new (pObject) TYPE;
#define new DEBUG_NEW
   }
   static void PASCAL Destruct(TYPE * pObject)
   {
      TYPE* p = dynamic_cast < TYPE * > (pObject);
      p->~TYPE();
   }
};

class WindowsThread;       // forward reference for friend declaration


template < class HT, class CT >
class handle_map
{
public:


   fixed_alloc_no_sync     m_alloc;
   mutex                   m_mutex;
   void (PASCAL* m_pfnConstructObject)(CT* pObject);
   void (PASCAL* m_pfnDestructObject)(CT* pObject);
   ::collection::map < HANDLE, HANDLE, CT *, CT *> m_permanentMap;
   ::collection::map < HANDLE, HANDLE, CT *, CT *> m_temporaryMap;

   handle_map();
   virtual ~handle_map()
   {
      delete_temp();
   }

// Operations
public:
   CT * from_handle(HANDLE h, CT * (* pfnAllocator) (::ca::application *, HANDLE) = NULL, ::ca::application * papp = NULL);
   void delete_temp();

   void set_permanent(HANDLE h, CT * permOb);
   void remove_handle(HANDLE h);

   CT * lookup_permanent(HANDLE h);
   CT * lookup_temporary(HANDLE h);

   friend class ::radix::thread;
};

class CLASS_DECL_win hwnd_map :
   public handle_map < ::win::hwnd_handle, ::win::window >
{
public:
};

/*class CLASS_DECL_win hdc_map :
   public handle_map < ::win::hdc_handle, ::win::graphics >
{
public:
};*/

/*class hgdiobj_map :
   public handle_map < ::win::hgdiobj_handle, ::win::graphics_object >
{
public:
};*/

/*
class CLASS_DECL_win hdc_map :
   public handle_map < ::win::hmenu_handle, ::win::menu >
{
public:
};
*/

/* xxx

template < class HT, class CT >
handle_map < HT, CT > ::handle_map() :
      m_permanentMap(1024),
      m_temporaryMap(1024),
      m_alloc(sizeof(CT), 1024)
{

   ASSERT(HT::s_iHandleCount == 1 || HT::s_iHandleCount == 2);

   m_permanentMap.InitHashTable(1024, TRUE);
   m_temporaryMap.InitHashTable(1024, TRUE);

   m_pfnConstructObject    = ConstructDestruct<CT>::Construct;
   m_pfnDestructObject     = ConstructDestruct<CT>::Destruct;

}

template < class HT, class CT >
CT* handle_map < HT, CT >::from_handle(HANDLE h, CT * (*pfnAllocator) (::ca::application *, HANDLE), ::ca::application * papp)
{

   single_lock sl(&m_mutex, TRUE);

   ASSERT(HT::s_iHandleCount == 1 || HT::s_iHandleCount == 2);

   if (h == NULL)
      return NULL;

   CT* pObject = lookup_permanent(h);
   if (pObject != NULL && pObject->get_os_data() == (void *) h)
      return pObject;   // return permanent one
   else if ((pObject = lookup_temporary(h)) != NULL && pObject->get_os_data() == (void *) h)
   {
      HANDLE* ph = pObject->m_handlea;
      ASSERT(ph[0] == h || ph[0] == NULL);
      ph[0] = h;
      if (HT::s_iHandleCount == 2)
      {
         ASSERT(ph[1] == h || ph[1] == NULL);
         ph[1] = h;
      }
      return pObject;   // return current temporary one
   }

   // This handle wasn't created by us, so we must create a temporary
   // C++ object to wrap it.  We don't want the ::fontopus::user to see this primitive::memory
   // allocation, so we turn tracing off.

   //bool bEnable = __enable_memory_tracking(FALSE);
#ifndef ___PORTABLE
   _PNH pnhOldHandler = __set_new_handler(&__critical_new_handler);
#endif

   CT* pTemp = NULL;
   try
   {
      if(pfnAllocator != NULL)
      {
         pTemp = pfnAllocator(papp, h);
         if (pTemp == NULL)
            throw memory_exception();
      }
      else
      {
         // get primitive::memory for the object from the fixed allocator
   //      ASSERT((UINT)m_pClass->m_nObjectSize == m_alloc.GetAllocSize());
         pTemp = (CT*)m_alloc.Alloc();
         if (pTemp == NULL)
            throw memory_exception();

         // now construct the object in place
         ASSERT(m_pfnConstructObject != NULL);
         (*m_pfnConstructObject)(pTemp);
      }

      // set it in the ::collection::map
      m_temporaryMap.set_at(h, pTemp);
   }
   catch(base_exception * pe)
   {
#ifndef ___PORTABLE
      __set_new_handler(pnhOldHandler);
#endif
      //__enable_memory_tracking(bEnable);
      ::ca::rethrow(pe);
   }


#ifndef ___PORTABLE
   __set_new_handler(pnhOldHandler);
#endif
   //__enable_memory_tracking(bEnable);

   // now set the handle in the object
   HANDLE* ph = pTemp->m_handlea;  // after ::radix::object
   ph[0] = h;
   if (HT::s_iHandleCount == 2)
      ph[1] = h;

   return pTemp;
}

#ifdef DEBUG   // out-of-line version for primitive::memory tracking
template < class HT, class CT >
void handle_map < HT, CT >::set_permanent(HANDLE h, CT * permOb)
{

   single_lock sl(&m_mutex, TRUE);

   //bool bEnable = __enable_memory_tracking(FALSE);
   m_permanentMap[(LPVOID)h] = permOb;
   //__enable_memory_tracking(bEnable);

}
#endif //DEBUG

#ifdef DEBUG
template < class HT, class CT >
void handle_map < HT, CT > ::remove_handle(HANDLE h)
{

   single_lock sl(&m_mutex, TRUE);

   // make sure the handle entry is consistent before deleting
   CT* pTemp = lookup_temporary(h);
   if (pTemp != NULL)
   {
      // temporary objects must have correct handle values
      HANDLE* ph = pTemp->m_handlea;
      ASSERT(ph[0] == h || ph[0] == NULL);
      if (HT::s_iHandleCount == 2)
      {
         ASSERT(ph[1] == h || ph[1] == NULL);
      }
   }
   pTemp = lookup_permanent(h);
   if (pTemp != NULL)
   {
      HANDLE* ph = pTemp->m_handlea;
      ASSERT(ph[0] == h);
      // permanent object may have secondary handles that are different
   }
   // remove only from permanent ::collection::map -- temporary objects are removed
   //  at idle in CHandleMap::delete_temp, always!
   m_permanentMap.remove_key((LPVOID)h);
}
#endif

template < class HT, class CT >
void handle_map < HT, CT >::delete_temp()
{

   single_lock sl(&m_mutex, TRUE);

   if (::ca::is_null(this))
      return;

   POSITION pos = m_temporaryMap.get_start_position();
   while (pos != NULL)
   {
      HANDLE h; // just used for asserts
      CT * pTemp;
      m_temporaryMap.get_next_assoc(pos, h, pTemp);

      // zero out the handles
      ASSERT(HT::s_iHandleCount == 1 || HT::s_iHandleCount == 2);
      HANDLE* ph = pTemp->m_handlea;  // after ::radix::object
      ASSERT(ph[0] == h || ph[0] == NULL);
      ph[0] = NULL;
      if (HT::s_iHandleCount == 2)
      {
         ASSERT(ph[1] == h || ph[1] == NULL);
         ph[1] = NULL;
      }

      ASSERT(m_pfnDestructObject != NULL);
      pTemp->m_papp = NULL;
      (*m_pfnDestructObject)(pTemp);   // destruct the object
   }

   m_temporaryMap.remove_all();       // free up dictionary links etc
   m_alloc.FreeAll();   // free all the primitive::memory used for these temp objects
}


// Note: out-of-line DEBUG version is in winhand.cpp
#ifndef DEBUG
template < class HT, class CT >
inline void handle_map < HT, CT >::set_permanent(HANDLE h, CT * permOb)
   { m_permanentMap[(HANDLE)h] = permOb; }

template < class HT, class CT >
inline void handle_map < HT, CT >::remove_handle(HANDLE h)
{
   // remove only from permanent ::collection::map -- temporary objects are removed
   //  at idle in CHandleMap::delete_temp, always!
   m_permanentMap.remove_key((HANDLE)h);
}
#endif

template < class HT, class CT >
inline CT* handle_map <HT, CT>::lookup_permanent(HANDLE h)
{

   single_lock sl(&m_mutex, TRUE);

   CT * pt = m_permanentMap.get(h, (CT*) NULL);
   if(pt != NULL && pt->get_os_data() == (void *) h)
      return pt;
   else
      return NULL;

}

template < class HT, class CT >
inline CT* handle_map <HT, CT>::lookup_temporary(HANDLE h)
{

   single_lock sl(&m_mutex, TRUE);

   CT * pt = m_temporaryMap.get(h, (CT*) NULL);
   if(pt != NULL && pt->get_os_data() == (void *) h)
      return pt;
   else
      return NULL;

}


CLASS_DECL_win hwnd_map * PASCAL afxMapHWND(bool bCreate = FALSE);
CLASS_DECL_win mutex * PASCAL afxMutexHwnd();
CLASS_DECL_win himagelist_map * PASCAL afxMapHIMAGELIST(bool bCreate = FALSE);
//CLASS_DECL_win hdc_map * PASCAL afxMapHDC(bool bCreate = FALSE);
//CLASS_DECL_win hgdiobj_map * PASCAL afxMapHGDIOBJ(bool bCreate = FALSE);
//CLASS_DECL_win hmenu_map * PASCAL afx_map_HMENU(bool bCreate = FALSE);

*/
