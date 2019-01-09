////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Системозависимые функции.
         \en System-dependent functions. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __SYSTEM_DEPENDENCY_H
#define __SYSTEM_DEPENDENCY_H

#include <system_types.h>
#include <system_cpp_standard.h>

#ifdef _MSC_VER 
#define LOG_PATH     _T("C:\\Logs\\")
#else // _MSC_VER 
#define LOG_PATH     _T("")
#endif // _MSC_VER 

#ifndef _MSC_VER

#include <stdio.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <limits.h>
#include <tool_cstring.h>
#include <unistd.h>


#if (C3D_GLIBC_VERSION >= 2023)  // C3D_GLIBC_VERSION
  #include <cmath>
  #define c3d_isnan(d) std::isnan(d)
#else  // C3D_GLIBC_VERSION
  #include <math.h>
  #define c3d_isnan(d) isnan(d)
#endif  // C3D_GLIBC_VERSION

#define MAX_PATH PATH_MAX

#define _finite std::isfinite


//------------------------------------------------------------------------------
// \ru RGB-цвет. \en RGB color. 
// ---
typedef uint32 COLORREF;


//------------------------------------------------------------------------------
// RGBs of int colors from WinApi
// ---
#define RGB(r, g ,b)  ((uint32) (((uint8) (r) | ((uint16) (g) << 8)) | (((uint32) (uint8) (b)) << 16))) 


//------------------------------------------------------------------------------
// \ru Дать значение красного цвета в пределах 0 - 255. \en Get value of red color in range from 0 to 255. 
// ---
inline uint8 GetRValue(COLORREF rgb_color)
{
  return (uint8) (rgb_color);
}


//------------------------------------------------------------------------------
// \ru Дать значение зелёного цвета в пределах 0 - 255. \en Get value of green color in range from 0 to 255. 
// ---
inline uint8 GetGValue(COLORREF rgb_color)
{
  return (uint8) (rgb_color >> 8);
}


//------------------------------------------------------------------------------
// \ru Дать значение синего цвета в пределах 0 - 255. \en Get value of blue color in range from 0 to 255. 
// ---
inline uint8 GetBValue(COLORREF rgb_color)
{
  return (uint8) (rgb_color >> 16);
}


// \ru Вывод сообщений об ошибках пока отключаем в Линуксе \en Output of error messages is temporary disabled in Linux 
#define _RPT0(warnlvl, text)
#define _RPT1(a1, a2, a3) // \ru не реализовано \en not implemented  
#define _CRT_WARN
#define _CRT_ASSERT

#else // _MSC_VER

#include <windows.h>

// \ru Одновременное определение _MBCS и _UNICODE недопустимо! \en Simultaneous definition of _MBCS and _UNICODE is not acceptable! 
#if defined(_MBCS) && defined(_UNICODE)
  #error Multibyte Character Sets (MBCS) not supported
#endif // _MBCS

  #define c3d_isnan(d) _isnan(d)

#endif // _MSC_VER

#include <algorithm>
#ifdef STANDARD_C11
#include <chrono>
// #error <thread> is not supported when compiling with /clr or /clr:pure.
// #include <thread>
#endif


//------------------------------------------------------------------------------
// \ru Используется только в tool_cstring.h \en Used only in tool_cstring.h 
// ---
inline TCHAR * CharLowerEx( TCHAR * s )
{ 
#ifdef _MSC_VER // \ru не реализованно \en not implemented  
  return ::CharLower(s); 
#else // _MSC_VER
  PRECONDITION( false );
  return s; 
#endif // _MSC_VER
}


//------------------------------------------------------------------------------
// \ru Используется только в tool_cstring.h \en Used only in tool_cstring.h 
// ---
inline TCHAR * CharUpperEx( TCHAR * s )
{ 
#ifdef _MSC_VER // \ru не реализованно \en not implemented  
  return ::CharUpper(s); 
#else // _MSC_VER
  PRECONDITION( false );
  return s; 
#endif // _MSC_VER
}

//------------------------------------------------------------------------------
// \ru Используеися только в io_tape.h \en Used only in io_tape.h  
// ---
#if defined(C3D_DEBUG)
inline void MessageTrace( const TCHAR * lpText, const TCHAR * lpCaption = NULL )
{
#ifdef _MSC_VER // \ru не реализованно \en not implemented  
  ::MessageBox( (HWND)NULL, lpText, lpCaption, MB_ICONINFORMATION );
#else // _MSC_VER
  PRECONDITION( false );
#endif // _MSC_VER
}
#endif // C3D_DEBUG


//------------------------------------------------------------------------------
//
// ---
inline bool TestFilePresent( const TCHAR * strFileName, 
                             uint32 * year = NULL, uint32 * month = NULL, uint32 * day = NULL, 
                             uint32 * hour = NULL, uint32 * minute = NULL, uint32 * second = NULL )
{
#ifdef _MSC_VER // \ru не реализованно \en not implemented  
  bool bRes = false; 
  if ( strFileName && strFileName[0] )
  {
    // \ru запретить вывод сообщения об ошибке OpenFileErrorBox \en forbid output of OpenFileErrorBox error message 
    uint uiOldMode = ::SetErrorMode( SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX );

    WIN32_FIND_DATA w32fd;
    HANDLE hFinder = ::FindFirstFile( strFileName, &w32fd );
    if ( hFinder != INVALID_HANDLE_VALUE ) 
    {
      if ( year || month || day || hour || minute || second )
      {
        SYSTEMTIME sysTime;
        FileTimeToSystemTime(&(w32fd.ftLastWriteTime), &sysTime);

        if ( year   ) { *year   = sysTime.wYear;   }
        if ( month  ) { *month  = sysTime.wMonth;  }
        if ( day    ) { *day    = sysTime.wDay;    }
        if ( hour   ) { *hour   = sysTime.wHour;   }
        if ( minute ) { *minute = sysTime.wMinute; }
        if ( second ) { *second = sysTime.wSecond; }
      }
      
      ::FindClose( hFinder );
      bRes = true;
    }

    SetErrorMode( uiOldMode );
  }
  return bRes;

#else // _MSC_VER
  if ( strFileName != NULL || 
       year != NULL || month != NULL || day != NULL || 
       hour != NULL || minute != NULL || second != NULL ) 
  {
    // Для подавления warnings.
  }
  PRECONDITION( false );
  return false;

#endif // _MSC_VER
}


//------------------------------------------------------------------------------
// \ru Используется только в конвертерах \en Used only in converters 
// ---
inline bool DeleteFileEx( const TCHAR* fileName )
{ 
#ifdef _MSC_VER
  return !!::DeleteFile(fileName); 
#else // _MSC_VER
#ifndef __MOBILE_VERSION__
  return !!remove(c3d::C3DToPathstring( fileName ? c3d::string_t(fileName) : c3d::string_t() ).c_str());
#endif // __MOBILE_VERSION__
#endif // _MSC_VER
}


//------------------------------------------------------------------------------
// \ru Используется только в tool_time_test.cpp \en Used only in tool_time_test.cpp  
// ---
inline void ThreadPriority( bool normalPriority )
{
#ifdef _MSC_VER
  ::SetThreadPriority( ::GetCurrentThread(), normalPriority ? THREAD_PRIORITY_NORMAL : THREAD_PRIORITY_ABOVE_NORMAL );
#else // _MSC_VER
  if ( normalPriority ) 
  {
    // Для подавления warnings.
  }
  // \ru пока закрыто для мобильных платформ \en temporary closed for mobile platforms 
  //static int prio = getpriority(PRIO_PROCESS, getpid());
  //setpriority(PRIO_PROCESS, getpid(), normalPriority ? prio : getrlimit(RLIMIT_NICE, NULL));    
#endif // _MSC_VER
}


//------------------------------------------------------------------------------
// \ru Используется во многих местах для замеров производительности при отладке \en Used in many places for performance measurements in debug mode 
// ---
inline uint32 GetTickCountEx()
{
#ifndef  _MSC_VER 
  tms tm;
  return 10. * times(&tm);  // микросекунды, поэтому умножаем на 10, чтобы привести тоже к миллисекундам
#else // _MSC_VER 
  return ::GetTickCount();  // миллисекунды
#endif // _MSC_VER 
}


//------------------------------------------------------------------------------
// \ru Кросс-платформенная функция сна. \en Cross-platform sleep function.
// ---
inline void SleepCP( uint32 msec )
{
#ifdef _MSC_VER
  ::Sleep( msec );
#else
  usleep( msec * 1000 ); // usleep takes sleep time in us (1 millionth of a second)
#endif

// #error <thread> is not supported when compiling with /clr or /clr:pure.
/*
#ifdef STANDARD_C11
  std::this_thread::sleep_for( std::chrono::milliseconds( msec ) );
#else
#ifdef _MSC_VER
  ::Sleep( msec );
#else
  usleep( msec * 1000 ); // usleep takes sleep time in us (1 millionth of a second)
#endif
#endif
*/
}


//------------------------------------------------------------------------------
/** \brief \ru Высокоточный таймер.
           \en High resolution timer. \~ 
  \details \ru Высокоточный таймер.
           \en High resolution timer. \~
  \ingroup Base_Tools
*/
// ---
class MbAccurateTimer {
protected:
  double  lastTime;   ///< \ru Время в секундах. \en Time in seconds. 
private:
#ifdef STANDARD_C11
  std::chrono::high_resolution_clock::time_point begTime; ///< \ru Засечка времени. \en Time stamp. 
#else
  uint64  begTime; ///< \ru Засечка времени. \en Time stamp. 
#endif

public:
  /// \ru Конструктор. \en Constructor. 
  MbAccurateTimer() : lastTime( 0.0 ) {}

  /// \ru Сброшен ли таймер. \en Is empty timer?
  virtual bool    IsEmpty () const { return !(lastTime > 0.0); }
  /// \ru Сбросить таймер. \en Reset timer. 
  virtual void    SetEmpty()       { lastTime = 0.0; }
  /// \ru Добавить значение. \en Add time value. 
  virtual bool    SetTime ( double t ) { lastTime = t; return (t >= 0); }
  /// \ru Начать отсчет. \en Start time measurement. 
  virtual void    Begin  ();
  /// \ru Закончить отсчет. \en Finish time measurement. 
  virtual void    End    ();
  /// \ru Получить значение времени. \en Get time value. 
          double  GetLast() const { return lastTime; }
  /// \ru Получить значение частоты. \en Get frequency value. 
          double  PerSec () const { return (lastTime > 0.0 ? 1.0 / lastTime : 0.0); }
};


//------------------------------------------------------------------------------
// \ru Начать отсчет. \en Start time measurement. 
//---
inline void MbAccurateTimer::Begin()
{
#ifdef STANDARD_C11
  begTime = std::chrono::high_resolution_clock::now();
#else
  begTime = ::GetTickCountEx();
#endif
}

//------------------------------------------------------------------------------
// \ru Закончить отсчет. \en Finish time measurement. 
// ---
inline void MbAccurateTimer::End()
{
#ifdef STANDARD_C11
  std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
  double t = std::chrono::duration_cast< std::chrono::duration<double> >(endTime - begTime).count();
#else
  double t = (double)(::GetTickCountEx() - begTime) / 1000.0;
#endif
  SetTime( t );
}


//------------------------------------------------------------------------------
/** \brief \ru Таймер со статистикой.
           \en Average timer. \~ 
  \details \ru Таймер со статистикой.
           \en Average timer. \~
  \ingroup Base_Tools
*/
// ---
class MbAverageTimer : public MbAccurateTimer
{
  double  avgTime;  ///< \ru Среднее время в секундах. \en Average time in seconds. 
  double  minTime;  ///< \ru Минимальное время в секундах. \en Min. time in seconds. 
  double  maxTime;  ///< \ru Максимальное время в секундах. \en Max. time in seconds. 
  uint    runCount; ///< \ru Количество запусков. \en Number of samples. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbAverageTimer() : avgTime( 0.0 ), minTime( 0.0 ), maxTime ( 0.0 ), runCount( 0 ) {}

  virtual bool    IsEmpty() const { return (runCount == 0); }
  virtual void    SetEmpty();
  virtual bool    SetTime( double );
          double  GetAvg() const { return avgTime; }
          double  GetMin() const { return minTime; }
          double  GetMax() const { return maxTime; }
};

//------------------------------------------------------------------------------
// \ru Сбросить таймер. \en Reset timer. 
//---
inline void MbAverageTimer::SetEmpty()
{
  MbAccurateTimer::SetEmpty();
  avgTime = minTime = maxTime = 0.0;
  runCount = 0;
}

//------------------------------------------------------------------------------
// \ru Добавить значение. \en Add value. 
//---
inline bool MbAverageTimer::SetTime( double t )
{
  bool res = MbAccurateTimer::SetTime( t );

  if ( IsEmpty() )
    avgTime = minTime = maxTime = t;
  else
  {
    avgTime += (t - avgTime) / (runCount + 1);
    minTime = (std::min)( minTime, t );
    maxTime = (std::max)( maxTime, t );
  }

  runCount++;
  return res;
}


#ifndef threadLocal
# if __STDC_VERSION__ >= 201112 && !defined __STDC_NO_THREADS__
#  define threadLocal _Thread_local
# elif defined _WIN32 && ( \
       defined _MSC_VER || \
       defined __ICL || \
       defined __DMC__ || \
       defined __BORLANDC__ )
#  define threadLocal __declspec(thread) 
/* note that ICC (linux) and Clang are covered by __GNUC__ */
# elif defined __GNUC__ || \
       defined __SUNPRO_C || \
       defined __xlC__
#  define threadLocal __thread
# else
#  error "Cannot define thread_local"
# endif
#endif


#endif // __SYSTEM_DEPENDENCY_H
