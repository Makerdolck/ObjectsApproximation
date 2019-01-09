//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Расчет пересечений тел посредством аппарата булевой операции.
         \en Calculation of intersections between solids using the boolean operations. \~

*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDET_BOOL_H
#define __CDET_BOOL_H


#include <mb_operation_result.h>
#include <mb_enum.h>
#include <vector>


class  MATH_CLASS MbSolid;
class  MATH_CLASS MbSNameMaker;
class  MATH_CLASS MbCurveEdge;


//----------------------------------------------------------------------------------------
/** \brief \ru Расчет пересечений тел посредством аппарата булевой операции.
           \en Calculation of intersections between solids using the boolean operations. \~
  \details \ru Расчет пересечений тел посредством аппарата булевой операции. 
           \en Calculation of intersections between solids using the boolean operations. \~ \n
  \param[in] solid1 - \ru Первое тело. \en The first solid. \~
  \param[in] solid2 - \ru Второе тело. \en The second solid. \~
  \param[out] edges - \ru Ребра пересечения тел. \en Intersection edges. \~
  \param[out] (*faceNumbers)[4] - \ru Номера пересекшихся граней. \n
                faceNumbers[0] - пересекшиеся грани первого тела, \n
                faceNumbers[1] - пересекшиеся грани второго тела, \n
                faceNumbers[2] - подобные грани первого тела, \n
                faceNumbers[3] - подобные грани второго тела. \n
                                  \en Numbers of intersection faces. \n
                faceNumbers[0] - intersected faces of the first solid, \n
                faceNumbers[1] - intersected faces of the second solid, \n
                faceNumbers[2] - similar faces of the first solid, \n
                faceNumbers[3] - similar faces of the second solid. \n \~
  \return \ru Код результата операции. \en Operation result code. \~
  
  \warning \ru Тела будут изменены операцией! Если требуется сохранить тела без изменений, 
               передавайте копии, сделанные помощью MbSolid::Duplicate().
           \en The solids will be modified by this operation! To keep the body intact,
               give the copies made using MbSolid::Duplicate(). \~

  \ingroup Collision_Detection
*/
//---
MATH_FUNC (MbResultType) InterferenceSolids( MbSolid & solid1, MbSolid & solid2,
                                             std::vector<MbCurveEdge*> * edges,
                                             std::vector<ptrdiff_t> (*faceNumbers)[4] );


#endif // __CDET_BOOL_H

