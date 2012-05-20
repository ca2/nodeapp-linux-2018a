#pragma once


#if !defined(_MAC) && (defined(_M_MRX000) || defined(_M_AMD64) || defined(_M_IA64)) && (_MSC_VER >= 1100) && !(defined(MIDL_PASS) || defined(RC_INVOKED))
 #define POINTER_64 __ptr64
 typedef unsigned int64_t POINTER_64_INT;
 #if defined(_WIN64)
  #define POINTER_32 __ptr32
 #else
  #define POINTER_32
 #endif
#else
 #if defined(_MAC) && defined(_MAC_INT_64)
  #define POINTER_64 __ptr64
  typedef unsigned int64_t POINTER_64_INT;
 #else
  #if (_MSC_VER >= 1300) && !(defined(MIDL_PASS) || defined(RC_INVOKED))
   #define POINTER_64 __ptr64
  #else
   #define POINTER_64
  #endif
  typedef unsigned long POINTER_64_INT;
 #endif
 #define POINTER_32
#endif

#if defined(_IA64_) || defined(_AMD64_)
#define FIRMWARE_PTR
#else
#define FIRMWARE_PTR POINTER_32
#endif

//
// A compiler with support for __sptr and __uptr is in qualification.  To
// participate in qualification, remove the "0 &&" term below and also
// enable warning 4826 in warning.w
//

#if 0 && (_MSC_FULL_VER >= 140041204) && !defined(MIDL_PASS) && !defined(RC_INVOKED)
#define POINTER_SIGNED __sptr
#define POINTER_UNSIGNED __uptr
#else
#define POINTER_SIGNED
#define POINTER_UNSIGNED
#endif

#define SPOINTER_32 POINTER_SIGNED POINTER_32
#define UPOINTER_32 POINTER_UNSIGNED POINTER_32

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef signed int          INT32, *PINT32;
typedef int64_t      INT64, *PINT64;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;
typedef unsigned int        UINT32, *PUINT32;
typedef uint64_t    UINT64, *PUINT64;

//
// The following types are guaranteed to be signed and 32 bits wide.
//

typedef signed int LONG32, *PLONG32;

//
// The following types are guaranteed to be unsigned and 32 bits wide.
//

typedef unsigned int ULONG32, *PULONG32;
typedef unsigned int DWORD32, *PDWORD32;

#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif


/*typedef __int3264 INT_PTR, *PINT_PTR;
typedef unsigned __int3264 UINT_PTR, *PUINT_PTR;

typedef __int3264 LONG_PTR, *PLONG_PTR;
typedef unsigned __int3264 ULONG_PTR, *PULONG_PTR;*/


#ifdef _WIN64

#define ADDRESS_TAG_BIT 0x40000000000UI64

typedef int64_t SHANDLE_PTR;
typedef unsigned int64_t HANDLE_PTR;
typedef unsigned int UHALF_PTR, *PUHALF_PTR;
typedef int HALF_PTR, *PHALF_PTR;

#if !defined(__midl)
__inline
unsigned long
HandleToULong(
    const void *h
    )
{
    return((unsigned long) (ULONG_PTR) h );
}

__inline
long
HandleToLong(
    const void *h
    )
{
    return((long) (LONG_PTR) h );
}

__inline
void *
ULongToHandle(
    const unsigned long h
    )
{
    return((void *) (UINT_PTR) h );
}


__inline
void *
LongToHandle(
    const long h
    )
{
    return((void *) (INT_PTR) h );
}


__inline
unsigned long
PtrToUlong(
    const void  *p
    )
{
    return((unsigned long) (ULONG_PTR) p );
}

__inline
unsigned int
PtrToUint(
    const void  *p
    )
{
    return((unsigned int) (UINT_PTR) p );
}

__inline
unsigned short
PtrToUshort(
    const void  *p
    )
{
    return((unsigned short) (unsigned long) (ULONG_PTR) p );
}

__inline
long
PtrToLong(
    const void  *p
    )
{
    return((long) (LONG_PTR) p );
}

__inline
int
PtrToInt(
    const void  *p
    )
{
    return((int) (INT_PTR) p );
}

__inline
short
PtrToShort(
    const void  *p
    )
{
    return((short) (long) (LONG_PTR) p );
}

__inline
void *
IntToPtr(
    const int i
    )
// Caution: IntToPtr() sign-extends the int value.
{
    return( (void *)(INT_PTR)i );
}

__inline
void *
UIntToPtr(
    const unsigned int ui
    )
// Caution: UIntToPtr() zero-extends the unsigned int value.
{
    return( (void *)(UINT_PTR)ui );
}

__inline
void *
LongToPtr(
    const long l
    )
// Caution: LongToPtr() sign-extends the long value.
{
    return( (void *)(LONG_PTR)l );
}

__inline
void *
ULongToPtr(
    const unsigned long ul
    )
// Caution: ULongToPtr() zero-extends the unsigned long value.
{
    return( (void *)(ULONG_PTR)ul );
}

#define PtrToPtr64( p )         ((void * POINTER_64) p)
#define Ptr64ToPtr( p )         ((void *) p)
#define HandleToHandle64( h )   (PtrToPtr64( h ))
#define Handle64ToHandle( h )   (Ptr64ToPtr( h ))

__inline
void *
Ptr32ToPtr(
    const void * POINTER_32 p
    )
{
    return((void *) (ULONG_PTR) (unsigned long) p);
}

__inline
void *
Handle32ToHandle(
    const void * POINTER_32 h
    )
{
    return((void *) (LONG_PTR) (long) h);
}

__inline
void * POINTER_32
PtrToPtr32(
    const void *p
    )
{
    return((void * POINTER_32) (unsigned long) (ULONG_PTR) p);
}

#define HandleToHandle32( h )       (PtrToPtr32( h ))

#endif // !_midl

#else  // !_WIN64

#define ADDRESS_TAG_BIT 0x80000000UL

typedef unsigned short UHALF_PTR, *PUHALF_PTR;
typedef short HALF_PTR, *PHALF_PTR;
typedef _W64 long SHANDLE_PTR;
typedef _W64 unsigned long HANDLE_PTR;

#define HandleToULong( h ) ((ULONG)(ULONG_PTR)(h) )
#define HandleToLong( h )  ((LONG)(LONG_PTR) (h) )
#define ULongToHandle( ul ) ((HANDLE)(ULONG_PTR) (ul) )
#define LongToHandle( h )   ((HANDLE)(LONG_PTR) (h) )
#define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )
#define PtrToLong( p )  ((LONG)(LONG_PTR) (p) )
#define PtrToUint( p ) ((UINT)(UINT_PTR) (p) )
#define PtrToInt( p )  ((INT)(INT_PTR) (p) )
#define PtrToUshort( p ) ((unsigned short)(ULONG_PTR)(p) )
#define PtrToShort( p )  ((short)(LONG_PTR)(p) )
#define IntToPtr( i )    ((VOID *)(INT_PTR)((int)i))
#define UIntToPtr( ui )  ((VOID *)(UINT_PTR)((unsigned int)ui))
#define LongToPtr( l )   ((VOID *)(LONG_PTR)((long)l))
#define ULongToPtr( ul ) ((VOID *)(ULONG_PTR)((unsigned long)ul))

/*#if !defined(__midl)
inline
void * POINTER_64
PtrToPtr64(
    const void *p
    )
{
    return((void * POINTER_64) (uint64_t) (ULONG_PTR)p );
}

inline
void *
Ptr64ToPtr(
    const void * POINTER_64 p
    )
{
    return((void *) (ULONG_PTR) (uint64_t) p);
}

inline
void * POINTER_64
HandleToHandle64(
    const void *h
    )
{
    return((void * POINTER_64)(int64_t)(LONG_PTR)h );
}

inline
void *
Handle64ToHandle(
    const void * POINTER_64 h
    )
{
    return((void *) (ULONG_PTR) (uint64_t) h );
}
#endif*/

#define Ptr32ToPtr( p )         ((void *) p)
#define Handle32ToHandle( h )   (Ptr32ToPtr( h ))
#define PtrToPtr32( p )         ((void * POINTER_32) p)
#define HandleToHandle32( h )   (PtrToPtr32( h ))

#endif // !_WIN64

#define HandleToUlong(h)  HandleToULong(h)
#define UlongToHandle(ul) ULongToHandle(ul)
#define UlongToPtr(ul) ULongToPtr(ul)
#define UintToPtr(ui)  UIntToPtr(ui)

#define MAXUINT_PTR  (~((UINT_PTR)0))
#define MAXINT_PTR   ((INT_PTR)(MAXUINT_PTR >> 1))
#define MININT_PTR   (~MAXINT_PTR)

#define MAXULONG_PTR (~((ULONG_PTR)0))
#define MAXLONG_PTR  ((LONG_PTR)(MAXULONG_PTR >> 1))
#define MINLONG_PTR  (~MAXLONG_PTR)

#define MAXUHALF_PTR ((UHALF_PTR)~0)
#define MAXHALF_PTR  ((HALF_PTR)(MAXUHALF_PTR >> 1))
#define MINHALF_PTR  (~MAXHALF_PTR)

//
// SIZE_T used for counts or ranges which need to span the range of
// of a pointer.  SSIZE_T is the signed variation.
//

typedef ULONG_PTR SIZE_T, *PSIZE_T;
typedef LONG_PTR SSIZE_T, *PSSIZE_T;

#if _WIN32_WINNT >= 0x0600 || (defined(__cplusplus) && defined(WINDOWS_ENABLE_CPLUSPLUS))

#define MAXUINT8    ((UINT8)~((UINT8)0))
#define MAXINT8     ((INT8)(MAXUINT8 >> 1))
#define MININT8     ((INT8)~MAXINT8)

#define MAXUINT16   ((UINT16)~((UINT16)0))
#define MAXINT16    ((INT16)(MAXUINT16 >> 1))
#define MININT16    ((INT16)~MAXINT16)

#define MAXUINT32   ((UINT32)~((UINT32)0))
#define MAXINT32    ((INT32)(MAXUINT32 >> 1))
#define MININT32    ((INT32)~MAXINT32)

#define MAXUINT64   ((UINT64)~((UINT64)0))
#define MAXINT64    ((INT64)(MAXUINT64 >> 1))
#define MININT64    ((INT64)~MAXINT64)

#define MAXULONG32  ((ULONG32)~((ULONG32)0))
#define MAXLONG32   ((LONG32)(MAXULONG32 >> 1))
#define MINLONG32   ((LONG32)~MAXLONG32)

#define MAXULONG64  ((ULONG64)~((ULONG64)0))
#define MAXLONG64   ((LONG64)(MAXULONG64 >> 1))
#define MINLONG64   ((LONG64)~MAXLONG64)

#define MAXULONGLONG ((ULONGLONG)~((ULONGLONG)0))
#define MINLONGLONG ((LONGLONG)~MAXLONGLONG)

#define MAXSIZE_T   ((SIZE_T)~((SIZE_T)0))
#define MAXSSIZE_T  ((SSIZE_T)(MAXSIZE_T >> 1))
#define MINSSIZE_T  ((SSIZE_T)~MAXSSIZE_T)

#define MAXUINT     ((UINT)~((UINT)0))
#define MAXINT      ((INT)(MAXUINT >> 1))
#define MININT      ((INT)~MAXINT)

#define MAXDWORD32  ((DWORD32)~((DWORD32)0))
#define MAXDWORD64  ((DWORD64)~((DWORD64)0))

#endif // _WIN32_WINNT >= 0x0600

//
// Add Windows flavor DWORD_PTR types
//

//typedef ULONG_PTR DWORD_PTR, *PDWORD_PTR;

//
// The following types are guaranteed to be signed and 64 bits wide.
//

typedef int64_t LONG64, *PLONG64;


//
// The following types are guaranteed to be unsigned and 64 bits wide.
//

typedef uint64_t ULONG64, *PULONG64;
typedef uint64_t DWORD64, *PDWORD64;

//
// Legacy thread affinity.
//

typedef ULONG_PTR KAFFINITY;
typedef KAFFINITY *PKAFFINITY;

#ifdef __cplusplus
}
#endif
