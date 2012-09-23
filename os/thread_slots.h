
#ifndef __AFXTLS_H__
#define __AFXTLS_H__

#pragma once

// Classes declared in this file

//class simple_list;
class thread_slot_data;                  // for manipulationg thread local storage
template < int iSlot >
class thread_local_object;               // for storing thread local data
class process_local_object;              // for storing thread local data
class no_track_object;

// template class typed_simple_list<>
// template class thread_local<>
// template class process_local<>

/////////////////////////////////////////////////////////////////////////////
// thread_slot_data - manages owned base_array of "slots" for thread local storage

struct thread_data; // private to implementation
struct slot_data;   // private to implementation


class CLASS_DECL_LNX thread_slot_data
{
public:


   no_track_object *                           m_pa[1024]; // state of each slot (allocated or not)


   thread_slot_data();
   void delete_data();

};


class CLASS_DECL_LNX thread_local_storage
{
public:

   DWORD                            m_tlsIndex;   // used to access system thread-local storage
   thread_slot_data *               m_pthreadslotdata;


   thread_local_storage();
   ~thread_local_storage();
   void delete_data();

   thread_slot_data * get_slot_data();
#undef new
   void * PASCAL operator new(size_t, void * p)
      { return p; }
#define new DEBUG_NEW
};


extern BYTE _gen_ThreadData[sizeof(thread_local_storage)];
extern thread_local_storage * gen_ThreadData;


class CLASS_DECL_LNX no_track_object
{
public:
#undef new
   void * PASCAL operator new(size_t nSize);
#define new DEBUG_NEW
   void PASCAL operator delete(void *);

#if defined(DEBUG) && !defined(___NO_DEBUG_CRT)
#undef new
   void * PASCAL operator new(size_t nSize, const char *, int);
#define new DEBUG_NEW
   void PASCAL operator delete(void * pObject, const char *, int);
#endif
    virtual ~no_track_object() {};
};

template < int iSlot >
class __NOVTABLE thread_local_object
{
public:
// Attributes
   no_track_object* get_data(no_track_object* ( * pfnCreateObject)());
   no_track_object* GetDataNA();

// Implementation
   //int m_nSlot;
   ~thread_local_object();
};


template < int iSlot >
no_track_object* thread_local_object < iSlot> ::get_data(no_track_object* ( * pfnCreateObject)())
{

   if (gen_ThreadData == NULL)
   {
#undef new
      gen_ThreadData = new(_gen_ThreadData) thread_local_storage;
#define new DEBUG_NEW
      ENSURE(gen_ThreadData != NULL);
   }

   no_track_object * pValue = (no_track_object *) gen_ThreadData->get_slot_data()->m_pa[iSlot];

   if(pValue == NULL)
   {

      // allocate zero-init object
      pValue =  (*pfnCreateObject)();

      // set tls data to newly created object
      gen_ThreadData->get_slot_data()->m_pa[iSlot] = pValue;
      ASSERT(gen_ThreadData->get_slot_data()->m_pa[iSlot] == pValue);

   }

   return pValue;

}
template < int iSlot >
no_track_object* thread_local_object < iSlot > ::GetDataNA()
{
   return gen_ThreadData->get_slot_data()->m_pa[iSlot];
}
template < int iSlot >
thread_local_object < iSlot > ::~thread_local_object()
{
//   if (m_nSlot != 0 && gen_ThreadData != NULL)
  //    gen_ThreadData->FreeSlot(m_nSlot);
   //m_nSlot = 0;
}


class CLASS_DECL_LNX process_local_object
{
public:

   mutex          m_mutex;

// Attributes
   no_track_object* get_data(no_track_object* (* pfnCreateObject)());

// Implementation
   no_track_object* volatile m_pObject;
   ~process_local_object();
};

template < class TYPE, int iSlot >
class thread_local :
   public thread_local_object < iSlot >
{
// Attributes
public:
   inline TYPE* get_data()
   {
      return (TYPE *) thread_local_object < iSlot >::get_data(&CreateObject);
      //ENSURE(pData != NULL);
      //return pData;
   }
   inline TYPE* GetDataNA()
   {
      TYPE* pData = (TYPE*)thread_local_object < iSlot >::GetDataNA();
      return pData;
   }
   inline operator TYPE*()
   {
      return get_data();
   }
   inline TYPE* operator->()
   {
      return get_data();
   }

// Implementation
public:
   static no_track_object* CreateObject()
      { return new TYPE; }
};

#define THREAD_LOCAL(class_name, ident_name, slot) \
   __COMDAT thread_local<class_name, slot> ident_name;
#define EXTERN_THREAD_LOCAL(class_name, ident_name, slot) \
   extern CLASS_DECL_LNX thread_local<class_name, slot> ident_name;

template<class TYPE>
class process_local : public process_local_object
{
// Attributes
public:
   inline TYPE* get_data()
   {
      TYPE* pData = (TYPE*)process_local_object::get_data(&CreateObject);
      ENSURE(pData != NULL);
      return pData;
   }
   inline TYPE* GetDataNA()
      { return (TYPE*)m_pObject; }
   inline operator TYPE*()
      { return get_data(); }
   inline TYPE* operator->()
      { return get_data(); }

// Implementation
public:
   static no_track_object* CreateObject()
      { return new TYPE; }
};

#define PROCESS_LOCAL(class_name, ident_name) \
   __COMDAT process_local<class_name> ident_name;
#define EXTERN_PROCESS_LOCAL(class_name, ident_name) \
   extern process_local<class_name> ident_name;

/////////////////////////////////////////////////////////////////////////////

void CLASS_DECL_LNX __init_local_data(HINSTANCE hInstInit);
void CLASS_DECL_LNX __term_local_data(HINSTANCE hInstTerm, bool bAll = FALSE);
void CLASS_DECL_LNX __tls_add_ref();
void CLASS_DECL_LNX __tls_release();

#endif //__AFXTLS_H__

/////////////////////////////////////////////////////////////////////////////
