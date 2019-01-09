////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Базовые макросы и функции.
         \en Base macros and functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef MATH_DEFINE_H
#define MATH_DEFINE_H


#include <system_types.h>

#ifdef _MSC_VER  // define PRECONDITION
  #include <crtdbg.h>
  #define PRECONDITION  _ASSERT ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
#else // _MSC_VER
  #include <assert.h>
  #define _ASSERT assert
  #ifndef PRECONDITION
    #ifdef C3D_DEBUG
      #include <iostream>
      inline void __precondition( bool expr, const char* file, int line, const char* func, const char* expression ) {
          if ( !expr ) fprintf(stderr, "In file %s, line %d:\nfailed PRECONDITION `%s' in function: %s.\n", file, line, expression, func);
      }
      #define PRECONDITION(expr) __precondition(expr, __FILE__, __LINE__, __PRETTY_FUNCTION__, #expr)  ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
    #else
      #define PRECONDITION(expr) ((void)0)  ///< \ru Определение PRECONDITION. \en The PRECONDITION definition.   \~ \ingroup Base_Tools
    #endif  // C3D_DEBUG
  #endif  // PRECONDITION
#endif // _MSC_VER

#include <tool_memory_debug.h> // \ru Внимание! Читаем внимательно! Если вытереть отсюда, то подключить везде, где есть функции из этого файла! \en Attention! Read carefully! If remove this from here, then it should be included anywhere where the functions from this file exist! 
#include <utility>

namespace c3d // namespace C3D
{
typedef std::pair<size_t, size_t>  NumberPair;   ///< \ru Пара целочисленных неотрицательных индексов. \en Pair of non-negative integer indices. 
typedef std::pair<uint, uint>      UintPair;     ///< \ru Пара 32-битных целочисленных неотрицательных индексов. \en Pair of 32-bit non-negative integer indices. 
typedef std::pair<double, double>  DoublePair;   ///< \ru Пара действительных чисел двойной точности с плавающей запятой. \en Pair of doubles. 
typedef std::pair<size_t, double>  NumberDouble; ///< \ru Пара номер-число. \en Index-double pair. 
typedef std::pair<double, size_t>  DoubleNumber; ///< \ru Пара число-номер. \en Double-index pair. 
typedef std::pair<bool, double>    FlagDouble;   ///< \ru Пара флаг-число. \en Flag-double pair. 
typedef std::pair<double, bool>    DoubleFlag;   ///< \ru Пара число-флаг. \en Double-flag pair.
}


#ifdef _MSC_VER
//------------------------------------------------------------------------------
/** \brief \ru Определение CALL_DECLARATION.
           \en The CALL_DECLARATION definition. \~
  \details \ru Определение CALL_DECLARATION. \n
           \en The CALL_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define CALL_DECLARATION    __cdecl
//------------------------------------------------------------------------------
/** \brief \ru Определение EXPORT_DECLARATION.
           \en The EXPORT_DECLARATION definition. \~
  \details \ru Определение EXPORT_DECLARATION. \n
           \en The EXPORT_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define EXPORT_DECLARATION  __declspec(dllexport)
#else // _MSC_VER
//------------------------------------------------------------------------------
/** \brief \ru Определение CALL_DECLARATION.
           \en The CALL_DECLARATION definition. \~
  \details \ru Определение CALL_DECLARATION. \n
           \en The CALL_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define CALL_DECLARATION
//------------------------------------------------------------------------------
/** \brief \ru Определение EXPORT_DECLARATION.
           \en The EXPORT_DECLARATION definition. \~
  \details \ru Определение EXPORT_DECLARATION. \n
           \en The EXPORT_DECLARATION definition. \n \~
  \ingroup Base_Tools
*/
// ---
#define EXPORT_DECLARATION

#endif // _MSC_VER

//#ifdef _MSC_VER // std_min() / std_max()
//
//#define std_max(a,b)      (std::max)(a,b)
//#define std_min(a,b)      (std::min)(a,b)
//#define std_maxRef(a,b)   (std::max)(a,b)
//#define std_minRef(a,b)   (std::min)(a,b)
//
//#ifndef NOMINMAX
//#include <algorithm>
//#endif // NOMINMAX
//
//#else // _MSC_VER

#include <algorithm>

#define std_max(a,b)      (std::max)(a,b)
#define std_min(a,b)      (std::min)(a,b)
#define std_maxRef(a,b)   (std::max)(a,b)
#define std_minRef(a,b)   (std::min)(a,b)

//#endif  // _MSC_VER

//------------------------------------------------------------------------------
// \ru Для совместимости с VC < 2005 \en To be compatible with VC < 2005 
//---
#if defined(_MSC_VER) && (_MSC_VER < 1400)
  #define TEMPLATE_TYPENAME
#else
  #define TEMPLATE_TYPENAME template<typename Type>
#endif // _MSC_VER

//------------------------------------------------------------------------------
// \ru Синтаксис дружественной шаблонной функции шаблона \en Syntax of friendly template function of a template 
#ifndef _MSC_VER 

  #define TEMPLATE_FRIEND friend // \ru по стандарту C++98 \en by the C++98 standard 
  #define TEMPLATE_SUFFIX <Type>
  #define TEMPLATE_SUFFIX2 <T1, T2>
  #define FORVARD_DECL_TEMPLATE_TYPENAME( _FUNC ) template<typename Type> _FUNC
  #define FORVARD_DECL_TEMPLATE_TYPENAME2( _FUNC ) template<typename T1, typename T2> _FUNC
  
#else // _MSC_VER

  #define TEMPLATE_FRIEND template<typename Type> friend // \ru для VS2005 \en for VS2005 
  #define TEMPLATE_SUFFIX
  #define TEMPLATE_SUFFIX2 
  #define FORVARD_DECL_TEMPLATE_TYPENAME( _FUNC ) 
  #define FORVARD_DECL_TEMPLATE_TYPENAME2( _FUNC ) 
  
#endif // _MSC_VER


//------------------------------------------------------------------------------
/** \brief \ru Объявление оператора присваивания и конструктора копирования.
           \en The declaration of assignment operator and copy constructor. \~
  \details \ru Объявление приватных оператора присваивания и конструктора копирования.
    Используется для запрета неявной реализации этой функциональности, т.к.
    при отсутствии в классах явного дублирующего конструктора и оператора присваивания
    автоматически генерируются неявные - в основном, через копирование памяти,
    что может привести к некорректному поведению системы.
           \en The declaration of private assignment operator and copy constructor.
    This is used to prohibit an implicit implementation of this functionality because
    if there is no an explicit copy constructor or an assignment operator
    implicit copy constructor and assigment operator are generated automatically mostly by copying of memory,
    that can lead to incorrect behaviour of the system. \~
  \ingroup Base_Tools
*/
//---
#define OBVIOUS_PRIVATE_COPY( ClassName )       \
private:                                        \
  ClassName & operator = ( const ClassName & ); \
  ClassName( const ClassName & );


//------------------------------------------------------------------------------
/// \ru Получить количество элементов массива. \en Get the number of array elements. \~ \ingroup Base_Tools
//---
#define COUNTOF(array) (sizeof(array)/sizeof(array[0]))

//------------------------------------------------------------------------------
/// \ru Максимальное количество элементов матрицы MxN. \en Maximum number of MxN matrix elements. 
//---
#define MATRIX_MAX_COUNT  1000000000

//------------------------------------------------------------------------------
/// \ru Максимальный размер массива. \en Maximum size of array. 
//---
#define ARRAY_MAX_COUNT   1000000


//------------------------------------------------------------------------------
// \ru Объявить тест кейс как дружественный по отношению к классу, для \en Declare the test case as friendly to the class in order 
// \ru того чтобы была возможность доступа к закрытым членам этого класса. \en to be able to access to the private members of the class. 
//---
#define DECLARE_FRIEND_TEST_CASE( SuiteName, CaseName ) friend class SuiteName ## _ ## CaseName ## _Test


//------------------------------------------------------------------------------
// \ru Макросы __TODO__ и __WARN__ предназначены для \en Macros __ TODO__ and __ WARN__ are designed for  
// \ru генерации сообщений совместно с #pragma message \en generation of messages with #pragma message 
// \ru и позволяют в среде MsDev переходить на строку \en and allow to move to the next line in the MsDev environment  
// \ru кода с сообщением по двойному клику в окне Output \en with the message by double click in the Output window  
//
// \ru примеры: \en examples: 
// \ru #pragma message( __TODO__ "Восстановить закрытый код" ) \en #pragma message( __TODO__ "Restore the private code" ) 
// \ru #pragma message( __WARN__ "Отсутствует проверка на NULL" ) \en #pragma message( __WARN__ "There is no check for NULL" ) 
//---
#ifdef _MSC_VER // __TODO__ / __WARN__

#define __ANYTOSTR__(x) #x
#define __DEFTOSTR__(x) __ANYTOSTR__(x)
#define __TODO__ __FILE__ "("__DEFTOSTR__(__LINE__)") : TODO: "
#define __WARN__ __FILE__ "("__DEFTOSTR__(__LINE__)") : warning: "

#else // _MSC_VER 

#define __TODO__ 
#define __WARN__ 

#endif // _MSC_VER 


//------------------------------------------------------------------------------
/** \brief \ru Объявление экспортности или импортности классов.
           \en The declaration of export or import classes. \~
  \details \ru Объявление экспортности в данном модуле или импортности в других подключаемых модулях. \n
           \en The declaration of export in this module or import in other plugged modules. \n \~
  \ingroup Base_Tools
*/
// ---
// \ru Модуль геометрического моделирования. \en Geometric modeling module. 
#ifdef _MSC_VER
#if defined ( _BUILDMATHDLL )
  #define MATH_CLASS         __declspec( dllexport )
  #define MATH_FUNC(retType) __declspec( dllexport ) retType CALL_DECLARATION
  #define MATH_FUNC_EX       __declspec( dllexport ) // \ru для KNOWN_OBJECTS_RW_REF_OPERATORS_EX и KNOWN_OBJECTS_RW_PTR_OPERATORS_EX \en for KNOWN_OBJECTS_RW_REF_OPERATORS_EX and KNOWN_OBJECTS_RW_PTR_OPERATORS_EX 
#else
  #define MATH_CLASS         __declspec( dllimport )
  #define MATH_FUNC(retType) __declspec( dllimport ) retType CALL_DECLARATION
  #define MATH_FUNC_EX       __declspec( dllimport ) 
#endif
#else // _MSC_VER
  #define MATH_CLASS
  #define MATH_FUNC(retType) retType
  #define MATH_FUNC_EX
#endif

// \ru Модуль геометрических ограничений. \en Geometric constraints module. 
#define   GCE_CLASS    MATH_CLASS
#define   GCM_CLASS    MATH_CLASS
#define   GCE_FUNC     MATH_FUNC
#define   GCM_FUNC     MATH_FUNC

// \ru Модуль конвертеров. \en Converters module. 
#define   CONV_CLASS   MATH_CLASS
#define   CONV_FUNC    MATH_FUNC
#define   CONV_FUNC_EX MATH_FUNC_EX

// \ru Поддержка кода. \en Support of the code. 
#ifndef NULL
#define NULL 0
#endif


//-------------------------------------------------------------------------------
/** \brief \ru Вычисление косинуса и синуса.
           \en Calculation of the cosine and sine. \~
  \details \ru В 1.7 раза быстрее, чем отдельное вычисление sin и cos, 
    проверено под release с оптимизацией.
           \en 1.7 times faster than a single calculation of sin and cos, 
    Tested in release configuration with optimization. \~
  \param[in] tt - \ru Угол (в радианах).
                  \en Angle (in radians). \~
  \param[out] cosT - \ru Косинус угла tt.
                     \en Cosine of the angle tt. \~
  \param[out] sinT - \ru Синус угла tt.
                     \en Sine of the angle tt. \~
  \ingroup Base_Algorithms
*/
// ---
inline void GetCosSin( const double & tt, double & cosT, double & sinT )
{
// 2013-08-06 Голованов Н. Пробуем единый подход для PLATFORM_64 / !PLATFORM_64
//#if !defined(PLATFORM_64) && defined(_MSC_VER)  // Use inline _asm
//  __asm {
//    mov     eax, tt
//    mov     ebx, cosT
//    mov     ecx, sinT
//    FLD     qword ptr [eax]
//    FSINCOS
//    FSTP    qword ptr [ebx]
//    FSTP    qword ptr [ecx]
//  }
//#else // PLATFORM_64
  cosT = ::cos( tt );
  sinT = ::sin( tt );
//#endif // PLATFORM_64
}

//-------------------------------------------------------------------------------
// \ru Погасить отладочное требование. \en . Mute debug assert.
// ---
inline void DummyAssert( bool ) {
}

//-------------------------------------------------------------------------------
// \ru Работа с OpenMP. // \en Work with OpenMP.
// ---
#ifndef _OPENMP
//-------------------------------------------------------------------------------
// \ru Заглушки для OpenMP. \en Stubs for OpenMP.
// ---
#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus
  typedef struct{ void * _lk; } omp_lock_t;
  typedef struct{ void * _lk; } omp_nest_lock_t;
  inline void   CALL_DECLARATION omp_set_num_threads   ( int )              {}
  inline int    CALL_DECLARATION omp_get_num_threads   ( void )             { return 1; }
  inline int    CALL_DECLARATION omp_get_max_threads   ( void )             { return 1; }
  inline int    CALL_DECLARATION omp_get_thread_num    ( void )             { return 0; }
  inline int    CALL_DECLARATION omp_get_num_procs     ( void )             { return 1; }
  inline void   CALL_DECLARATION omp_set_dynamic       ( int )              {}
  inline int    CALL_DECLARATION omp_get_dynamic       ( void )             { return 0; }
  inline int    CALL_DECLARATION omp_in_parallel       ( void )             { return 0; }
  inline void   CALL_DECLARATION omp_set_nested        ( int )              {}
  inline int    CALL_DECLARATION omp_get_nested        ( void )             { return 0; }
  inline void   CALL_DECLARATION omp_init_lock         ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_destroy_lock      ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_set_lock          ( omp_lock_t* )      {}
  inline void   CALL_DECLARATION omp_unset_lock        ( omp_lock_t* )      {}
  inline int    CALL_DECLARATION omp_test_lock         ( omp_lock_t* )      { return 1; }
  inline void   CALL_DECLARATION omp_init_nest_lock    ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_destroy_nest_lock ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_set_nest_lock     ( omp_nest_lock_t* ) {}
  inline void   CALL_DECLARATION omp_unset_nest_lock   ( omp_nest_lock_t* ) {}
  inline int    CALL_DECLARATION omp_test_nest_lock    ( omp_nest_lock_t* ) { return 1; }
  inline double CALL_DECLARATION omp_get_wtime         ( void )             { return 1.0; }
  inline double CALL_DECLARATION omp_get_wtick         ( void )             { return 1.0; }
#if defined(__cplusplus)
}
#endif  // __cplusplus
#else  // _OPENMP
  #include <omp.h>
#endif // _OPENMP

#endif // MATH_DEFINE_H
