////////////////////////////////////////////////////////////////////////////////
/**
  \file
  \brief \ru Класс PrimitiveCount представляет функционал для подсчета количества объектов в сцене.
         \en NO TRANSLATION. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_PRIMITIVECOUNT_H
#define __VSN_PRIMITIVECOUNT_H

#include "vsn_global.h"

VSN_BEGIN_NAMESPACE

//------------------------------------------------------------------------------
/** \brief \ru Класс PrimitiveCount представляет функционал для подсчета количества объектов в сцене.
           \en NO TRANSLATION. \~
    \ingroup Vision_Common
*/
// ---
class VSN_CLASS PrimitiveCount
{
public:
  // конструктор по умолчанию
  PrimitiveCount();
  // деструктор
  virtual ~PrimitiveCount();
public:
  // вернуть признак активности счетчика
  static bool IsActivated();
  // активизировать счетчик для подсчетов
  static void SetActivation( bool activation );
  // сбросить все счетчики
  static void ResetCounters();

  // Returns number of triangles in graph.
  static int GetTriangleCount();
  // Adds \a num triangles to total count. Used by node instances in the
  // scene graph during traversal.
  static void AddNumTriangles( int num );
    
  // Returns number of bodies in graph.
  static int GetBodyCount();
  // Adds \a num bodies to total count. Used by node instances in the
  // scene graph during traversal.
  static void AddNumBodies( uint bodies );
};
VSN_END_NAMESPACE

#endif // __VSN_PRIMITIVECOUNT_H
