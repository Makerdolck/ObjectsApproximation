////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Макросы стандартов C и C++.
         \en C\C++ standards. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_CPP_STANDARD_H
#define __SYSTEM_CPP_STANDARD_H

#include <cstddef>

// С11
#if (defined(_MSC_VER) && (_MSC_VER > 1600)) || (defined(__INTEL_C) ) || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))
#define STANDARD_C11
#endif

// С++11
#if defined(__cplusplus) && (__cplusplus >= 201103L) // С++11 (fully supported)
#define STANDARD_CPP11
#endif

// GCC version (https://sourceforge.net/p/predef/wiki/Compilers/)
#if !defined(_MSC_VER)  // _MSC_VER
  #if defined(__GNUC__)
    #if defined(__GNUC_MINOR__)
      #if defined(__GNUC_PATCHLEVEL__)
        #define C3D_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
      #else
        #define C3D_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
      #endif
    #else
      #define #define C3D_GCC_VERSION (__GNUC__ * 10000)
    #endif
  #else
    #define C3D_GCC_VERSION 0
  #endif
#else  // _MSC_VER
  #define C3D_GCC_VERSION 0
#endif  // _MSC_VER

// GNU glibc version.
#if !defined(_MSC_VER)
  #include <features.h>
  #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
    #define C3D_GLIBC_VERSION (__GLIBC__ * 1000 + __GLIBC_MINOR__)
  #else
    #define C3D_GLIBC_VERSION 0
  #endif
#else
  #define C3D_GLIBC_VERSION 0
#endif

#ifdef STANDARD_C11
#include<atomic>
#endif

#if ( defined(STANDARD_C11) || defined(STANDARD_CPP11) )
#define STANDARD_CPP11_RVALUE_REFERENCES
#endif


//------------------------------------------------------------------------------
// \ru Использование атомарных операций согласно стандарту C++11. \en Using atomic operations according to C++11 standard.
//---
#ifdef STANDARD_C11
typedef std::atomic_ptrdiff_t use_count_type; ///< \ru Потокобезопасный тип счётчика ссылок. \en Thread-safe references count type.

//------------------------------------------------------------------------------
/** \ru Получить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
Type LoadTypeValue( const AtomicType & v ) { 
  return v.load(); 
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType>
void StoreTypeValue( const AtomicType & src, AtomicType & dst ) { 
  dst.store( src.load() ); 
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
void StoreTypeValue( const Type src, AtomicType & dst ) { 
  dst.store( src ); 
}

#else
typedef ptrdiff_t use_count_type; ///< \ru Потоконебезопасный тип счётчика ссылок. \en Thread-unsafe references count type.

//------------------------------------------------------------------------------
/** \ru Получить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
Type LoadTypeValue( const AtomicType & v ) { 
  return v; 
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType>
void StoreTypeValue( const AtomicType & src, AtomicType & dst ) { 
  dst = src; 
}

//------------------------------------------------------------------------------
/** \ru Установить значение. \en Get value.
*/
//---
template <class AtomicType, class Type>
void StoreTypeValue( const Type src, AtomicType & dst ) { 
  dst = src; 
}

#endif


#endif // __SYSTEM_CPP_STANDARD_H
