//////////////////////////////////////////////////////////////////////////////////////////
/** 
  \file \brief  \ru Иерархическое представление тела (СД для геометрического поиска).
                \en Hierarchical representation of solid (ST for geometric search). \~
*/
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CDET_SOLID_H
#define __CDET_SOLID_H

#include <mt_ref_item.h>
#include <templ_sptr.h>
#include <templ_s_array.h>
#include <templ_fdp_array.h>
#include <mb_matrix3d.h>
#include <mb_placement3d.h>
#include <solid.h>
#include <tri_lump.h>
#include <cdet_data.h>


class             MbBoundVolumeTree;
class  MATH_CLASS MbGrid;

//----------------------------------------------------------------------------------------
/* 
  It is intended to internal use. Use datatype cdet_item instead this.
  \brief \ru Иерархическое представление тела.
         \en Hierarchical representation of solid. \~
*/
// ---
class MbHRepSolid: public MtRefItem
{
public:
  const cdet_app_item         myComp;   // \ru Деталь сборки (данные пользователя). \en The component, pointer to client app data.
  std::vector<MbFaceAndGrid>  myGrids;  // \ru Триангуляционная аппроксимация на каждую грань тела; \en Triangulation approximation on each face of the solid;     
  mutable size_t              myIndex;
  double                      gridSag;  // \ru Максимальный прогиб сетки; \en Maximum grid sag; 
  MbMatrix3D                  toW0;     // \ru Матрица трансформации в мировую С.К. в исходном состоянии; \en Matrix of transformation to the world coordinate system at initial state; 
  MbMatrix3D                  toW;      // \ru Матрица трансформации в мировую С.К.; \en Matrix of transformation to the world coordinate system;   
  ptrdiff_t                   label;    // \ru Маркировка; \en Label;  
  bool                        moved;    // \ru Маркировка смеженных компонент; \en Label of adjacent components;   

public:
  MbBoundVolumeTree * bvt;  // \ru Иерархическое дерево объемов. \en Bounding volume tree. 

public:
  MbHRepSolid( const std::vector<MbFaceAndGrid> & arGrid, const MbPlacement3D & place
             , double gridSag, const cdet_app_item & comp );
 ~MbHRepSolid();

public:
  void                SetGrid( size_t faceInd, const MbGrid & grid );
  const MbMatrix3D  & GetMatrixToWorld() const { return toW; }
  const MbMatrix3D  & GetMatrixToWorld0() const { return toW0; }
  /// \ru Соответствует ли структура присланному ламп. \en Whether a structure corresponds to the sent lump.
  bool                IsMatched( const MbLumpAndFaces & ) const;
  bool                IsMatched( const MbHRepSolid & ) const;
  bool                IsMoved() const { return moved; }
  void                SetMoved( bool move ) { moved = move; }
  void                SetPlacement ( const MbPlacement3D & newPos ) { newPos.GetMatrixFrom(toW); }
  void                GetPlacement ( MbPlacement3D & pos ) { pos.Init(toW); }

private:
  MbHRepSolid ( const MbHRepSolid & ); // not defined
  MbHRepSolid & operator = ( const MbHRepSolid & );  // not defined
}; // MbHRepSolid

#endif // __CDET_SOLID_H

// eof