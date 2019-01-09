////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Глобальные объявления типов и определения.
         \en Global type declarations and defines. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_GLOBAL_H
#define __VSN_GLOBAL_H

#if defined(M_E) && !defined(_MATH_DEFINES_DEFINED)
#define _MATH_DEFINES_DEFINED
#endif

#include <math_define.h>


#define VSN_NAMESPACE C3DVision

#if !defined(VSN_NAMESPACE)
#  define VSN_BEGIN_NAMESPACE
#  define VSN_END_NAMESPACE
#  define VSN_USE_NAMESPACE
#  define VSN_PREPEND_NAMESPACE_VISION(name) ::name
#else
#  define VSN_BEGIN_NAMESPACE namespace VSN_NAMESPACE {
#  define VSN_END_NAMESPACE }
#  define VSN_USE_NAMESPACE using namespace VSN_NAMESPACE;
#  define VSN_PREPEND_NAMESPACE_VISION(name) ::VSN_NAMESPACE::name
#endif

VSN_BEGIN_NAMESPACE

#ifdef _MSC_VER
#if defined ( _BUILDVISIONDLL )
#define VSN_CLASS         __declspec( dllexport )
#define VSN_FUNC(retType) __declspec( dllexport ) retType CALL_DECLARATION
#define VSN_FUNC_EX       __declspec( dllexport ) // \ru для KNOWN_OBJECTS_RW_REF_OPERATORS_EX и KNOWN_OBJECTS_RW_PTR_OPERATORS_EX \en for KNOWN_OBJECTS_RW_REF_OPERATORS_EX and KNOWN_OBJECTS_RW_PTR_OPERATORS_EX 
#else
#define VSN_CLASS         __declspec( dllimport )
#define VSN_FUNC(retType) __declspec( dllimport ) retType CALL_DECLARATION
#define VSN_FUNC_EX       __declspec( dllimport ) 
#endif
#else // _MSC_VER
#define VSN_CLASS
#define VSN_FUNC(retType) retType
#define VSN_FUNC_EX
#endif


/* Define V_NULL */
#ifndef V_NULL
#define V_NULL 0
#endif

#define VSN_MAXINT INT_MAX
#define VSN_DELETE_AND_NULL(x) delete x; x = V_NULL;

#define VSN_DECLARE_PRIVATE(Class) friend class Class##Private; \
Class##Private* vsn_d_ptr; \
Class##Private& vsn_d() { return *vsn_d_ptr; } \
const Class##Private& vsn_d() const { return *vsn_d_ptr; }

#define VSN_DECLARE_PUBLIC(Class) \
friend class Class; \
Class* vsn_p_ptr; \
inline void setPublic(Class* ptr) { vsn_p_ptr = ptr; } \
Class& vsn_p() { return *vsn_p_ptr; } \
const Class& vsn_p() const { return *vsn_p_ptr; }
#define VSN_INIT_PRIVATE(Class) \
vsn_d_ptr = new Class##Private(); vsn_d_ptr->setPublic(this);
#define VSN_D(Class) Class##Private& d = vsn_d();
#define VSN_P(Class) Class& p = vsn_p();
#define VSN_FINI_PRIVATE() \
delete vsn_d_ptr; vsn_d_ptr = V_NULL;

#define VSN_DECLARE_EX_PRIVATE(Class) friend class Class##Private; \
inline Class##Private& vsn_d() { return reinterpret_cast<Class##Private &>(*vsn_d_ptr); } \
inline const Class##Private& vsn_d() const { return reinterpret_cast<const Class##Private &>(*vsn_d_ptr); }

#define VSN_DECLARE_EX_PUBLIC(Class) \
friend class Class; \
inline Class& vsn_p() { return static_cast<Class &>(*vsn_p_ptr); } \
inline const Class& vsn_p() const { return static_cast<Class &>(*vsn_p_ptr); }

#define VSN_2EXD(Class) Class##Private* d = (Class##Private*)vsn_d();

#define VSN_DECLARE_2EX_PRIVATE(Class) friend class Class##Private; \
inline Class##Private* vsn_d() { return (Class##Private*)(vsn_d_ptr); } \
inline const Class##Private* vsn_d() const { return (const Class##Private*)(vsn_d_ptr); }


typedef void(*FunctionPtr)();
typedef void(__stdcall *CallBackFunc)();

// A void "unused parameter" warnings
#define VSN_UNUSED(parameter) (void)parameter;

template <typename T>
inline const T& vMin(const T& a, const T& b) { return (a < b) ? a : b; }
template <typename T>
inline const T& vMax(const T& a, const T& b) { return (a < b) ? b : a; }

VSN_END_NAMESPACE

VSN_USE_NAMESPACE


#endif //__VSN_GLOBAL_H

