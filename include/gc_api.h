//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file     
  \brief \ru Тестовый программный интерфейс геометрического решателя C3D Solver.
         \en Testing program interface of C3D Solver. 
         \~

  \details \ru Данный файл содержит типы данных и вызовы, предназначенные для тестирования
            и отладки, поэтому могут быть изменены или удалены из API C3D Solver
            в будущих версиях. Для применения решателя двухмерных ограничений рекомендуется 
            использовать только интерфейс, объявленный в заголовочных файлах gce_api.h и gce_types.h.
           
           \en This file contains data types and calls for testing and debugging, so they 
           can be modified or removed from the C3D Solver API in future versions. To use 
           the 2D constraint solver, it is recommended to use only the interface declared 
           in the header files gce_api.h and gce_types.h.
           \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GC_API_H
#define __GC_API_H

#include <mb_matrix.h>
#include <gce_res_code.h>
//
#include "gce_types.h"
#include "gce_kompas_interface.h"

class MATH_CLASS            MbPolyCurve;
template <class Type> class SArray;
template <class Type> class FDPArray;

//----------------------------------------------------------------------------------------
// \ru Дескрипция координаты геометрического примитива. \en Description of geometric primitive coordinate.
//---
template<class Geom = geom_item>
struct geom_coord
{
  Geom        geom;
  coord_name  crdName;

  geom_coord( Geom g, coord_name cName ) : geom(g), crdName(cName) {}
  geom_coord( const geom_coord & crd )
  {
    geom = crd.geom;
    crdName = crd.crdName;    
  }
  geom_coord & operator = ( const geom_coord & crd )
  {
    geom = crd.geom;
    crdName = crd.crdName;
    return *this;
  }

private:
  geom_coord();
};

/**
  \addtogroup Constraints2D_API   
  \{
*/

//----------------------------------------------------------------------------------------
/// \ru Состояние определенности системы геометрических ограничений \en State of geometric constraints system. 
/*\ru Не менять идентификаторы, возможна фиксация этой величины в файлах.
  \en Do not change identifiers, this value can be fixed in files. \~
*/
//---
enum GcConstraintStatus
{
  tcs_Unknown = 0,         ///< \ru О состоянии ничего не известно. \en State is unknown. 
  tcs_WellConstrained = 1, ///< \ru Полностью определенная система - не имеет степеней свобод. \en Well-constrained system - does not have degrees of freedom. 
  tcs_UnderConstrained,    ///< \ru Недоопределенная система - имеются степени свободы. \en Underconstrained system - there are degrees of freedom. 
  tcs_OverConstrained,     ///< \ru Переопределенная система - система ограничений несовместна (есть противоречия). \en Overconstrained system - the system of constraints is inconsistent (there are contradictions). 
};

//----------------------------------------------------------------------------------------
/// \ru Собрать линейно зависимые и избыточные ограничения \en Collect linearly dependent and redundant constraints 
// ---
GCE_FUNC(bool) GCE_CollectLinearDependedConstrains( GCE_system, SArray<constraint_item> & ldepCons );

//----------------------------------------------------------------------------------------
/** \brief \ru Проверка: Останется ли система удовлетворенной, если  изменить координаты точки. 
           \en Check: Whether the system remains satisfied if the point coordinates are changed.
    \details
    \ru Функция работает корректно, если на момент её вызова система ограничений решена. При проверке 
  не происходит пробного перерешивания системы, а оценивается лишь удовлетворенность 
  смежных ограничений при новом параметре (px,py). Функция может быть применена для оценки области значений(окрестность 
  некоторой погрешности) точки, в которой система остается удовлетворенной.
    \en The function works correctly if the system is in resolved state, during the check 
  a test resolving of the system is not performed, only satisfaction 
  of adjacent constraints with a new parameter (px,py) is estimated.
        The function can be applied for estimation of domain (neighborhood 
  of some tolerance) of the point in which the system remains satisfied. \~
*/
//---
GCE_FUNC(bool) GCE_CheckPointSatisfaction( GCE_system gSys, geom_item pnt, point_type cp, double px, double py );

//----------------------------------------------------------------------------------------
/// \ru Выдать состояние ограничения \en Get the state of constraint 
/**
  \param gc_item - \ru ограничение пользователя
                   \en user constraint \~
  \return \ru Статус ограничения
          \en State of constraint \~
  \note
\ru Правильный результат гарантирован только после того, как система была решена.
  Когда функция выдает переопределенность для конкретного ограничения, то это не
  означает, что именно оно является проблемным. Переопределенность всегда 
  охватывает множество ограничений, решатель "случайно" выбирает в качестве 
  проблемного одно из них.
\en Correct result is guaranteed only after system is resolved.
  When the function returns overconstraint for the specific constraint, then it does not
  mean that this constraint is problem. Overdetermination always 
  involve a set of constraints, the solver "randomly" chooses one of them 
  as problem. \~
*/
//---
GCE_FUNC(GcConState) GCE_GetConstraintState( GCE_system, constraint_item gc_item );

//----------------------------------------------------------------------------------------
/// \ru Выдать состояние системы ограничений \en Get constraint system state 
/**
  \param \ru gSys Контекст решателя
         \en gSys Solver state \~
  \return \ru Статус ограничения
          \en State of constraint \~
  \note \ru Сложность выполнения функции эквивалентна запросам GCE_GetCoordinateDOF(), GCE_GetPointDOF()
        \en Function complexity is equivalent  to requests GCE_GetCoordinateDOF(), GCE_GetPointDOF() \~
*/
//---
GCE_FUNC(GcConstraintStatus) GCE_GetConstraintStatus( GCE_system gSys );

//----------------------------------------------------------------------------------------
/// \ru Проверить, удовлетворена ли система ограничений. \en To check the satisfaction of constraints.
/**
  For internal use only!
  \param \ru gSys - Контекст решателя.
         \en gSys - Solver state. \~
  \param \ru c3dVer - Версия ядра c3d.
         \en c3dVer - Mathematical kernel version. \~
  \return \ru Код результата вычислений.
          \en Calculation result code. \~
*/
//---
GCE_FUNC(GCE_result) GCE_CheckSatisfaction( GCE_system gSys, VERSION c3dVer );

//----------------------------------------------------------------------------------------
/** \brief \ru Выдать степень свободы отдельной координаты (переменной).
           \en Get the degree of freedom of separate coordinate (variable).
  \details  \ru Функция возвращает степень свободы координаты; Если возвращается значение < 0,
                то вычислить степень свободы не удалось.
            \en The function returns degree of freedom of the coordinate;    If a negative 
                value is returned, then it is failed to calculate the degree of freedom. \~
*/
//--
GCE_FUNC(ptrdiff_t) GCE_GetCoordinateDOF( GCE_system, geom_coord<> crd );

//----------------------------------------------------------------------------------------
/// \ru Выдать координаты переменных геометрической модели, \en Get coordinates of variables of geometric models 
/// \ru значения которых не зависят от изменения входных переменных in_coords; \en which do not depend on changes of input variables in_coords; 
// ---
GCE_FUNC(bool)  GCE_GetOutVarCoordinates( GCE_system gcContext, 
                                             const SArray<var_item> & in_coords, 
                                             const SArray<constraint_item> & drvCons, 
                                                   SArray<var_item> & outCoords );

//----------------------------------------------------------------------------------------
/// \ru Задать фиксацию координаты параметрического объекта \en Specify fixation of a parametric object coordinate 
//---
GCE_FUNC(constraint_item) GCE_FixCoordinate( GCE_system gSys, geom_item g, coord_name crd );

//----------------------------------------------------------------------------------------
/// \ru Задать ограничение "Радиальный размер" \en Specify "Radial dimension" constraint  
/**
  \param cir   - \ru окружность или дуга
                 \en a circle or an arc \~        
  \param diam  - \ru признак диаметрального размера
                 \en flag of diametral dimension \~
*/
//---
GCE_FUNC(constraint_item) GCE_FormCirDimension( GCE_system gcContext, geom_item cir, GCE_dim_pars dPars, bool diam );

//----------------------------------------------------------------------------------------
/// \ru Отменить режим драггинга \en Cancel dragging mode 
// ---
GCE_FUNC(void) GCE_ResetMovingMode( GCE_system );

/**
  \}
  Constraints2D_API
*/

/*
  Deprecated functions
*/

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Рекомендуется использовать #GCE_AddSymmetry.
             \en The function is obsolete. It is recommended to use #GCE_AddSymmetry. \~ 
 */
//---
GCE_FUNC(constraint_item) GCE_FormPointSymmetry( GCE_system gcContext, geom_item pnt[2], geom_item curve, int8 );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Рекомендуется использовать #GCE_AddUnaryConstraint
             \en The function is obsolete Use #GCE_AddUnaryConstraint instead. \~
*/
//---
GCE_FUNC(constraint_item) GCE_FormFixedLineAngle( GCE_system, geom_item, constraint_type );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Рекомендуется использовать #GCE_AddIncidence.
             \en The function is obsolete. It is recommended to use #GCE_AddIncidence. \~ 
*/
GCE_FUNC(constraint_item) GCE_FormPointOnCurve( GCE_system, geom_item, geom_item );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Рекомендуется использовать #GCE_ChangeDrivingDimension
             \en The function is obsolete It is recommended to use #GCE_ChangeDrivingDimension \~ 
*/
//---
GCE_FUNC(GCE_result) GCE_ChangeFixedDimension( GCE_system, constraint_item, double, bool );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Устаревшая функция. Вызов будет удален в одной из следующих версий. 
                 Вместо неё используйте #GCE_PrepareMovingGeoms.
             \en An obsolete function. The call will be removed in one of the next versions. 
                 Use #GCE_PrepareMovingGeoms instead. \~
*/
//---
GCE_FUNC(GCE_result) GCE_PrepareMovingOfGeoms( GCE_system, SArray<geom_item> &, double, double );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Устаревшая функция. Вызов будет удален в одной из следующих версий (2016). Вместо неё используйте #GCE_AddAngle4P.
             \en An obsolete function. The call will be removed in one of the next versions (2016). Use #GCE_AddAngle4P instead. \~
*/
//---
GCE_FUNC(constraint_item)  GCE_Form_AngDimension4P ( GCE_system, const GCE_dim_pars & 
                                                   , geom_item, geom_item, geom_item, geom_item 
                                                   , double, bool, bool, bool );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Вместо неё применять #GCE_Evaluate.
             \en The function is obsolete. Use #GCE_Evaluate instead. \~
*/
//---
GCE_FUNC(GCE_result) GCE_ApplyChanges( GCE_system );

//----------------------------------------------------------------------------------------
/**
  \attention \ru Функция устарела. Вместо неё применять #GCE_AddFixedLength.
             \en The function is obsolete Use #GCE_AddFixedLength instead. \~
*/
//---
GCE_FUNC(constraint_item) GCE_FormFixedLength( GCE_system, geom_item );

//----------------------------------------------------------------------------------------
/*
  \attention \ru Функция устарела. Вместо неё применять #GCE_FixCoordinate.
  \en The function is obsolete Use #GCE_FixCoordinate instead. \~
*/
//---
GCE_FUNC(constraint_item) GCE_FormFixedCoordinate( GCE_system, geom_coord<> );

//----------------------------------------------------------------------------------------
/*
  Internal use only
*/
//---
GCE_FUNC(GCE_system) GCE_RestoreFromJournal( const char * fName );

//----------------------------------------------------------------------------------------
/**
  \note Used only for testing
*/
//---
GCE_FUNC(const GCE_diagnostic_pars &) GCE_DiagnosticPars( GCE_system gSys );

//----------------------------------------------------------------------------------------
// Measure a dimension value (it used for testing purposes only)
//---
GCE_FUNC(double) GCT_Measure( GCE_system gSys, constraint_type cType, geom_item g1, geom_item g2 );

struct GCT_auto_c_query;

//----------------------------------------------------------------------------------------
// Функция обратного вызова для подтверждения автоограничения
// Callback function to confirm auto-constraint
//---
typedef double ( *GCT_auto_c_weight )( GCT_auto_c_query *, constraint_type cType
                                     , geom_item g1, geom_item g2 );

//----------------------------------------------------------------------------------------
// Событийная функция: регистрация нового авто-ограничения в системе
// Event function: a new auto-constraint is registered in the system.
//---
typedef void ( *GCT_auto_c_registered )( GCT_auto_c_query *, GCE_system gSys, constraint_item cItem );

//----------------------------------------------------------------------------------------
// It always returns 1.0
//---
inline double _DefaultWeight( GCT_auto_c_query *, constraint_type
                            , geom_item, geom_item ) { return 1.0; }

//----------------------------------------------------------------------------------------
// Структура запроса автоограничений
// Data structure of a query to autoconstrain
//---
struct GCT_auto_c_query
{
  std::vector<geom_item> geoms;    ///< \ru Множество объектов автоограничивания. \en Set of objects of auto-constraining.
  GCT_auto_c_weight      wFunc;
  GCT_auto_c_query() : geoms(), wFunc( _DefaultWeight ) {}

private:
  GCT_auto_c_query( const GCT_auto_c_query & );
  GCT_auto_c_query & operator = ( const GCT_auto_c_query & );
};

//----------------------------------------------------------------------------------------
//
//---
enum GCT_auto_c_result
{
    GCE_AUTO_C_DONE
  , GCE_NO_AUTO_CONSTRAINT
};

//----------------------------------------------------------------------------------------
// Задать автоматическое ограничение для данной пары объектов.
// Set automatic constraint for the given pair of geometric objects.
//---
GCE_FUNC(constraint_item) GCT_AutoConstrain( GCE_system gSys, geom_item g1, geom_item g2, GCT_auto_c_query * );

//----------------------------------------------------------------------------------------
// Автоматическая генерация ограничений.
// Automatically generate constraints.
//---
GCE_FUNC(GCT_auto_c_result) GCT_AutoConstrain( GCE_system gSys, GCT_auto_c_query * );

#endif

// eof
