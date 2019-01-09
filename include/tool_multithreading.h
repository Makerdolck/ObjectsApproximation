////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Управление параллельной обработкой данных.
       \en Managing of parallel data processing. \~
\details \ru Управление параллельной обработкой данных.\n
        \en Managing of parallel data processing. \n \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __TOOL_MULTITHREADING_H
#define __TOOL_MULTITHREADING_H

#include <system_dependency.h>
#include <tool_mutex.h>

//------------------------------------------------------------------------------
/**
\brief \ru Режимы многопоточных вычислений.
       \en Multithreading modes. \~
\details \ru Режимы многопоточных вычислений. \n
         \en Multithreading modes. \n \~
\ingroup Data_Structures
*/
//---
enum MbeMultithreadedMode {
  mtm_Off         = 0,  // \ru Многопоточность отключена. \en Multithreading is off. 
  mtm_Standard    = 1,  // \ru Включена многопоточность при обработке независимых друг от друга объектов (без общих данных). \en Multithreading is ON for independent objects (without common data).
  mtm_Items       = 2,  // \ru Включена многопоточность при обработке зависимых объектов (имеющих общие данные). \en Multithreading is ON for dependent objects (with shared data).
  mtm_Max         = 31  // \ru Включена максимальная многопоточность. \en Maximal multithreading is on.
};


//------------------------------------------------------------------------------
/**
\brief \ru Родительский класс данных для менеджера параллельной обработки.
       \en Parent class of data for manager of parallel processing. \~
\details \ru Родительский класс для данных, которые могут обрабатываться параллельно
         с помощью менеджера кэшей.
         \en Parent class for data which could be processed in parallel using the cache manager. \~
  \ingroup Base_Tools
*/
// ---
class AuxiliaryData {
public:
  AuxiliaryData() {}
  AuxiliaryData( const AuxiliaryData & ) {}
  virtual ~AuxiliaryData() {}
};


//------------------------------------------------------------------------------
/**
\brief \ru Менеджер параллельной обработки данных (менеджер кэшей).
       \en Manager for parallel data processing (the cache manager). \~
\details \ru Менеджер кэшей представляет шаблон, содержащий:
       longTerm - данные, которые используются при последовательном выполнении и
       для инициализации данных, обрабатываемых в других потоках;
       tcache - данные для потоков, которые используются при параллельном выполнении.
       Каждый поток по идентификатору threadKey использует только свою копию данных.
       Для многопоточной обработки зависимых (имеющих общие данные) объектов должен
       использоваться режим многопоточных вычислений mtm_Items. \n
       \en The cache manager is a template which contains:
       longTerm - data used in sequential execution and for initialization of thread data;
       tcache - data for threads which is used in parallel execution. 
       Each thread uses its own copy of data according to threadKey.
       For multithreaded processing of dependent (with shared data) objects,
       the mtm_Items multithreading mode should be used. \n \~
*/
// ---
template<class T>
class CacheManager {
  struct List
  {
    unsigned int _id;
    T*           _data;
    List*        _next;
    List( unsigned int id, T* data) : 
      _id( id ),
      _data( data != NULL ? data : new T() ), // _data != NULL
      _next( NULL ) {}
  private:
    List() : _id( 0 ), _data( NULL ), _next( NULL ) {}
  };

private:
  T*         longTerm;
  List*      tcache;
  CommonMutex  lock;

public:
  CacheManager();
  CacheManager( const CacheManager & );
  ~CacheManager();

  /** \brief \ru Получить указатель на кэш (данные) текущего потока. Всегда возвращает ненулевое значение.
      \en Get a pointer to the cache (data) of the current thread. Always returns non-null value. \~
  */
  T *   Cache    ();

  /** \brief \ru Оператор (). Возвращает указатель на кэш (данные) текущего потока. Всегда возвращает ненулевое значение.
      \en Operator (). Returns a pointer to the cache (data) of the current thread. Always returns non-null value. \~
  */
  T *   operator ()();

  /** \brief \ru Удалить кэши. Если resetLongTerm==true, также удалить кэш главного потока.
             \en Delete caches. If resetLongTerm==true, also delete the main thread cache. \~
  */
  void  Reset    ( bool resetLongTerm = false );

  /** \brief \ru Получить указатель на кэш (данные) главного потока. Всегда возвращает ненулевое значение.
                 Все операции с кэшем главного потока должны быть защищены блокировкой кэша.
             \en Get a pointer to cache (data) of the main thread. Always returns non-null value.
                 All operations with the main thread cache should be protected by the cache lock. \~
  */
  T *   LongTerm ();

  /** \brief \ru Получить указатель на блокировку для операций с кэшем главного потока. Всегда возвращает ненулевое значение.
  \en Get a pointer to the lock for operations with the main thread cache. Always returns non-null value. \~
  */
  CommonMutex* GetLock() { return &lock; }
  
private:
  CacheManager & operator = ( const CacheManager & );
};


//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template<class T>
inline CacheManager<T>::CacheManager()
  : longTerm( NULL )
  , tcache  ( NULL ) 
{
}


#define NULLKEY 0

//------------------------------------------------------------------------------
// \ru Конструктор. \en Constructor. 
// ---
template<class T>
inline CacheManager<T>::CacheManager( const CacheManager<T> & item )
  : longTerm( item.longTerm ? new T( *item.longTerm ) : NULL )
  , tcache  ( NULL ) 
{
}


//------------------------------------------------------------------------------
// \ru Деструктор. \en Destructor. 
// ---
template<class T>
inline CacheManager<T>::~CacheManager()
{ 
  Reset( true ); 
}

//------------------------------------------------------------------------------
// \ru Получить указатель на кэш главного потока. Всегда возвращает ненулевое значение.
//     Все операции с кэшем главного потока должны быть защищены блокировкой кэша.
// \en Get a pointer to the main thread cache. Always returns non-null value.
//     All operations with the main thread cache should be protected by the cache lock.
// ---
template<class T>
inline T* CacheManager<T>::LongTerm ()
{
  if ( longTerm == NULL )
    longTerm = new T();
  return longTerm;
}

//------------------------------------------------------------------------------
// \ru Получить указатель на кэш текущего потока. Всегда возвращает ненулевое значение.
// \en Get a pointer to the current thread cache. Always returns non-null value.
// ---
template<class T>
inline T * CacheManager<T>::Cache()
{
#define INIT_BY_LONGTERM ( longTerm ? new T( *longTerm ) : new T() )

  if ( !IsSafeMultithreading()
#ifndef C3D_NATIVE_LOCK
    || !omp_in_parallel()
#endif
    ) {
    if ( longTerm == NULL ) {
      ScopedLock sl(&lock);
      if ( longTerm == NULL ) 
        longTerm = new T();
    }
    Reset( false ); // \ru Оптимизация памяти (удалить неиспользуемые кэши). \en Memory optimization (delete unused caches).
    return longTerm;
  }

  T * res = NULL;
  unsigned int threadKey = GetThreadKey();
  if ( threadKey == NULLKEY ) {
    threadKey = CreateThreadKey();
  }

  {
    ScopedLock sl(&lock);

    if ( tcache == NULL ) 
    {
      tcache = new List( GetThreadKey(), INIT_BY_LONGTERM );
      res = tcache->_data;
    }

    if ( res == NULL ) {
      List* entry = tcache;
      while( entry != NULL ) {
        if( entry->_id == threadKey )
        {
          res = entry->_data;
          break;
        }
        // \ru Если кэш не найден в списке, 'entry' содержит последний (на данный момент) элемент в списке.
        // \en If cache not found in the list, 'entry' contains the last element in the list (at that point).
        if ( entry->_next == NULL)
          break; 
        entry = entry->_next;
      }
      if ( res == NULL ) {
          entry->_next = new List( threadKey, INIT_BY_LONGTERM );
          res = entry->_next->_data;
      }
    }
  } // ScopedLock

  return res;

}


//------------------------------------------------------------------------------
// \ru Оператор (). Возвращает указатель на кэш текущего потока (всегда ненулевое значение).
// \en Operator (). Returns a pointer to the current thread cache (always non-null value).
// ---
template<class T>
inline T * CacheManager<T>::operator()() 
{
  return Cache();
}


//------------------------------------------------------------------------------
// \ru Удалить кэши. Если resetLongTerm==true, удалить также эш главного потока.
// \en Delete caches. If resetLongTerm==true, also delete the main thread cache.
// ---
template<class T>
inline void CacheManager<T>::Reset( bool resetLongTerm )
{
  if ( resetLongTerm && longTerm != NULL ) {
    ScopedLock sl(&lock);
    if ( longTerm != NULL )
      delete longTerm;
    longTerm = NULL;
  }
  if ( tcache != NULL ) {
    ScopedLock sl(&lock);
    List* entry = tcache;
    while ( entry != NULL ) {
      delete entry->_data;
      tcache = entry->_next;
      delete entry;
      entry = tcache;
    }
    tcache = NULL;
  }
}


#endif // __TOOL_MULTITHREADING_H
