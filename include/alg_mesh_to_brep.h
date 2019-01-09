////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief Функции преобразования полигональной модели в граничное представление.

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ALG_MESH_TO_BREP_H
#define __ALG_MESH_TO_BREP_H

#include <math_define.h>
#include <mb_variables.h>
#include <mb_operation_result.h>
#include <templ_sptr.h>
#include <vector>
#include <utility>


class  MATH_CLASS MbCartPoint3D;
class  MATH_CLASS MbVector3D;
class  MATH_CLASS MbFaceShell;
class  MATH_CLASS MbFace;
class  MATH_CLASS MbLoop;
class  MATH_CLASS MbOrientedEdge;
class  MATH_CLASS MbCurveEdge;
class  MATH_CLASS MbMesh;
class  MATH_CLASS MbGrid;
class  MATH_CLASS MbCollection;
class  MATH_CLASS MbSNameMaker;
class  MATH_CLASS MbTriangle;
class  MATH_CLASS IProgressIndicator;
class  MATH_CLASS ProgressBarWrapper;
struct MATH_CLASS GridsToShellValues;


//------------------------------------------------------------------------------
// Удалить лишние (дублирующие) точки.
// ---
bool RemoveRedundantPoints( std::vector<MbCartPoint3D> & points, 
                            std::vector<MbTriangle> & triangles, 
                            ProgressBarWrapper * baseProgBar );

//------------------------------------------------------------------------------
// Удалить лишние (дублирующие) точки.
// ---
bool RemoveRedundantPoints( std::vector< std::pair<MbCartPoint3D,MbVector3D> > & pointNormals, 
                            std::vector<MbTriangle> & triangles, 
                            ProgressBarWrapper * baseProgBar );

//------------------------------------------------------------------------------
// Удалить лишние (дублирующие) точки.
// ---
bool RemoveRedundantPoints( std::vector<MbCartPoint3D> & points, 
                            std::vector<uint> & indexes );

//------------------------------------------------------------------------------
// Объединить ребра двух смежных плоских граней с полигональной границей
// (возвращает общее после сшивки ребро)
// ---
MbCurveEdge * StitchAdjacentGridsEdges( MbFace & face1, MbOrientedEdge & edge1,
                                        MbFace & face2, MbLoop & loop2, size_t e2Ind );

//------------------------------------------------------------------------------
// Обеспечить связность треугольных граней
// ---
bool ConnectTriangleFaces( const std::vector< SPtr<MbFace> >                               & faces,
                           const std::vector< std::pair<c3d::NumberPair,c3d::NumberPair> > & edgesPairs,
                                 std::vector< std::pair<c3d::NumberPair,c3d::NumberPair> > * combinedPairs,
                                 ProgressBarWrapper                                        * baseProgBar );

//------------------------------------------------------------------------------
// Преобразовать триангуляцию в оболочку.
// ---
MbFaceShell * ConvertGridToShell( const MbGrid & grid, const GridsToShellValues & params, const MbSNameMaker & snMaker, 
                                  MbResultType & res, IProgressIndicator * progBar = NULL );


//------------------------------------------------------------------------------
// Преобразовать полигональную модель в оболочку.
// ---
MbFaceShell * ConvertMeshToShell( const MbMesh & mesh, const GridsToShellValues & params, const MbSNameMaker & snMaker, 
                                  MbResultType & res, IProgressIndicator * progBar = NULL );


//------------------------------------------------------------------------------
// Преобразовать триангуляцию в оболочку.
// ---
MbFaceShell * ConvertCollectionToShell( const MbCollection & grid, 
                                        bool mergeFaces,
                                        const MbSNameMaker & snMaker, 
                                        MbResultType & res, 
                                        IProgressIndicator * progIndicator );


#endif // __ALG_UTILITES_H
