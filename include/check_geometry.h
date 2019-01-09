////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Диагностика оболочек и их составляющих.
         \en Diagnostics of shells and their components. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CHECK_GEOMETRY_H
#define __CHECK_GEOMETRY_H


#include <mb_operation_result.h>
#include <mb_variables.h>
#include <topology.h>
#include <vector>


class  MATH_CLASS MbCurve3D;
class  MATH_CLASS MbSurfaceIntersectionCurve;
class  MATH_CLASS MbFaceShell;
class  MATH_CLASS MbSolid;


//------------------------------------------------------------------------------
/** \brief \ru Информация о пересечении двух тел.
           \en Information about two solids intersection. \~
  \details \ru Информация о пересечении двух тел при диагностике их оболочек. \n
           \en Information about intersection of two solids during diagnostics of their shells. \n \~
  \ingroup Algorithms_3D
*/
//---
struct MATH_CLASS MbIntersectionData {
protected:
  RPArray<MbCurveEdge> curves;         ///< \ru Ребра пересечения (владеет по счетчику ссылок). \en Intersection edges (owns by reference counter). 
  SArray<ptrdiff_t>    faceNumbers1;   ///< \ru Номера касающихся граней первого тела. \en The numbers concerning faces of the first  solid.
  SArray<ptrdiff_t>    faceNumbers2;   ///< \ru Номера касающихся граней второго тела. \en The numbers concerning faces of the second solid.
  MbSolid *            solid;          ///< \ru Тело пересечения  (владеет по счетчику ссылок). \en Intersection solid (owns by reference counter). 
  bool                 isTangentCurve; ///< \ru Пересечения - это линии касания. \en Intersections are tangency lines. 
  bool                 isSolid;        ///< \ru Пересечения образуют тела (для BUG_39711). \en Intersections form solids  (for BUG_39711).  

public:
  MbIntersectionData();
  MbIntersectionData( const MbCurveEdge & edge );
  MbIntersectionData( const RPArray<MbCurveEdge> & edges, bool isSolidEgdes );
  MbIntersectionData( const RPArray<MbCurveEdge> & edges, const SArray<ptrdiff_t> & numbers1, const SArray<ptrdiff_t> & numbers2 );
  MbIntersectionData( const MbSolid & sol  );
  ~MbIntersectionData();

public:
  /// \ru Пересечение точкой. \en Intersection is a point. 
  bool IsPoint()   const { return (solid == NULL && !isSolid) && (curves.Count() < 1) ;} 
  /// \ru Пересечение вдоль касательной линии. \en Intersection along a tangent line. 
  bool IsCurve()   const { return  isTangentCurve && (curves.Count() > 0); } 
  /// \ru Пересечение касательной областью поверхности. \en Intersection by a tangent region of a surface. 
  bool IsSurface() const { return !isTangentCurve && (curves.Count() > 0); } 
  /// \ru Пересечение - есть тело. \en Intersection is a solid. 
  bool IsSolid()   const { return (solid != NULL || isSolid); }

  /// \ru Установить флаг пересечения вдоль касательной линии. \en Set the flag of intersection along a tangent line. 
  void SetTangent( bool b ) { isTangentCurve = b; } 

  /// \ru Отдать указатель для просмотра тела. \en Get a pointer for viewing the solid. 
  const MbSolid     * GetSolid() const { return solid; } 
  /// \ru Отдать указатель для просмотра/модификации тела. \en Get a pointer for viewing/modification of the solid. 
        MbSolid     * SetSolid()       { return solid; }

  /// \ru Количество кривых пересечения. \en Count of intersection curves. 
        size_t        GetCurvesCount() const { return curves.Count(); } 
  /// \ru Получить массив кривых пересечения. \en Get the intersection curve array. 
        void          GetCurves( RPArray<MbCurveEdge> & crvs ) const { crvs.AddArray( curves ); } 
  /// \ru Получить указатель на кривую пересечения по индексу. \en Get a pointer to an intersection curve by the index. 
  const MbCurveEdge * GetCurve( size_t k ) const { return ((k < curves.Count()) ? curves[k] : NULL); } 
  /// \ru Получить номера касающихся граней первого/второго тела. \en Get numbers concerning faces of the first/second solid. 
        void          GetFaceNumbers( bool first, SArray<ptrdiff_t> & numbers ) const { first ? numbers = faceNumbers1 : numbers = faceNumbers2; } 

OBVIOUS_PRIVATE_COPY( MbIntersectionData ); // \ru Не реализовано \en Not implemented 
};


//------------------------------------------------------------------------------
/// \ru  (lenEps - ) \en  (lenEps is an ) 
/** \brief \ru Проверка вырожденности кривой в трехмерном пространстве.
           \en Check for the curve degeneration in three-dimensional space. \~
  \details \ru Проверка вырожденности кривой в трехмерном пространстве. \n
           \en Check for the curve degeneration in three-dimensional space. \n \~
  \param[in] curve - \ru Кривая.
                     \en Curve. \~
  \param[in] eps - \ru Неразличимая метрическая область.
                   \en Indistinguishable metric domain. \~
  \return \ru Возвращает состояние вырожденности кривой.
          \en Returns the state of the curve degeneration. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) IsDegeneratedCurve( const MbCurve3D & curve, double eps );


//------------------------------------------------------------------------------
/// \ru Проверка на полное совпадение двух кривых пересечения поверхностей c метрической точностью lenEps \en Check for complete coincidence of two intersection curves of surfaces with metric tolerance lenEps 
//---
bool IsCoincidentCurves( const MbSurfaceIntersectionCurve & intCurve1,
                         const MbSurfaceIntersectionCurve & intCurve2,
                         double lenEps );


//------------------------------------------------------------------------------
/** \brief \ru Проверка оболочки тела на замкнутость.
           \en Check of solid's shell for closedness. \~
  \details \ru Проверка оболочки тела на замкнутость. \n
           \en Check of solid's shell for closedness. \n \~
  \param[in] shell - \ru Оболочка.
                     \en A shell. \~
  \param[in] checkChangedOnly - \ru Проверять только измененные грани оболочки.
                                \en Only modified faces of a shell are to be checked. \~
  \return \ru Возвращает состояние замкнутости оболочки.
          \en Returns the state of shell closedness. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckShellClosure( const MbFaceShell & shell, bool checkChangedOnly = false );


//------------------------------------------------------------------------------
/** \brief \ru Проверка оболочки тела на замкнутость.
           \en Check of solid's shell for closedness. \~
  \details \ru Проверка оболочки тела на замкнутость. \n
           \en Check of solid's shell for closedness. \n \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckSolidClosure( const MbSolid & solid );


//------------------------------------------------------------------------------
// \ru Обнаружение самопересечений в геометрии модели тела \en Detection of self-intersections in solid's model geometry 
// \ru Алгоритм ориентирован на работу с произвольной моделью тела. \en The algorithm is intended for working with arbitrary model of a solid. 
//---
// MbResultType CheckSelfIntersectGeometry( const MbFaceShell & shell, bool checkAll );


////////////////////////////////////////////////////////////////////////////////
//
// \ru Функции для проверки элементов оболочки \en Functions for checking shell's elements 
//
////////////////////////////////////////////////////////////////////////////////


//------------------------------------------------------------------------------
/** \brief \ru Поиск краевых ребер замкнутой оболочки.
           \en Search for the boundary edges of a closed shell. \~
  \details \ru Поиск краевых ребер замкнутой оболочки. \n
           \en Search for the boundary edges of a closed shell. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] boundaryEdges - \ru Множество для краевых ребер.
                             \en Set of boundary edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно краевое ребро.
          \en Returns true if at least one boundary edges is found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CheckBoundaryEdges( const RPArray<MbCurveEdge> & allEdges,
                                     RPArray<MbCurveEdge> * boundaryEdges );

//------------------------------------------------------------------------------
/** \brief \ru Поиск некорректных ребер.
           \en Search of incorrect edges. \~
  \details \ru Поиск некорректных ребер. Не ищет краевые ребра замкнутой оболочки. \n
    Для поиска краевых ребер используйте функцию CheckBoundaryEdges. \n
           \en Search of incorrect edges. Does not look for the boundary edges of a closed shell. \n
    Use function CheckBoundaryEdges for searching for boundary edges. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] badEdges - \ru Множество для некорректных ребер.
                        \en Set of incorrect edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно некорректное ребро.
          \en Returns true if at least one incorrect edge is found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CheckBadEdges( const RPArray<MbCurveEdge> & allEdges,
                                RPArray<MbCurveEdge> * badEdges );

//------------------------------------------------------------------------------
/** \brief \ru Поиск неточных вершин.
           \en Search for inexact vertices. \~
  \details \ru Поиск неточных вершин оболочки. \n
           \en Search for inexact vertices of a shell. \n \~
  \param[in] vertArr - \ru Множество вершин оболочки.
                       \en Set of shell's vertices. \~
  \param[in] mMaxAcc - \ru Порог отбора неточных вершин.
                       \en Accuracy of inexact vertices filtration. \~
  \param[in] inexactVerts - \ru Множество для неточных вершин.
                            \en Set of inexact vertices. \~
  \return \ru Возвращает true, если найдена хотя бы одна неточная вершина.
          \en Returns true if at least one inexact vertex is found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CheckInexactVertices( const RPArray<MbVertex> & vertArr, double mMaxAcc,
                                       RPArray<MbVertex> * inexactVerts );


//------------------------------------------------------------------------------
/** \brief \ru Является ли кривая пересечения ребра неточной.
           \en Is the curve of intersection edges inaccurate. \~
  \details \ru Является ли кривая пересечения ребра неточной (оценочно). \n
           \en Is the curve of intersection edges inaccurate (estimated). \ n \~
  \param[in] edge - \ru Ребро оболочки.
                    \en The edge of the shell. \~
  \param[in] mMaxAcc - \ru Порог отбора неточного ребра.
                       \en Accuracy selection inaccurate ribs. \~
  \return \ru Возвращает true, если ребро неточное.
          \en Returns true, if the edge is inaccurate. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) IsInexactEdge( const MbCurveEdge & edge, double mMaxAcc );


//------------------------------------------------------------------------------
/** \brief \ru Поиск неточных ребер оценочно.
           \en Approximate search of inexact edges. \~
  \details \ru Поиск неточных ребер оболочки оценочно. \n
           \en Approximate search of inexact edges of a shell. \n \~
  \param[in] allEdges - \ru Множество ребер оболочки.
                        \en Set of edges of a shell. \~
  \param[in] mMaxAcc - \ru Порог отбора неточных ребер.
                       \en Accuracy of inexact edges filtration. \~
  \param[in] inexactEdges - \ru Множество для неточных ребер.
                            \en Set of inexact edges. \~
  \return \ru Возвращает true, если найдено хотя бы одно неточное ребро.
          \en Returns true if at least one inexact edge is found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) CheckInexactEdges( const RPArray<MbCurveEdge> & allEdges, double mMaxAcc,
                                    RPArray<MbCurveEdge> * inexactEdges );

//------------------------------------------------------------------------------
/** \brief \ru Проверка подложек и указаний на грани.
           \en Check of substrates and pointers to faces. \~
  \details \ru Проверка подложек и указаний на грани оболочки. \n
           \en Check of substrates and pointers to faces of a shell. \n \~
  \param[in] shell - \ru Проверяемая оболочка.
                     \en A shell to check. \~
  \param[out] areIdenticalBaseSurfaces - \ru Наличие общих подложек.
                                         \en Whether there are common substrates. \~
  \param[out] areBadFacePointers - \ru Наличие неверных указателей на соседние грани.
                                   \en Whether there are invalid pointers to neighboring faces. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (void) CheckBadFaces( const MbFaceShell & shell,
                                bool & areIdenticalBaseSurfaces,
                                bool & areBadFacePointers );


//------------------------------------------------------------------------------
/** \brief \ru Проверка взаимного расположения циклов грани.
           \en Check interposition of face loops. \~
  \details \ru Проверка взаимного расположения циклов грани. 
           \en Check interposition of face loops. \n \~
  \param[in] face - \ru Грань.
                    \en Face. \~
  \return \ru Возвращает true, если расположение и ориентация циклов корректны.
          \en Returns true if interposition of loops and their orientations are correct. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckLoopsInterposition( const MbFace & face );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
           Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
           \en Check for connectivity of loop edges.
           Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n \~
  \param[in] surf - \ru Базовая поверхность грани, содержащей проверяемый цикл.
                    \en The base surface of a face that contains the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \param[out] badConnectedEdges - \ru Ребра с плохой связностью.
                                  \en Edges with bad connectivity. \~
  \param[out] badVertexEdges - \ru Ребра с неправильными вершинами.
                               \en Edges with incorrect vertices. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckLoopConnection( const MbSurface & surf, bool faceSense, const MbLoop & loop, 
                                      double & lengthTolerance, double * paramTolerance,
                                      RPArray<const MbOrientedEdge> * badConnectedEdges = NULL,
                                      RPArray<const MbCurveEdge> * badVertexEdges = NULL );

//------------------------------------------------------------------------------
/** \brief \ru Проверка связности ребер цикла.
           \en Check for connectivity of a loop edges. \~
  \details \ru Проверка связности ребер цикла грани.
           Возвращает максимальные метрическую и параметрическую (опционально) погрешности построения цикла. \n
           \en Check for connectivity of a loop edges.
           Returns the maximal metric and parametric (optionally) tolerances of the loop construction. \n \~
  \param[in] face - \ru Грань, содержащая проверяемый цикл.
                    \en Face containing the loop under test. \~
  \param[in] loop - \ru Цикл грани.
                    \en Face loop. \~
  \param[out] lengthTolerance - \ru Максимальное метрическое значение разрыва между ребрами.
                                \en The maximal metric value of a gap between edges. \~
  \param[out] paramTolerance  - \ru Максимальное параметрическое значение разрыва между ребрами.
                                \en The maximal parametric value of a gap between edges. \~
  \param[out] badConnectedEdges - \ru Ребра с плохой связностью.
                                  \en Edges with bad connectivity. \~
  \param[out] badVertexEdges - \ru Ребра с неправильными вершинами.
                               \en Edges with incorrect vertices. \~
  \return \ru Возвращает true, если связность ребер не нарушена.
          \en Returns true if the connectivity is good. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckLoopConnection( const MbFace & face, const MbLoop & loop, 
                                      double & lengthTolerance, double * paramTolerance,
                                      RPArray<const MbOrientedEdge> * badConnectedEdges = NULL,
                                      RPArray<const MbCurveEdge> * badVertexEdges = NULL );

//------------------------------------------------------------------------------
/** \brief \ru Найти циклы грани с самопересечениями.
           \en Find face loops with self-intersections. \~
  \details \ru Найти циклы грани с самопересечениями.
           Возвращает найденные циклы с самопересечениям. \n
           \en Find face loops with self-intersections.
           Returns the found loops with self-intersections. \n \~
  \param[in] face - \ru Грань, содержащая проверяемые циклы.
                    \en Face containing loops under test. \~
  \param[in] nameMaker - \ru Именователь.
                         \en An object for naming the new objects. \~
  \param[in] checkInsideEdges - \ru Искать самопересечения внутри области определения двумерных кривых ребер.
                                \en Find edges with self-intersections inside. \~
  \param[out] loopPnts - \ru Точки самопересечения c номерами циклов.
                         \en Points of self-intersecting loops and the numbers of loops. \~
  \return \ru Возвращает true, если найдены самопересечения циклов.
          \en Returns true if the self-intersection has been found. \~
  \ingroup Algorithms_3D
*/
// ---
MATH_FUNC (bool) FindLoopsSelfIntersections( const MbFace & face, const MbSNameMaker & nameMaker, bool checkInsideEdges,
                                             std::vector< std::pair<c3d::NumberPair, MbCartPoint3D> > * loopPnts );


//------------------------------------------------------------------------------
/** \brief \ru Проверка связности граней faces.
           \en Check for connectivity of faces 'faces'. \~
  \details \ru Проверка топологической связности граней faces. \n
           \en Check for topological connectivity of faces 'faces'. \n \~
  \param[in] faces - \ru Проверяемый набор граней.
                     \en Set of faces under check. \~
  \return \ru Возвращает true, все грани топологически связаны.
          \en Returns true if all the faces are topologically connected. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) CheckFacesConnection( const RPArray<MbFace> & faces );


//------------------------------------------------------------------------------
/** \brief \ru Найти в исходной оболочке "родительские" грани производной оболочки.
           \en Find "parent" faces of a derived shell in the initial shell. \~
  \details \ru Найти в исходной оболочке "родительские" грани производной оболочки
    геометрическим поиском подобных граней с наложением.
    Флаг sameNormals установить false, если исходная оболочка участвовала в булевом вычитании тел вторым операндом. \n
           \en Find "parent" faces of a derived shell in the initial shell
    by geometric search of similar faces with overlapping.
    Flag sameNormals is to be set to false if the initial shell was involved in the boolean subtraction of solids as a second operand. \n \~
  \param[in] srcShell - \ru Исходная оболочка.
                        \en The source shell. \~
  \param[in] dstShell - \ru Производная оболочка.
                        \en The derived shell. \~
  \param[in] sameNormals - \ru Искать с одинаковым (true) или противоположным (false) направлением нормалей.
                           \en Search with the same (true) or the opposite (false) direction of normals. \~
  \param[out] simPairs - \ru Множество соответствий - номеров граней в исходной и производной оболочках.
                         \en Set of correspondences - indices of faces in the initial and the derived shells. \~
  \return \ru Возвращает true, все найдено хоть одно соответствие.
          \en Returns true if at least one correspondence is found. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) FindOverlappingSimilarFaces( const MbFaceShell & srcShell, const MbFaceShell & dstShell, bool sameNormals,
                                              std::vector<c3d::NumberPair> & simPairs );


//------------------------------------------------------------------------------
/** \brief \ru Найти на каких гранях исходной оболочки базируются ребра производной оболочки.
           \en Find faces edges of the derived shell are based on. \~
  \details \ru Найти на каких гранях исходной оболочки базируются ребра производной оболочки геометрическим поиском. 
    Поиск соответствия проводится по поверхностям из граней, на которые ссылается ребро, а не по поверхностям в кривой пересечения ребра. 
    Флаг sameNormals установить false, если исходная оболочка участвовала в булевом вычитании тел вторым операндом. \n
           \en Determine on which faces of the initial shell edges of the derived shell are based on by the geometric search. 
    Search of the correspondence is performed by surfaces from faces the edge refers to, but not by surfaces from the intersection curve of the edge. 
    Flag sameNormals is to be set to false if the initial shell was involved in the boolean subtraction of solids as a second operand. \n \~
  \param[in] edges - \ru Ребра производной оболочки.
                     \en Edges of an arbitrary shell. \~
  \param[in] shell - \ru Исходная оболочка.
                     \en The source shell. \~
  \param[in] sameNormals - \ru Искать с одинаковым (true) или противоположным (false) направлением нормалей.
                           \en Search with the same (true) or the opposite (false) direction of normals. \~
  \param[out] efPairs - \ru Множество соответствий - номеров ребер во входном массиве и номеров граней в исходной оболочке.
                        \en Set of correspondence - indices of edges in the input array and numbers of faces in the input shell. \~
  \return \ru Возвращает true, все найдено хоть одно соответствие.
          \en Returns true if at least one correspondence is found. \~
  \ingroup Algorithms_3D
*/
//---
MATH_FUNC (bool) FindFacesEdgesCarriers( const c3d::ConstEdgesVector & edges, const MbFaceShell & shell, bool sameNormals,
                                         std::vector<c3d::NumberPair> & efPairs );


#endif // __CHECK_GEOMETRY_H
