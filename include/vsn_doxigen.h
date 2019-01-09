////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Группы для документирования с помощью Doxygen.
         \en Groups for documenting with Doxygen. \~
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __VSN_DOXIGEN_H
#define __VSN_DOXIGEN_H

//-----------------------------------------------------------------------------
//
//
// \ru Группы модуля "C3D Vision" визуализации геометрических данных. \en C3D Vision groups of geometry data visualization. \~
//
//
//-----------------------------------------------------------------------------
/**
  \ru \defgroup  Vision_Data Модуль визуализации геометрических данных
  \en \defgroup  Vision_Data The Geometry Data Visualization Module
  \~ \ingroup Vision_Data
*/


//-----------------------------------------------------------------------------
//
// \ru Подгруппа Vision_Viewport - Модуль для отрисовки сцены и ее управления. \en Module for rendering scene and its control. \~
//
//-----------------------------------------------------------------------------
/**
\ru \defgroup  Vision_SceneGraph    Представление сцены в виде графа
\en \defgroup  Vision_SceneGraph    Scene representation set as a graph
\~ \ingroup Vision_Data
*/

/**
\ru \defgroup  Vision_Geometry    Геометрическое представление сцены
\en \defgroup  Vision_Geometry    Geometry representation for the scene
\~ \ingroup Vision_Data
*/

/**
\ru \defgroup  Vision_Viewport    Отрисовка и управление сценой
\en \defgroup  Vision_Viewport    Scene rendering and manipulation
\~ \ingroup Vision_Data
*/

/**
  \ru \defgroup Vision_Process   Процессы управления камерой
  \en \defgroup Vision_Process   Camera control processes
  \~ \ingroup  Vision_Viewport
*/



#endif /* __VSN_DOXIGEN_H */
