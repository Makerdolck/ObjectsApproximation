////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Реализация блокировок на базе системных механизмов синхронизации 
          и OpenMP блокировок.
         \en Locks implementation on base of system synchronization mechanisms
         and OpenMP locks. \~
  details \ru Реализация блокировок (в том числе блокировки в области видимости) 
          на базе системных механизмов синхронизации и OpenMP блокировок.\n
          \en Implementation of locks (including scoped lock) on base of 
          system synchronization mechanisms and OpenMP locks.\n \~
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __TOOL_MUTEX_H
#define __TOOL_MUTEX_H

#include <math_define.h>

//------------------------------------------------------------------------------
/** \brief \ru Управление блокировками.
\en Locks management. \~
\details \ru Переменная включает использование блокировок на базе
системных механизмов синхронизации вместо OpenMP блокировок,
что позволяет использование других механизмов распараллеливания (не только OpenMP).
\en The define enables using locks on base of system synchronization mechanisms instead of OMP locks,
that allows use of other parallelization frameworks (not only OpenMP).\n \~
\ingroup Base_Tools
*/
// ---
//#define C3D_NATIVE_LOCK


#ifdef C3D_NATIVE_LOCK

#ifdef _MSC_VER
#include <Windows.h>
#include <stdexcept> 

//------------------------------------------------------------------------------
/** \brief \ru Управление блокировками на базе механизмов синхронизации Windows.
\en Locks management on Windows. \~
\details \ru По умолчанию используется реализация на базе критических секций.
Переменная включает использование мьютекса вместо критической секции.
\en By default, the lock implementation of base of critical sections is used.
The define enables using mutex instead of critical section.\n \~
\ingroup Base_Tools
*/
// ---
//#define C3D_USE_NATIVE_MUTEX

//------------------------------------------------------------------------------
/** \brief \ru Обертка для Windows API.
\en Wrapper for Windows synchronization API. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonMutex
{
#ifdef C3D_USE_NATIVE_MUTEX
  HANDLE m_hMutex;
#else
  CRITICAL_SECTION m_critical_section;
#endif

public:
  CommonMutex()
  {
#ifdef C3D_USE_NATIVE_MUTEX
    m_hMutex = CreateMutex ( NULL, FALSE, NULL );
#else
    try {
      if ( !::InitializeCriticalSectionAndSpinCount(&m_critical_section, 0x1000/*0x80000000*/) )
        throw std::runtime_error("Mutex initialization error");
    }
    catch ( ... ) {
      throw;
    }
#endif
  }
  ~CommonMutex()
  {
#ifdef C3D_USE_NATIVE_MUTEX
    CloseHandle ( m_hMutex );
#else
    ::DeleteCriticalSection(&m_critical_section);
#endif
  }

  void lock()    
  { 
#ifdef C3D_USE_NATIVE_MUTEX
    WaitForSingleObject ( m_hMutex, INFINITE );
#else
    try {
      ::EnterCriticalSection(&m_critical_section);
    }
    catch ( ... ) {
      throw;
    }
#endif
  }
  void unlock()
  {
#ifdef C3D_USE_NATIVE_MUTEX
    ReleaseMutex ( m_hMutex );
#else
    ::LeaveCriticalSection(&m_critical_section);
#endif
  }

private:
  // \ru Запрет копирования. \en Copy forbidden.
  CommonMutex ( const CommonMutex& );
  CommonMutex& operator = ( const CommonMutex& );
};
#else
#include <pthread.h>
//------------------------------------------------------------------------------
/** \brief \ru Обертка для Linux мьютекс API.
\en Wrapper for Linux mutex API. \~
\ingroup Base_Tools
*/
// ---
class CommonMutex
{
  pthread_mutex_t m_mutex;

public:
  CommonMutex()
  { // Init recursive mutex
    pthread_mutexattr_t attr;
    pthread_mutexattr_init ( &attr );
    pthread_mutexattr_settype ( &attr, PTHREAD_MUTEX_RECURSIVE );
    pthread_mutex_init ( &m_mutex, &attr );
    pthread_mutexattr_destroy ( &attr );
  }
  ~CommonMutex() { pthread_mutex_destroy( &m_mutex ); }

  void lock()    { pthread_mutex_lock ( &m_mutex ); }
  void unlock()  { pthread_mutex_unlock ( &m_mutex ); }

private:
  // \ru Запрет копирования. \en Copy forbidden.
  CommonMutex ( const CommonMutex& );
  CommonMutex& operator = ( const CommonMutex& );
};
#endif // _MSC_VER

#else // C3D_NATIVE_LOCK

//------------------------------------------------------------------------------
/** \brief \ru Обертка для OpenMP lock.
\en Wrapper for OpenMP lock. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonMutex
{
  omp_lock_t m_lock;
public:
  CommonMutex()  { omp_init_lock( &m_lock ); }
  ~CommonMutex() { omp_destroy_lock( &m_lock ); }
  void lock()    { omp_set_lock( &m_lock ); }
  void unlock()  { omp_unset_lock( &m_lock ); }
};

//------------------------------------------------------------------------------
/** \brief \ru Обертка для вложенного OpenMP lock.
\en Wrapper for nested OpenMP lock. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS CommonRecursiveMutex
{
  omp_nest_lock_t m_lock;
public:
  CommonRecursiveMutex()  { omp_init_nest_lock( &m_lock ); }
  ~CommonRecursiveMutex() { omp_destroy_nest_lock( &m_lock ); }
  void lock()    { omp_set_nest_lock( &m_lock ); }
  void unlock()  { omp_unset_nest_lock( &m_lock ); }
};

#endif // C3D_NATIVE_LOCK

//------------------------------------------------------------------------------
/** \brief \ru Блокировка в области видимости.
\en Scoped lock. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS ScopedLock
{
  CommonMutex* m_mutex;
public:
  ScopedLock (CommonMutex* mutex) : m_mutex (mutex) { m_mutex->lock(); }
  ~ScopedLock () { m_mutex->unlock(); }

private:
  ScopedLock();
  ScopedLock ( const ScopedLock& );
  ScopedLock& operator = ( const ScopedLock& );
};

//------------------------------------------------------------------------------
/** \brief \ru Рекурсивная блокировка в области видимости.
\en Recursive scoped lock. \~
\ingroup Base_Tools
*/
// ---
class MATH_CLASS ScopedRecursiveLock
{
  CommonRecursiveMutex* m_mutex;
public:
  ScopedRecursiveLock (CommonRecursiveMutex* mutex) : m_mutex (mutex) { m_mutex->lock(); }
  ~ScopedRecursiveLock () { m_mutex->unlock(); }

private:
  ScopedRecursiveLock();
  ScopedRecursiveLock ( const ScopedRecursiveLock& );
  ScopedRecursiveLock& operator = ( const ScopedRecursiveLock& );
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект, предоставляющий средства синхронизации.
           \en Base object providing means of synchronization. \~
    \details \ru Базовый объект, предоставляющий средства синхронизации. \n
             \en Base object providing means of synchronization. \n \~
\ingroup Base_Tools
*/  
// ---  
class MbSyncItem {
protected:
  mutable omp_lock_t lock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.

public:
  MbSyncItem() {  omp_init_lock ( &lock ); }
  virtual ~MbSyncItem() { omp_destroy_lock( &lock ); }

  void Lock() const { omp_set_lock( &lock ); } 
  void Unlock() const { omp_unset_lock( &lock ); } 
};

//------------------------------------------------------------------------------
/** \brief \ru Базовый объект синхронизации, поддерживающий множественные блокировки.
           \en Base synchronization object with support of nested locks. \~
    \details \ru Базовый объект синхронизации, поддерживающий множественные блокировки. \n
             \en Base synchronization object with support of nested locks. \n \~
\ingroup Base_Tools
*/  
// ---  
class MbNestSyncItem {
protected:
  mutable omp_nest_lock_t lock;  // \ru Критическая секция для монопольного доступа к объекту. \en The critical section for exclusive access to the object.

public:
  MbNestSyncItem() {  omp_init_nest_lock ( &lock ); }
  virtual ~MbNestSyncItem() { omp_destroy_nest_lock( &lock ); }

  void Lock() const { omp_set_nest_lock( &lock ); } 
  void Unlock() const {  omp_unset_nest_lock( &lock ); } 
};


//------------------------------------------------------------------------------
// \ru Установлен ли режим безопасной многопоточности (используется в CacheManager).
//  \en Whether is enabled a safe multithreading mode (used in CacheManager).
// ---
MATH_FUNC(bool) IsSafeMultithreading();

//------------------------------------------------------------------------------
// \ru Создать идентификатор текущего потока.
//  \en Create a current thread identifier.
// ---
MATH_FUNC(unsigned int) CreateThreadKey();

//------------------------------------------------------------------------------
// \ru Получить идентификатор текущего потока.
//  \en Get a current thread identifier.
// ---
MATH_FUNC(unsigned int) GetThreadKey();

//------------------------------------------------------------------------------
// \ru Получить указатель на мьютекс для операций выделения и освобождения памяти.
// \en Get a pointer to the mutex for memory allocation and deallocation operations.
// ---
MATH_FUNC(CommonRecursiveMutex*) GetMemoryLock();

//------------------------------------------------------------------------------
// \ru Установить блокировку в области видимости для операций выделения и освобождения памяти.
// \en Set scoped lock for memory allocation and deallocation operations.
// ---
#define SET_MEMORY_SCOPED_LOCK ScopedRecursiveLock memScopedLock( GetMemoryLock() );

#endif // __TOOL_MUTEX_H
