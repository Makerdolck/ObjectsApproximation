////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Контроль выделения памяти под отладкой.
         \en Memory allocation control during the debugging process. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MEMORY_DEBUG_H
#define __MEMORY_DEBUG_H

#include <math_x.h>
#include <memory.h>
#include <system_types.h>

#ifdef C3D_DEBUG
  #if defined(__APPLE__) && defined(__MACH__) // mac
  #include <malloc/malloc.h>
  #elif defined(__FreeBSD__)
  #include <stdlib.h>
  #else
  #include <malloc.h>
  #endif // mac

  #ifndef __DISABLE_MEMORY_CONTROL__  // \ru ЯТ - чтобы можно было отключать в других проектах \en ЯТ - to allow to disable it in other projects 
  #define USE_REALLOC_IN_ARRAYS // true
  #endif // __DISABLE_MEMORY_CONTROL__
#endif // C3D_DEBUG

#ifdef __FILL_FREE_HEAP_HEAR__
#include <filhpsw.h>
#endif
#ifdef __REALLOC_ARRAYS_STATISTIC_
#include <stdio.h>
#endif


#ifdef __FILL_FREE_HEAP_HEAR__

//------------------------------------------------------------------------------
///
//---
unsigned char gl_fillHeapChar = 0;

//------------------------------------------------------------------------------
///
// ---
inline void SetFillHeap( unsigned char mask ) {
  gl_fillHeapChar = mask;
}

//------------------------------------------------------------------------------
/// \ru Проверить правильность указателя (работает очень медленно). \en Check pointer (it works very slow).  \~ \ingroup Base_Tools
// ---
inline void CheckPointer( void * ptr, bool alreadyFreed )
{
  if ( !alreadyFreed ) {
    size_t size = ptr ? ::_msize(ptr) : 0;
    if ( size ) {
      unsigned char * begMem = (unsigned char * )ptr;
      unsigned char * mem = begMem;
      unsigned char * endMem = begMem + size;

      uint criticalFreeCount = std_min( size, (uint)4 );

      uint freeCount = 0;
      do {
        if ( *mem == 0xDD ) freeCount++;
        else                freeCount = 0;
        mem++;
      }
      while ( mem < endMem && freeCount <= criticalFreeCount );

      if ( freeCount >= criticalFreeCount )
        ::MessageTrace( _T("Repeated free") );
    }
  }
}

//------------------------------------------------------------------------------
/// \ru Заполнить все свободные блоки константным значением (работает очень медленно). \en Fill all free block by constant values (it works very slow)  \~ \ingroup Base_Tools
// ---
inline void FillFreeHeap( bool checkFree ) {
  if ( gl_fillHeapChar != 0 ) {

    if ( checkFree ) {
      int heapCheckResult = heapcheckfree( gl_fillHeapChar );

      switch ( heapCheckResult ) {
        case _BADVALUE  :
          ::MessageTrace( _T("FillFree - A value other than the fill value was found") );
          break;
        case _HEAPCORRUPT :
          ::MessageTrace( _T("FillFree - Heap has been corrupted") );
          break;
        case _HEAPEMPTY :
          ::MessageTrace( _T("FillFree - No heap") );
          break;
          // case _HEAPOK Heap is accurate
      }
    }

    if ( !checkFree ) {
      int heapCheckResult = heapfillfree( gl_fillHeapChar );
      switch ( heapCheckResult ) {
        case _HEAPCORRUPT :
          ::MessageTrace( _T("FillFree - Heap has been corrupted") );
          break;
        case _HEAPEMPTY :
          ::MessageTrace( _T("FillFree - No heap") );
          break;
          //  case _HEAPOK  Heap is accurate
      }
    }
  }
}

#define __CHECK_POINTER(p,a)     ::CheckPointer( p, a );
#define __FILL_FREE_HEAP__       ::FillFreeHeap(false);
#define __CHECK_FREE_HEAP__      ::FillFreeHeap(true);

#else // __FILL_FREE_HEAP_HEAR__

#define __CHECK_POINTER(p,a)  // ::CheckPointer( p, a );
#define __FILL_FREE_HEAP__    // ::FillFreeHeap(true);
#define __CHECK_FREE_HEAP__   // ::FillFreeHeap(true);

#endif // __FILL_FREE_HEAP_HEAR__


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_

//------------------------------------------------------------------------------
/// \ru Заполнить все свободные блоки константным значением (работает очень медленно). \en Fill all free block by constant values (it works very slow)  \~ \ingroup Base_Tools
// ---
inline void FillFreeHeap() {
#ifdef __FILL_FREE_HEAP_HEAR__
  ::FillFreeHeap( false/*checkFree*/ );
#endif // __FILL_FREE_HEAP_HEAR__
}

//------------------------------------------------------------------------------
/// \ru Проверить указатель и значение. \en Check pointer and value.  \~ \ingroup Base_Tools
// ---
inline void CheckPointerAndValue( void * ptr, size_t size )
{
  PRECONDITION( ptr != (void *)0xEEEEEEEE );
  PRECONDITION( ptr != (void *)0xFFFFFFFF );
  PRECONDITION( ptr != (void *)0xAAAAAAAA );

  if ( ptr ) {
    // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
    if ( size > 11 ) {
      uint32* _ptr = (uint32 *)ptr;
      uint32 value1 = *_ptr++;
      uint32 value2 = *_ptr++;
      uint32 value3 = *_ptr;
      PRECONDITION( value1 != 0xEEEEEEEE || value2 != 0xEEEEEEEE || value3 != 0xEEEEEEEE );
      PRECONDITION( value1 != 0xFFFFFFFF || value2 != 0xFFFFFFFF || value3 != 0xFFFFFFFF );
      PRECONDITION( value1 != 0xAAAAAAAA || value2 != 0xAAAAAAAA || value3 != 0xAAAAAAAA );
    }
    else if ( size > 7 ) {
      uint32 * _ptr = (uint32 *)ptr;
      uint32 value1 = *_ptr++;
      uint32 value2 = *_ptr;
      PRECONDITION( value1 != 0xEEEEEEEE && value2 != 0xEEEEEEEE );
      PRECONDITION( value1 != 0xFFFFFFFF || value2 != 0xFFFFFFFF );
      // PRECONDITION( value1 != 0xAAAAAAAA && value2 != 0xAAAAAAAA );
    }
    else if ( size > 3 ) {
      uint32 value = *(uint32 *)ptr;
      PRECONDITION( value != 0xEEEEEEEE );
      PRECONDITION( value != 0xFFFFFFFF );
      // PRECONDITION( value != 0xAAAAAAAA );
    }
    else if ( size > 1 ) {
      uint16 value = *(uint16 *)ptr;
      PRECONDITION( value != 0xEEEE );
      PRECONDITION( value != 0xFFFF );
      // PRECONDITION( value != 0xAAAA );
    }
  }
}

//------------------------------------------------------------------------------
/// \ru Выделить память указанного размера. \en Allocate memory of the given size.  \~ \ingroup Base_Tools
// ---
inline void * Allocate( size_t size, const char * ) // className )
{
  void * ptr = ::malloc( size );
  // \ru ИР дабы работал _msize: void *ptr = ::operator new( size ); \en ИР for working of _msize: void *ptr = ::operator new( size ); 
  if ( ptr ) {
    ::memset( ptr, 0xFF, size );
    //INSERT_CLEANUP_REGISTER( className );
  }
  return ptr;
}

//------------------------------------------------------------------------------
/// \ru Выделить память указанного размера под массив. \en Allocate memory of the given size for an array.  \~ \ingroup Base_Tools
// ---
inline void * AllocateArray( size_t size, const char * ) // className )
{
  void * ptr = ::malloc( size );
  // \ru ИР дабы работал _msize: void *ptr = ::operator new [] ( size ); \en ИР for working of _msize: void *ptr = ::operator new [] ( size ); 
  if ( ptr ) {
#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    ::memset( ptr, 0xffffffff, size );  // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
#endif // __MEMSET_USED_FREE_HEAP_HEAR__
    //INSERT_CLEANUP_REGISTER( className );
  }
  return ptr;
}

//------------------------------------------------------------------------------
/// \ru Освободить память указанного размера. \en Free memory of the given size.  \~ \ingroup Base_Tools
// ---
inline void Free( void * ptr, size_t size, const char * ) // className )
{
  if ( ptr ) {
    //REMOVE_CLEANUP_REGISTER( className );

    ::CheckPointerAndValue( ptr, size );

    __CHECK_POINTER( ptr, false/*alreadyFreed*/ ); // \ru проверить указатель \en check pointer 
    __CHECK_FREE_HEAP__

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    size_t ptr_size = ::_msize( ptr );
    PRECONDITION( ptr_size > 0 && ptr_size < 0xFFFFFFFF ); // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
    if ( ptr_size ) {
      PRECONDITION( size <= ptr_size );
      ::memset( ptr, 0xEE, ptr_size );
    }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

    ::free( ptr );
    // \ru ИР дабы работал _msize: ::operator delete( ptr ); \en ИР for working of _msize: ::operator delete( ptr ); 
    // \ru ЯТ вот это может быть и нельзя делать после delete, но если оставить \en ЯТ it is probably should not be done after the 'dele' operator, but if 
    // \ru только первое заполнение, то начальные 4(?) слова освобождаемой памяти \en only the first value is left then starting 4(?) words of released memory 
    // \ru не будут иметь значение 0xDDDDDDDD \en will not have the value 0xDDDDDDDD 
    //::memset( ptr, 0xEE, size );

    // \ru __CHECK_POINTER( ptr, true/*alreadyFreed*/ ); // проверить указатель \en __CHECK_POINTER( ptr, true/*alreadyFreed*/ ); // check pointer 
    __FILL_FREE_HEAP__ // \ru заполнить все свободные блоки \en fill all free blocks. 
  }

  //ptr = (void*)0xDDDDDDDD;
}

//------------------------------------------------------------------------------
/// \ru Освободить память, выделенную под массив. \en Free the memory allocated for the array.  \~ \ingroup Base_Tools
// ---
// \ru ЯТ можно перегрузить в классах operator delete [] ( void *, size_t ) и передать \en ЯТ it is pertinent to overload the operator delete [] ( void *, size_t ) and pass 
// \ru в эту функцию size_t size, но это будет не размер массива, а размер Type, \en to this function size_t size, but this will be not the size of an array but the size of 'Type', 
// \ru массив которых распределялся. То есть эта информация здесь не нужна (делать \en an array of which was not distribute. I.e. this information is not needed here ( 
// \ru ::memset не нее НЕЛЬЗЯ!) \en it is forbidden to do ::memset here) 
// ---
inline void FreeArray( void * ptr, const char * ) // className )
{
  if ( ptr ) {
    //REMOVE_CLEANUP_REGISTER( className );

    ::CheckPointerAndValue( ptr, 0/*size*/ );

    __CHECK_POINTER( ptr, false/*alreadyFreed*/ ); // \ru проверить указатель \en check pointer 
    __CHECK_FREE_HEAP__

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
    size_t size = ::_msize( ptr );
    PRECONDITION( size > 0 && size < 0xFFFFFFFF );  // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
    ::CheckPointerAndValue( ptr, size );
    if ( size )
      ::memset( ptr, 0xEE, size );
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

    ::free( ptr );

    // \ru ИР дабы работал _msize: ::operator delete [] ( ptr ); \en ИР for working of _msize: ::operator delete [] ( ptr ); 
    // \ru __CHECK_POINTER( ptr, true/*alreadyFreed*/ ); // проверить указатель \en __CHECK_POINTER( ptr, true/*alreadyFreed*/ ); // check pointer 
    __FILL_FREE_HEAP__ // \ru заполнить все свободные блоки \en fill all free blocks 
  }

  //ptr = (void*)0xDDDDDDDD;
}


#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


#ifdef __REALLOC_ARRAYS_STATISTIC_

struct OneArrayTypeStatistic {
public:
  size_t reallocCountSuccess; // \ru кол-во успешных перераспределений памяти \en A number of successfully reallocations of the memory 
  size_t firstAlloc         ; // \ru кол-во первичных распределений \en the number of primary allocations 
  size_t lastAlloc          ; // \ru кол-во освобождений \en the number of releases 
  size_t realAllocIncremet  ; // \ru кол-во запросов на увеличение \en the number of requests for an increment 
  size_t realAllocDecremet  ; // \ru кол-во запросов на уменьшение \en the number of requests for a decrement 
  size_t fullLength         ; // \ru суммарный размер в байтах \en the total size in bites 
  size_t maxOneArrayLength  ; // \ru максимальный размер одного массива \en the minimal size of one array 
  size_t sumCurrLength      ; // \ru суммарный размер памяти во всех массивах этого типа в данный момент \en the total size of the memory in all arrays of this type at this moment 
  size_t maxOneTimeLength   ; // \ru максимальный одновременный размер памяти во всех массивах этого типа \en the maximum simultaneous size of the memory in all arrays of this type 
  size_t minDelta           ; // \ru минимальное приращение \en minimum increment 
  size_t maxDelta           ; // \ru максимальное приращение \en maximum increment 
public:
  OneArrayTypeStatistic() { Clear(); }
  void Clear() {
    reallocCountSuccess = 0;
    firstAlloc          = 0;
    lastAlloc           = 0;
    realAllocIncremet   = 0;
    realAllocDecremet   = 0;
    fullLength          = 0;
    maxOneArrayLength   = 0;
    sumCurrLength       = 0;
    maxOneTimeLength    = 0;
    minDelta            = SYS_MAX_T; // \ru минимальное приращение \en minimum increment 
    maxDelta            = 0;         // \ru максимальное приращение \en maximum increment 
  }
};

const size_t STAT_ARRAY_COUNT = 6
static OneArrayTypeStatistic statisticArray[STAT_ARRAY_COUNT]
  = { OneArrayTypeStatistic(), OneArrayTypeStatistic(), OneArrayTypeStatistic(),
      OneArrayTypeStatistic(), OneArrayTypeStatistic(), OneArrayTypeStatistic() };

static size_t allReallocCount = 0;

//------------------------------------------------------------------------------
// \ru Уменьшение uint с проверкой \en A decrement of uint with the check 
// ---
inline void DecrementUint( uint & val, uint delta ) {
  val = (delta < val) ? (val - delta) : 0;
}

//------------------------------------------------------------------------------
/// \ru Статистика изменений размера массива. \en Statistics of array size changes.  \~ \ingroup Base_Tools
// arrayType : 
// \ru 0 - SArray (или наследники), \en 0 -SAray (or inheritors) 
// \ru 1 - RParray (или наследники), \en 1 -RPAray (or inheritors) 
// 2 - Array2,
// 3 - LiSArray,
// 4 - CcArray,
// \ru 5 - неопознанные (вообще-то, такого не должно быть) \en 5 - not defined (it should not happen) 
// ---
inline void ReallocArrayStatistic( void * oldParr, size_t oldSize, 
                                   void * newParr, size_t newSize, 
                                   uint arrayType ) 
{
  if ( oldParr || newSize ) {
    allReallocCount++;

    // \ru найдем статистическую запись про данный тип массива \en find a statistic record about the given type of array 
    size_t index = (size_t)arrayType;
    if ( index >= STAT_ARRAY_COUNT )
      index = STAT_ARRAY_COUNT - 1;
    OneArrayTypeStatistic & stat = statisticArray[index];

    if ( newSize > stat.maxOneArrayLength )
      stat.maxOneArrayLength = newSize;

    if ( !oldParr ) {                // \ru первичное распределение \en primary allocation 
      stat.firstAlloc++;
      stat.fullLength    += newSize; // \ru общее кол-во байт в этом типе массива \en the total number of bites in this type of an array 
      stat.sumCurrLength += newSize; // \ru суммарный размер памяти во всех массивах этого типа в данный момент \en the total size of the memory in all arrays of this type at this moment 
    }

    if ( oldParr && !newSize ) {     // \ru полное освобождение \en full release 
      stat.lastAlloc++;
      // \ru максимальный одновременный размер памяти во всех массивах этого типа \en the maximum simultaneous size of the memory in all arrays of this type 
      stat.maxOneTimeLength = std_max( stat.maxOneTimeLength, stat.sumCurrLength );
      // \ru суммарный размер памяти во всех массивах этого типа в данный момент \en the total size of the memory in all arrays of this type at this moment 
      ::DecrementUint( stat.sumCurrLength, oldSize );
    }

    if ( oldParr && newSize > oldSize ) { // \ru запрос на увеличение \en a request to increase 
      stat.realAllocIncremet++;
      size_t delta = newSize - oldSize;
      stat.fullLength    += delta; // \ru общее кол-во байт в этом типе массива \en the total number of bites in this type of an array 
      stat.sumCurrLength += delta; // \ru суммарный размер памяти во всех массивах этого типа в данный момент \en the total size of the memory in all arrays of this type at this moment 
      stat.minDelta = std_min( stat.minDelta, delta ); // \ru минимальное приращение \en minimum increment 
      stat.maxDelta = std_max( stat.maxDelta, delta ); // \ru максимальное приращение \en maximum increment 
    }

    if ( oldParr && newSize && newSize < oldSize ) { // \ru запрос на уменьшение \en a request to decrease 
      stat.realAllocDecremet++;
      // \ru максимальный одновременный размер памяти во всех массивах этого типа \en the maximum simultaneous size of the memory in all arrays of this type 
      stat.maxOneTimeLength = std_max( stat.maxOneTimeLength, stat.sumCurrLength );
      // \ru суммарный размер памяти во всех массивах этого типа в данный момент \en the total size of the memory in all arrays of this type at this moment 
      ::DecrementUint( stat.sumCurrLength, oldSize - newSize );
    }

#ifdef USE_REALLOC_IN_ARRAYS
    if ( oldParr && oldParr == newParr && newSize != oldSize )
      stat.reallocCountSuccess++;
#endif // USE_REALLOC_IN_ARRAYS
  }
}


//------------------------------------------------------------------------------
/// \ru Отчет по статистике изменений размера массива. \en A report by the statistics of array size changes.  \~ \ingroup Base_Tools
// ---
inline void ReallocReport( bool clear, const char * title = NULL )
{
  c3d::string_t text( title );

  if ( title.length() > 0 )
    text += _T("\n");

  char buf[200];
  _sntprintf( buf, 200, _T("Общее кол-во realloc " F_TD), allReallocCount );
  text += buf;

  for ( size_t i = 0; i < STAT_ARRAY_COUNT; i++ ) {
    OneArrayTypeStatistic &stat = statisticArray[i];

    // \ru если данных нет, то и писать про них не будем \en if there is no data then we will not write about it 
    if ( stat.firstAlloc || stat.lastAlloc || stat.realAllocIncremet || stat.realAllocDecremet ) {

      switch ( i ) {
        case 0 : text += _T("\n\nSArray  (или наследники)"); break;
        case 1 : text += _T("\n\nRParray (или наследники)"); break;
        case 2 : text += _T("\n\nArray2"); break;
        case 3 : text += _T("\n\nLiSArray  (или наследники)"); break;
        case 4 : text += _T("\n\nCcArray"); break;
        case 5 : text += _T("\n\nнеопознанные"); break;
      }

      _sntprintf( buf, 200, _T("\nвсего байт \t%d"), stat.fullLength );
      _sntprintf( buf, 200, _T("\nвсего байт \t" F_TD), stat.fullLength );
      text += buf;

      _sntprintf( buf, 200, _T("\nмакс.размер \t" F_TD), stat.maxOneArrayLength );
      text += buf;

      _sntprintf( buf, 200, _T("\nпервичных \t" F_TD), stat.firstAlloc );
      text += buf;

      _sntprintf( buf, 200, _T("\nосвобождений \t" F_TD), stat.lastAlloc );
      text += buf;

      if ( stat.realAllocIncremet ) {
        _sntprintf( buf, 200, _T("\nна увеличение \t" F_TD), stat.realAllocIncremet );
        text += buf;
      }

      if ( stat.realAllocDecremet ) {
        _sntprintf( buf, 200, _T("\nна уменьшение \t" F_TD), stat.realAllocDecremet );
        text += buf;
      }

#ifdef USE_REALLOC_IN_ARRAYS
      if ( stat.reallocCountSuccess ) {
        _sntprintf( buf, 200, _T("\nудачных realloc\t" F_TD), stat.reallocCountSuccess );
        text += buf;
      }
#endif

      _sntprintf( buf, 200, _T("\nmax одновременно " F_TD), stat.maxOneTimeLength );
      text += buf;

      if ( stat.realAllocIncremet ) {
        _sntprintf( buf, 200, _T("\nDelta min,max\t" F_TD "," F_TD ), stat.minDelta, stat.maxDelta );
        text += buf;
      }
    }
  }

#ifdef __ADRAFT_TTEST
  ::MessageTrace( string(text.c_str()).c_str(), _T("ReallocReport") );
  {
    t_ofstream out ( _T(LOG_PATH) _T("ADRAFT_TTEST.txt"), ios::out|ios::app );
    out << _T("\n") << _T("Статистика использования памяти \n") << text << _T("\n");
  }
#endif // __ADRAFT_TTEST

  if ( clear ) {
    // \ru очистить, чтобы в следующий раз цифры были новые, а не накопленные \en clear to renew numbers 
    allReallocCount = 0;
    for ( size_t i = 0; i < STAT_ARRAY_COUNT; i++ )
      statisticArray[i].Clear();
  }
}

#endif // __REALLOC_ARRAYS_STATISTIC_


//------------------------------------------------------------------------------
// \ru Использовать realloc для изменения размера массивов \en Use realloc to change arrays sizes 
// \ru (если не определено, то по-старому, через new и delete) \en (if it is not defined then use new and delete operators) 
//---
#ifdef USE_REALLOC_IN_ARRAYS

#ifdef C3D_DEBUG

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
//------------------------------------------------------------------------------
/// \ru Функция перезахватов памяти в массивах. \en Function of memory reallocation in arrays.  \~ \ingroup Base_Tools
// ---
inline void * ReallocArraySize( void * arr_parr, size_t newBytesCount, bool clear )
#else // __MEMSET_USED_FREE_HEAP_HEAR__
//------------------------------------------------------------------------------
/// \ru Функция перезахватов памяти в массивах. \en Function of memory reallocation in arrays.  \~ \ingroup Base_Tools
// ---
inline void * ReallocArraySize( void * arr_parr, size_t newBytesCount, bool )
#endif // __MEMSET_USED_FREE_HEAP_HEAR__
{
  __CHECK_POINTER( arr_parr, false/*alreadyFreed*/ ); // \ru проверить указатель \en check pointer 
  __CHECK_FREE_HEAP__

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
  if ( newBytesCount == 0 || clear ) {
    size_t arr_parr_size = ::_msize( arr_parr );
    PRECONDITION( arr_parr ? (arr_parr_size > 0 && arr_parr_size < 0xFFFFFFFF) : true ); // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
    if ( arr_parr_size )
      ::memset( arr_parr, 0xEE, arr_parr_size );
  }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

#if defined( _AFXDLL ) && defined( C3D_DEBUG )
  void * tmp_parr = _realloc_dbg( arr_parr, newBytesCount, _NORMAL_BLOCK, __FILE__, __LINE__ );
#else
  void * tmp_parr = ::realloc( arr_parr, newBytesCount );
#endif

  PRECONDITION( newBytesCount == 0 || tmp_parr != NULL ); // \ru проверка на нехватку памяти в массивах \en check the memory deficit in arrays  

#ifdef __MEMSET_USED_FREE_HEAP_HEAR__
  if ( clear ) {
    size_t tmp_parr_size = ::_msize( tmp_parr );
    PRECONDITION( tmp_parr ? (tmp_parr_size > 0 && tmp_parr_size < 0xFFFFFFFF) : true ); // \ru OV - надо доработать для 64-бит \en OV - it should be adapted for the 64-bit version 
    if ( tmp_parr_size )
      ::memset( tmp_parr, 0xEE, tmp_parr_size );
  }
#endif // __MEMSET_USED_FREE_HEAP_HEAR__

  // \ru __CHECK_POINTER( arr_parr, true/*alreadyFreed*/ ); // проверить указатель \en __CHECK_POINTER( arr_parr, true/*alreadyFreed*/ ); // check pointer 
  __FILL_FREE_HEAP__ // \ru заполнить все свободные блоки \en fill all free blocks 

  return tmp_parr;
}

#define REALLOC_ARRAY_SIZE(p,s,c) ::ReallocArraySize((p),(s),(c))
#else // C3D_DEBUG
#define REALLOC_ARRAY_SIZE(p,s,c) ::realloc((p),(s))
#endif // C3D_DEBUG

#endif // USE_REALLOC_IN_ARRAYS


#endif  // __MEMORY_DEBUG_H
