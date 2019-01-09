//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Модуль: GCRESCODE2D.
         \en Module: GCRESCODE2D. \~
  \details \ru Цель: Перечисление сообщений при решении параметрических систем в 2d.
           \en Target: Enumeration of messages while solving parametric systems in 2D \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __GCE_RES_CODE_H
#define __GCE_RES_CODE_H

#include <gce_types.h>

/**
  \addtogroup Constraints2D_API
  \{
*/

//----------------------------------------------------------------------------------------
/// \ru Состояние решения отдельного ограничения \en State of the separate constraint solving 
/**\ru <b>cst_None</b> - Нет данных. Код обычно показывает, что к решению этого
              ограничения не приступали или требуется перерешать еще раз после 
              изменения системы ограничений.\n
  <b>cst_Satisfied</b> - Ограничение решено. Означает, что ограничение решалось,
              и в текущий момент оно удовлетворено, даже если другие ограничения
              системы могут быть не решены.\n
  <b>cst_Redundant</b> - Избыточное ограничение. Ограничение не участвует в вычислениях,
              хотя удовлетворено, - является избыточным, т.е. удаление такого 
              ограничения из системы не изменит её множество решений, а наличие 
              не противоречит решению.\n
  <b>cst_Overconstraining</b> - Ограничение переопределяет систему. Ограничение делает
             систему уравнений несовместной, т.е. имеющей пустое множество
             решений. Что бы система решалась, нужно это ограничение удалить.\n
  <b>cst_Not_Satisfied</b> - Ограничение не решено. Причина ошибки не известна.
             Например, теоретически существует решение системы, но решатель не
             смог его найти.\n
  <b>cst_Unsolvable</b> - Ограничение не разрешимо. Такая ситуация возникает, если 
             ограничение вышло из области допустимых значений. Например, 
             линейный размер нулевой длины, не дифференцируем. Решатель не 
             может найти решение, где это ограничение могло бы быть 
             удовлетворено, или такое решение не существует.\n
   \en <b>cst_None</b> - No data. The code usually shows that solving of
              this constraint is not started or it is necessary to solve it again after 
              constraint system modification.\n
  <b>cst_Satisfied</b> - The constraint is solved. Means that the constraint was solved
              and currently it is satisfied, even though other constraints
              of the system can be not solved.\n
  <b>cst_Redundant</b> - Redundant constraint. The constraint is not involved in calculations
              although it is satisfied, - it is redundant, i.e. deletion of such 
              constraint from the system will not change the set of its solutions and its presence 
              does not contradict the solution.\n
  <b>cst_Overconstraining</b> - The constraint makes the system overconstrained. The constraint makes
             the system of equations inconsistent, i.e. having an empty set
             of solutions. This constraint is to be removed to make the system solvable.\n
  <b>cst_Not_Satisfied</b> - The constraint was not solved. Reason of an error is unknown.
             For example, solution of the system exists but the solver did not
             manage to find it.\n
  <b>cst_Unsolvable</b> - The constraint is unsolvable. Such situation appears if 
             the constraint exceeded the acceptable region. For example, 
             linear dimension of zero size, nondifferentiable. The solver 
             cannot find a solution where this constraint could be 
             satisfied or such solution does not exist.\n \~
  \attention
\ru ИДЕНТИФИКАТОРЫ НЕ МЕНЯТЬ!!! Значение идентификаторов могут записываться в файл.
\en DO NOT CHANGE IDENTIFIERS!!! Values of identifiers can be written to a file. \~
*/
//---
enum GcConState 
{
  cst_None = 0,         ///< \ru Нет данных \en No data 
  cst_Satisfied,        ///< \ru Ограничение решено \en Constraint is solved 
  cst_Redundant,        ///< \ru Ограничение решено, но является избыточным \en Constraint is solved but redundant 
  cst_Overconstraining, ///< \ru Ограничение не решено и является избыточным \en Constraint is not solved and redundant 
  cst_Not_Satisfied,    ///< \ru Ограничение не решено (по неизвестным причинам) \en Constraint is not solved (for unknown reasons) 
  cst_Unsolvable,       ///< \ru Ограничение не разрешимо \en Constraint is unsolvable 
};


//----------------------------------------------------------------------------------------

/**
  \}
*/
 

#endif // __GCE_RES_CODE_H

// eof