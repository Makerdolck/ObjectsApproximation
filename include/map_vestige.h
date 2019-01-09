////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Cледы трехмерных объектов.
         \en Vestiges of three-dimensional objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MAP_VESTIGE_H
#define __MAP_VESTIGE_H


#include <templ_p_array.h>
#include <templ_pointer.h>
#include <templ_sptr.h>
#include <mb_cart_point.h>
#include <mb_matrix.h>
#include <curve.h>
#include <topology_item.h>

class MbEdgeVestige;
class MbVertexVestige;


//------------------------------------------------------------------------------     
/** \brief \ru След трехмерного объекта.
           \en The vestige of three-dimensional object. \~
  \details \ru Базовый класс для классов следа трехмерного объекта.\n
           \en The base class for classes of three-dimensional object vestige.\n \~
  \ingroup Mapping
*/ 
// ---    
class MATH_CLASS MbBaseVestige : public TapeBase {     
protected:
  uint                      comp;       ///< \ru Компонент. \en A component. 
  size_t                    ident;      ///< \ru Идентификатор нити. \en A thread identifier. 
  uint16                    style;      ///< \ru Базовый стиль. \en A basis style.  \~ \internal \ru По просьбе группы 3D (Компас) \en 3D (Kompas) at request. \~ \endinternal
  MbAttributeContainer      attrData;   ///< \ru Атрибуты. \en Attributes.  \~ \internal \ru По просьбе группы Приложений (Компас) \en Apps (Kompas) at request. \~ \endinternal
protected:
  const MbTopologyItem *    item;       ///< \ru Топологический объект(используется только как временный внутри проецирования). \en Topological object (is used only as a temporary object in projection). 
  TOwnPointer<const MbName> name;       ///< \ru Имя. \en A name. 

public:
  
  /** \brief \ru Классификация плоского отображения.
             \en The classification of a planar mapping. \~
    \details \ru Классификация плоского отображения. \n
             \en The classification of a planar mapping. \n \~  
  */
  enum Type {        
    vt_None,            ///< \ru Тип неопределен. \en A type is undefined. 
    vt_SmoothEdge,      ///< \ru Линия перехода (гладкое ребро). \en A transition line (smooth edge). 
    vt_Edge,            ///< \ru Отображение ребра или линия очерка поверхности. \en Mapping of edge or isocline curve of surface. 
    vt_SectionLine,     ///< \ru Линия разреза. \en A cutaway line. 
    vt_AnnThreadThin,   ///< \ru Аннотационный объект резьба тонкая. \en An annotative object - a thread is thin. 
    vt_AnnThreadThick,  ///< \ru Аннотационный объект резьба толстая. \en An annotative object - a thread is thick. 
    vt_AnnThreadDashed, ///< \ru Аннотационный объект резьба штриховая. \en An annotative object - a thread is dashed. 
    vt_BoundLeft,       ///< \ru Левая граница вида. \en The left boundary of a view. 
    vt_BoundRight,      ///< \ru Правая граница вида. \en The right boundary of a view. 
    vt_Vertex,          ///< \ru Вершина. \en A vertex. 
    vt_SpacePoint,      ///< \ru Пространственная точка. \en A spatial point. 
    vt_SpaceCurve,      ///< \ru Пространственная кривая. \en A spatial curve. 
  };

  /** \brief \ru Классификатор подтипов аннотационных ребер.
             \en The classifier of annotative edges subtypes. \~
    \details \ru Классификатор подтипов аннотационных ребер. \n
             \en The classifier of annotative edges subtypes. \n \~  
  */
  enum SubType { 
    vst_None = 0, ///< \ru Подтип неопределен. \en A subtype is undefined. 
    vst_BaseBeg,  ///< \ru Основной начальный. \en A base initial type. 
    vst_BaseEnd,  ///< \ru Основной конечный. \en A base final type. 
    vst_ButtBeg,  ///< \ru Торцевой начальный. \en A butt initial type. 
    vst_ButtEnd,  ///< \ru Торцевой конечный. \en A butt final type. 
  };

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для топологического объекта.\n
             \en Constructor for a topological object.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem ) 
    : comp     ( otherComp            )
    , ident    ( otherIdent           )
    , style    ( SYS_MAX_UINT16       )
    , attrData (                      )
    , item     ( &otherItem           )
    , name     ( &otherItem.GetName() )
  { name.SetOwn( false ); }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для аннотационного объекта.\n
             \en Constructor for an annotative object.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbName & otherName, const MbTopologyItem * otherItem ) 
    : comp     ( otherComp      )
    , ident    ( otherIdent     )
    , style    ( SYS_MAX_UINT16 )
    , attrData (                )
    , item     ( otherItem      )
    , name     ( &otherName     )
  { name.SetOwn( false ); }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для пространственной точки или кривой.\n
             \en Constructor for a spatial point or curve.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  */
  MbBaseVestige( uint otherComp, size_t otherIdent, const MbName & otherName ) 
    : comp     ( otherComp      )
    , ident    ( otherIdent     )
    , style    ( SYS_MAX_UINT16 )
    , attrData (                )
    , item     ( NULL           )
    , name     ( &otherName     )
  { name.SetOwn( false ); }
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbBaseVestige( const MbBaseVestige & other, MbRegDuplicate * iReg );
  /// \ru Конструктор. \en Constructor. 
  MbBaseVestige()
    : comp    ( 0              )
    , ident   ( SYS_MAX_T      )
    , style   ( SYS_MAX_UINT16 )
    , item    ( NULL           )
    , name    ( NULL           )
  { name.SetOwn(false); }

public:
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const; ///< \ru Создать копию объекта. \en Create a copy of the object. 
public:
        uint                    GetComponent() const { return comp; }
        size_t                  GetIdentifier() const { return ident; }
        uint16                  GetStyle() const { return style; }
  const MbAttributeContainer &  GetAttributes() const { return attrData; }

        void                    CopyIdData( const MbBaseVestige & obj ) { comp = obj.comp; ident = obj.ident; }

        void                    SetProperties ( uint16 st                                  ) { style = st; }
        void                    SetProperties ( uint16 st, const MbAttributeContainer & ac ) { style = st; attrData.AttributesAssign( ac ); }
        void                    CopyProperties( const MbBaseVestige & obj )                  { style = obj.style; attrData.AttributesAssign( obj.attrData ); }
        
public:
  const MbTopologyItem *        GetItem() const { return item; }                 ///< \ru Топологический объект. \en A topological object. 
  const MbName *                GetVestigeName() const { return name; }          ///< \ru Имя. \en A name.   

private:
  bool operator == ( const MbBaseVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbBaseVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbBaseVestige )
OBVIOUS_PRIVATE_COPY( MbBaseVestige )
};
  

//------------------------------------------------------------------------------     
/** \brief \ru След вершины.
           \en The vestige of a vertex. \~
  \details \ru След вершины.\n
           \en The vestige of a vertex.\n \~
  \ingroup Mapping
*/ 
// --- 
class MATH_CLASS MbVertexVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
protected:
  MbCartPoint point;    ///< \ru Проекция вершины. \en A vertex projection. 
  bool        bvisible; ///< \ru Флаг видимости. \en A visibility flag. 
private:
  uint8       vesType;  ///< \ru Тип вершины. \en A vertex type. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор вершины.\n
             \en Constructor of a vertex.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool vis )
    : MbBaseVestige( otherComp, otherIdent, otherItem )
    , point   (           )
    , bvisible( vis       )
    , vesType ( vt_Vertex )
  {}
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор пространственной точки.\n
             \en Constructor of a spatial point.\n \~    
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~ 
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige( uint otherComp, size_t otherIdent, const MbName & otherName, bool vis )
    : MbBaseVestige( otherComp, otherIdent, otherName )
    , point   (               )
    , bvisible( vis           )
    , vesType ( vt_SpacePoint )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbVertexVestige( const MbVertexVestige & other, MbRegDuplicate * iReg )
    : MbBaseVestige( other, iReg    )
    , point        ( other.point    ) 
    , bvisible     ( other.bvisible ) 
    , vesType      ( other.vesType  ) 
  {}
  /// \ru Конструктор. \en Constructor. 
  MbVertexVestige()
    : MbBaseVestige()
    , point   (         )
    , bvisible( true    )
    , vesType ( vt_None )
  {}
public:
  /// \ru Создать копию объекта. \en Create a copy of the object. 
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;
public:
  /// \ru Тип отображения. \en Mapping type. 
        Type              GetType() const { return (Type)vesType; }
  /// \ru Это видимая точка? \en Is point visible? 
        bool              IsVisible() const { return bvisible; }
  /// \ru Получить точку. \en Get the point. 
  const MbCartPoint &     GetPoint() const { return point; }
  /// \ru Преобразовать точку по матрице. \en Transform the point. 
        void              TransformPoint( const MbMatrix & mtr ) { point.Transform( mtr ); }

private:
  bool operator == ( const MbVertexVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbVertexVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbVertexVestige )
OBVIOUS_PRIVATE_COPY( MbVertexVestige )
};


//------------------------------------------------------------------------------     
/** \brief \ru Информация о следе кривой.
           \en The information about a curve vestige. \~
  \details \ru Информация о следе кривой.\n 
    Для вложения в след ребра (MbVestigeEdge) и в след грани (MbVestigeEdge).\n
           \en The information about a curve vestige.\n 
    For including to the edge vestige (MbVestigeEdge) and face vestige (MbVestigeEdge).\n \~
  \ingroup Mapping
*/ 
// --- 
struct MATH_CLASS MbCurveVestige : public TapeBase {
  SPtr<MbCurve>               totalPrj;  ///< \ru Полная проекция (может быть NULL). \en A full projection (can be NULL). \~  \internal \ru Владеет. \en Owns. \~    \endinternal                                       
  PArray<MbCurve>             arTotal;   ///< \ru Все проекции в упорядоченной форме. \en All projections in an ordered form. \~ \internal \ru Не владеет. \en Doesn't own. \~ \endinternal                                       
  
  // \ru Двумерные кривые лежат копиями поэтому массивы владеющие \en Two-dimensional uv-curves are copies therefore arrays are owners 
  // \ru Если нет то указатель останется нулевым \en If no - the pointer remains NULL 
  TPointer<PArray<MbCurve> >  arVisPrj; ///< \ru Видимые проекции. \en Visible projections. 
  TPointer<PArray<MbCurve> >  arHidPrj; ///< \ru Не видимые проекции. \en Invisible projections. 

public:

  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор по умолчанию.\n        
      Создает объект с нулевой проекцией.
             \en Default constructor.\n        
      Creates an object with the null projection. \~ 
  */
  MbCurveVestige()
    : totalPrj( NULL )
    , arTotal ( 0, 1, false )
    , arVisPrj( NULL )
    , arHidPrj( NULL )
  {}

  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbCurveVestige( const MbCurveVestige & other, MbRegDuplicate * iReg ); 

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbCurveVestige()
  {
    ClearAll();
  }  

  /// \ru Создать копию объекта. \en Create a copy of the object. 
  virtual MbCurveVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;    

  /** \brief \ru Очистить проекции.
             \en Clear projections. \~
    \details \ru Очистить проекции.\n        
      Чистит список проекций в arTotal, 
      обнуляет указатели totalPrj, arVisPrj, arHidPrj.
             \en Clear projections.\n        
      Clears the list of projections in the arToral, 
      resets the totalPrj, arVisPrj, arHidPrj. \~ 
  */
  void ClearAll()
  {
    arTotal.HardFlush();
    totalPrj = NULL;
    arVisPrj = NULL;
    arHidPrj = NULL;
  }

  /** \brief \ru Добавить часть проекции.
             \en Add a part of projection. \~
    \details \ru Добавить новую часть общей проекции, имеющий признак видимости.\n
             \en Add a new part of the general projection which has a visibility attribute.\n \~        
    \param[in] segment - \ru Часть проекции.
                         \en A projection part. \~
    \param[in] visible - \ru Признак видимости.
                         \en A visibility attribute. \~
  */
  void        AddSegment( MbCurve & segment, bool visible );  
  
  /// \ru Обнулить указатель на полную проекцию. \en Set to null the pointer to a full projection. 
  void        ClearFullProjection();                            
  
  /** \brief \ru Поглотить данные.
             \en Absorb data. \~
    \details \ru Поглотить данные структуры otherDS и очистить ее.
             \en Absorb data of the otherDS structure and clear it. \~       
    \param[in] otherDS - \ru Источник данных.
                         \en Data source. \~         
  */
  bool        EatupOtherStructure( MbCurveVestige & otherDS ); 

  /// \ru Cвойство пустого отображения. \en The property of an empty mapping. 
  bool        IsEmpty() const
  {
    return (totalPrj == NULL)                 && 
           (arTotal.Count() < 1)              && 
           ( !arVisPrj || arVisPrj->empty() ) && 
           ( !arHidPrj || arHidPrj->empty() );
  }  

  /// \ru Указатель на полную проекцию. \en The pointer to a full projection. 
  MbCurve *   GetFullProjection() const;
  /// \ru Установить полную проекцию. \en Set a full projection. 
  void        SetTotalProjection( MbCurve & curve ); 

private:
  bool operator == ( const MbCurveVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbCurveVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL ( MbCurveVestige )
OBVIOUS_PRIVATE_COPY( MbCurveVestige )
};

    
//------------------------------------------------------------------------------     
/** \brief \ru След ребра или кривой.
           \en The vestige of an edge or a curve. \~
  \details \ru След ребра или кривой.
    Несет в себе информацию о следе одной кривой.\n
           \en The vestige of an edge or a curve.
    Carries the information about the vestige of one curve.\n \~
    \internal \ru МА -  На самом деле след не ребра, 
               а единичное отображение топологического объекта\n
               (как правило ребра)
              \en МА -  it is not an edge vestige, 
               it is unit mapping of a topological object\n
               (of edge usually) \~ \endinternal
  \ingroup Mapping
*/     
// ---      
class MATH_CLASS MbEdgeVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:
  MbCurveVestige curveInfo; ///< \ru Информация о следе кривой. \en The information about a curve vestige. 
private:
  uint8          vesType;   ///< \ru Тип отображения. \en A mapping type. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для ребра.\n
             \en Constructor for an edge.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] isSmooth   - \ru Тип ребра.
                            \en A type of edge. \~
  */
  explicit
  MbEdgeVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection )
    : MbBaseVestige( otherComp, otherIdent, otherItem            )
    , curveInfo    (                                             )
    , vesType      ( (uint8)(isSmooth ? vt_SmoothEdge : vt_Edge) ) 
  {
    if ( isSection )
      vesType = (uint8)vt_SectionLine;
  }
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор для пространственной точки или кривой.\n
             \en Constructor for a spatial point or curve.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя.
                            \en A name. \~
    \param[in] type       - \ru Тип ребра.
                            \en A type of edge. \~
  */
  explicit
  MbEdgeVestige( uint otherComp, size_t otherIdent, const MbName & otherName )
    : MbBaseVestige( otherComp, otherIdent, otherName )
    , curveInfo    (                                  )
    , vesType      ( (uint8)vt_SpaceCurve             ) 
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbEdgeVestige( const MbEdgeVestige & other, MbRegDuplicate * iReg )
    : MbBaseVestige( other, iReg           )
    , curveInfo    ( other.curveInfo, iReg ) 
    , vesType      ( other.vesType         )
  {}
  /// \ru Конструктор. \en Constructor. 
  MbEdgeVestige()
    : MbBaseVestige()
    , curveInfo()
    , vesType( vt_None )
  {}
public:
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;
public:
  /// \ru Тип отображения. \en Mapping type. 
  Type      GetType() const { return (Type)vesType; }
  void      SetType( Type vt ) { vesType = (uint8)vt; }
public:
  /// \ru Указатель на полную проекцию. \en The pointer to a full projection. 
  MbCurve * GetFullProjection() const { return curveInfo.GetFullProjection(); }
  /// \ru Обнулить указатель на полную проекцию. \en Set to null the pointer to a full projection. 
  void      ClearFullProjection() { curveInfo.ClearFullProjection(); }

  /// \ru Добавить MbEdgeVestige в массив. \en Add the MbEdgeVestige to an array. \~
  friend MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName, RPArray<MbEdgeVestige> & arr );

private:
  bool operator == ( const MbEdgeVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbEdgeVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbEdgeVestige )
OBVIOUS_PRIVATE_COPY( MbEdgeVestige )
};

//------------------------------------------------------------------------------     
/** \brief \ru Добавить MbEdgeVestige в массив.
           \en Add the MbEdgeVestige to an array. \~
  \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
           \en Add the MbEdgeVestige to an array by the private constructors. \n \~
  \param[in] otherComp  - \ru Компонент.
                          \en A component. \~
  \param[in] otherIdent - \ru Идентификатор нити.
                          \en A thread identifier. \~
  \param[in] otherName  - \ru Имя топологического объекта.
                          \en A name of a topological object. \~
  \param[in] type       - \ru Тип ребра.
                          \en A type of edge. \~
  \param[out] array     - \ru Массив ребер.
                          \en Array of edges. \~
  \ingroup Mapping
*/
// ---          
inline MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName,
                                        RPArray<MbEdgeVestige> & arr )
{
  MbEdgeVestige * vestige = new MbEdgeVestige( otherComp, otherIdent, otherName );
  if ( vestige ) {
    arr.Add( vestige );
  }
  return vestige;
}

          
//------------------------------------------------------------------------------     
/** \brief \ru След грани.
           \en The vestige of a face. \~
  \details \ru След грани.\n
    Несет в себе информацию о следах очерков этой грани.
           \en The vestige of a face.\n
    Carriers the information about the vestiges of this face outlines. \~
  \internal \ru МА -  На самом деле след не грани, 
                  а множественное отображение топологического объекта\n
                  (как правило грани или разбитого на части отображения ребра (при построении сечений) )
            \en МА -  it is not a vestige of a face, 
                  it is a multiple mapping of a topological object\n
                  (of a face or partitioned edge mapping usually) \~  \endinternal
  \ingroup Mapping
*/        
// ---          
class MATH_CLASS MbFaceVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:
  PArray<MbCurveVestige> curveInfos;  ///< \ru Информация о следах очерков этой грани. \en The information about vestiges of this face outlines. \~  \internal \ru Владеет. \en Owns. \~ \endinternal                                        
private:
  uint8                  vesType;     ///< \ru Тип отображения. \en A mapping type. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа грани.\n
             \en Constructor of a face vestige.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbFaceVestige( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem )
    : MbBaseVestige( otherComp, otherIdent, otherItem )
    , curveInfos   ( 0, 1, true )
    , vesType      ( vt_Edge    )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbFaceVestige( const MbFaceVestige & other, MbRegDuplicate * iReg );
public:
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;
public:
  /// \ru Тип отображения. \en Mapping type. 
  Type      GetType() const { return (Type)vesType; }

private:
  bool operator == ( const MbFaceVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbFaceVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbFaceVestige )
OBVIOUS_PRIVATE_COPY( MbFaceVestige )
};


//------------------------------------------------------------------------------     
/** \brief \ru След аннотационного объекта.
           \en The vestige of an annotative object. \~
  \details \ru След аннотационного объекта.\n
           \en The vestige of an annotative object.\n \~
  \ingroup Mapping
*/ 
// ---  
class MATH_CLASS MbAnnotationEdgeVestige : public MbBaseVestige {
  friend struct MbVEFVestiges;
public:  
  MbCurveVestige   curveInfo;  ///< \ru Информация о следе кривой. \en The information about a curve vestige. 
protected:
  uint8            vesType;    ///< \ru Тип отображения. \en A mapping type. 
  uint8            vesSubType; ///< \ru Подтип (для ветвления именования). \en A subtype for naming branching. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа аннотационного объекта.\n
             \en Constructor of an annotative object vestige.\n \~        
    \param[in] otherComp   - \ru Компонент.
                             \en A component. \~
    \param[in] otherIdent  - \ru Идентификатор нити.
                             \en A thread identifier. \~
    \param[in] otherItem   - \ru Топологический объект.
                             \en A topological object. \~
    \param[in] otherName   - \ru Имя.
                             \en A name. \~
    \param[in] type        - \ru Тип аннотационных ребер.
                             \en A type of annotative edges. \~
    \param[in] subType     - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
  */
  MbAnnotationEdgeVestige( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, 
                           Type type, SubType subType )
    : MbBaseVestige( otherComp, otherIdent, otherName, otherItem )
    , curveInfo    (                )
    , vesType      ( (uint8)type    )
    , vesSubType   ( (uint8)subType )
  {
  }
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbAnnotationEdgeVestige( const MbAnnotationEdgeVestige & other, MbRegDuplicate * iReg );

public:
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;

public:
  /// \ru Тип отображения. \en Mapping type. 
  Type        GetType()             const { return (Type)vesType;       }
  /// \ru Подтип аннотационных ребер. \en A subtype of annotative edges. 
  SubType     GetSubType()          const { return (SubType)vesSubType; }
  /// \ru Указатель на полную проекцию. \en The pointer to a full projection. 
  MbCurve *   GetFullProjection()   const { return curveInfo.GetFullProjection(); }
  /// \ru Обнулить указатель на полную проекцию. \en Set to null the pointer to a full projection. 
  void        ClearFullProjection()       { curveInfo.ClearFullProjection(); }

  /// \ru Добавить MbAnnotationEdgeVestige в массив. \en Add the MbAnnotationEdgeVestige to an array. \~
  friend MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                              const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType,
                                                              RPArray<MbAnnotationEdgeVestige> & array );

private:
  bool operator ==       ( const MbAnnotationEdgeVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator !=       ( const MbAnnotationEdgeVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbAnnotationEdgeVestige )
OBVIOUS_PRIVATE_COPY( MbAnnotationEdgeVestige )
};

//------------------------------------------------------------------------------
/** \brief \ru Добавить MbAnnotationEdgeVestige в массив.
           \en Add the MbAnnotationEdgeVestige to an array. \~
  \details \ru Добавить MbAnnotationEdgeVestige в массив через закрытые конструкторы. \n
           \en Add the MbAnnotationEdgeVestige to an array by the private constructors. \n \~
  \param[in] otherComp   - \ru Компонент.
                           \en A component. \~
  \param[in] otherIdent  - \ru Идентификатор нити.
                           \en A thread identifier. \~
  \param[in] otherItem   - \ru Топологический объект.
                           \en A topological object. \~
  \param[in] otherName   - \ru Имя топологического объекта.
                           \en A name of a topological object. \~
  \param[in] type        - \ru Тип аннотационных ребер.
                           \en A subtype of annotative edges. \~
  \param[in] subType     - \ru Подтип аннотационных ребер.
                           \en A subtype of annotative edges. \~
  \param[out] array      - \ru Массив аннотационных ребер.
                           \en Array of annotative edges. \~
  \ingroup Mapping
*/
// --- 
inline
MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                     const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType,
                                                     RPArray<MbAnnotationEdgeVestige> & array )
{
  MbAnnotationEdgeVestige * vestige = new MbAnnotationEdgeVestige( otherComp, otherIdent, otherItem, otherName, type, subType );
  if ( vestige ) {
    array.Add( vestige );
  }
  return vestige;
}


//------------------------------------------------------------------------------     
/** \brief \ru След условного обозначения.
           \en The vestige of a conventional notation. \~
  \details \ru След условного обозначения.\n
           \en The vestige of a conventional notation.\n \~
  \ingroup Mapping
*/ 
// ---   
class MATH_CLASS MbSymbolVestige : public MbBaseVestige { 
  friend struct MbVEFVestiges;
protected:
  bool                bvisible; ///< \ru Флаг видимости. \en A visibility flag. 
  TPointer<MbMatrix>  matrix;   ///< \ru Матрица трансформации плоскости сечения из внутреннего представления во внешнее. \en A matrix of transformation of a section plane from internal representation to external one. 

protected:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор следа условного обозначения с определенным признаком видимости.\n
             \en Constructor of a conventional notation vestige with a defined visibility attribute.\n \~        
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] otherName  - \ru Имя.
                            \en A name. \~   
    \param[in] _bvisible  - \ru Признак видимости.
                            \en A visibility attribute. \~
  */
  MbSymbolVestige( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible = true )
    : MbBaseVestige( otherComp, otherIdent, otherName, otherItem )
    , bvisible( _bvisible )
    , matrix  ( NULL      )
  {}
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbSymbolVestige( const MbSymbolVestige & other, MbRegDuplicate * iReg );
  /// \ru Конструктор. \en Constructor. 
  MbSymbolVestige()
    : MbBaseVestige()
    , bvisible( true )
    , matrix  ( NULL ) 
  {}

public:         
  virtual ~MbSymbolVestige() {}
  virtual MbBaseVestige & Duplicate( MbRegDuplicate * iReg = NULL ) const;

public:
  /// \ru Это видимая точка? \en Is point visible? 
  bool       IsVisible() const { return bvisible; }
  /// \ru Установить матрицу трансформации. \en Set a transformation matrix. 
  void       SetMatrix( const MbMatrix & initMatrix );
  /// \ru Матрица трансформации. \en Transformation matrix. 
  MbMatrix * GetMatrix() const { return matrix; }

private:
  bool operator == ( const MbSymbolVestige & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbSymbolVestige & ); // \ru Не реализован. \en Not implemented. 

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbSymbolVestige )
OBVIOUS_PRIVATE_COPY( MbSymbolVestige )
};


//------------------------------------------------------------------------------
// 
// ---
inline void MbSymbolVestige::SetMatrix( const MbMatrix & initMatrix )
{ 
  PRECONDITION( matrix == NULL );
  if ( matrix == NULL )
    matrix = new MbMatrix( initMatrix );
  else
    *matrix = initMatrix;
}


//------------------------------------------------------------------------------     
/** \brief \ru Набор массивов, выдаваемых наружу при получении всех линий.
           \en The set of arrays which are given after getting of all lines. \~
  \details \ru Набор массивов, выдаваемых наружу при получении всех линий тел(а) 
    с селектированием на видимые и невидимые.\n
           \en The set of arrays which are given after getting off all solid(s) lines 
    with the separation to visible and invisible.\n \~
  \ingroup Mapping
*/ 
// ---  
struct MATH_CLASS MbVEFVestiges {
//protected:
  PArray<MbVertexVestige>         vertexVestiges;   ///< \ru Следы вершин. \en Vestiges of vertices.
  PArray<MbEdgeVestige>           edgeVestiges;     ///< \ru Следы ребер. \en Vestiges of edges.
  PArray<MbFaceVestige>           faceVestiges;     ///< \ru Следы граней. \en Vestiges of faces.
  PArray<MbAnnotationEdgeVestige> annotateVestiges; ///< \ru Следы аннотационных объектов. \en Vestiges of annotative objects.
  PArray<MbSymbolVestige>         symbolVestiges;   ///< \ru Следы условного обозначения. \en Vestiges of a conventional notation. 
  PArray<MbVertexVestige>         pointVestiges;    ///< \ru Следы пространственных точек. \en Vestiges of spatial points. 
  PArray<MbEdgeVestige>           curveVestiges;    ///< \ru Следы пространственных кривых. \en Vestiges of spatial curves. 

public:
  /// \ru Конструктор. \en Constructor. 
  MbVEFVestiges()   
    : vertexVestiges  ( 0, 1, true ) 
    , edgeVestiges    ( 0, 1, true )
    , faceVestiges    ( 0, 1, true )
    , annotateVestiges( 0, 1, true )
    , symbolVestiges  ( 0, 1, true )
    , pointVestiges   ( 0, 1, true )
    , curveVestiges   ( 0, 1, true )
  {}      
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with the registrator. 
  MbVEFVestiges( const MbVEFVestiges & other, MbRegDuplicate * iReg );
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbVEFVestiges() {}

public:
  /// \ru Создать копию объекта. \en Create a copy of the object. 
  virtual MbVEFVestiges & Duplicate( MbRegDuplicate * iReg = NULL ) const;  
  /// \ru Очистить массивы следов. \en Clear arrays of vestiges. 
          void            SetEmpty() 
          { 
            vertexVestiges.Flush(); 
            edgeVestiges.Flush(); 
            faceVestiges.Flush();
            annotateVestiges.Flush(); 
            symbolVestiges.Flush(); 
            pointVestiges.Flush();
            curveVestiges.Flush();
          }
  /// \ru Освободить неиспользуемую память. \en Adjust memory. 
          void            Adjust()
          { 
            vertexVestiges.Adjust(); 
            edgeVestiges.Adjust(); 
            faceVestiges.Adjust();
            annotateVestiges.Adjust(); 
            symbolVestiges.Adjust(); 
            pointVestiges.Adjust();
            curveVestiges.Adjust();
          }
          bool            IsEmpty() const
          {
            return  (vertexVestiges.Count() < 1)   &&
                    (edgeVestiges.Count() < 1)     &&
                    (faceVestiges.Count() < 1)     &&
                    (annotateVestiges.Count() < 1) &&
                    (symbolVestiges.Count() < 1)   &&
                    (pointVestiges.Count() < 1)    &&
                    (curveVestiges.Count() < 1);
          }

public:
  /** \brief \ru Добавить MbVertexVestige в массив.
             \en Add the MbVertexVestige to an array. \~
    \details \ru Добавить MbVertexVestige в массив через закрытые конструкторы. \n
             \en Add the MbVertexVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] uv         - \ru Точка - след вершины.
                            \en Point - vertex vestige. \~
    \param[in] uv         - \ru Состояние видимости.
                            \en Visibility state. \~
  */
  MbVertexVestige * AddVestigeVertex( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, const MbCartPoint & uv, bool vis ); 

  /** \brief \ru Добавить MbVertexVestige в массив.
             \en Add the MbVertexVestige to an array. \~
    \details \ru Добавить MbVertexVestige в массив через закрытые конструкторы. \n
             \en Add the MbVertexVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
    \param[in] uv         - \ru Точка - след пространственной точки.
                            \en Point - vestige of a spatial point. \~
    \param[in] vis        - \ru Флаг видимости.
                            \en A visibility flag. \~
  */
  MbVertexVestige * AddVestigePoint( uint otherComp, size_t otherIdent, const MbName & otherName, const MbCartPoint & uv, bool vis ); 

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] isSmooth   - \ru Тип ребра (гладкое или нет).
                            \en A type of edge (smooth or not). \~
    \param[in] isSection  - \ru Ребро от сечения или разреза.
                            \en Edge of section. \~
  */
  MbEdgeVestige * AddVestigeEdge( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection );

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  */
  MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName );

  /** \brief \ru Добавить MbEdgeVestige в массив.
              \en Add the MbEdgeVestige to an array. \~
    \details \ru Добавить MbEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] mapCurves  - \ru Следы кривой.
                            \en Spatial curve vestiges. \~
    \param[in] visible    - \ru Состояние видимости.
                            \en A visibility state. \~
    \param[in] otherName  - \ru Имя топологического объекта.
                            \en A name of a topological object. \~
  */
  MbEdgeVestige * AddVestigeCurve( uint otherComp, size_t otherIdent, const RPArray<MbCurve> & mapCurves, bool visible, const MbName & otherName );

  /** \brief \ru Добавить MbFaceVestige в массив.
             \en Add the MbFaceVestige to an array. \~
    \details \ru Добавить MbFaceVestige в массив через закрытые конструкторы. \n
             \en Add the MbFaceVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
  */
  MbFaceVestige * AddVestigeFace( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem );

  /** \brief \ru Добавить MbAnnotationEdgeVestige в массив.
             \en Add the MbAnnotationEdgeVestige to an array. \~
    \details \ru Добавить MbAnnotationEdgeVestige в массив через закрытые конструкторы. \n
             \en Add the MbAnnotationEdgeVestige to an array by the private constructors. \n \~
    \param[in] otherComp   - \ru Компонент.
                             \en A component. \~
    \param[in] otherIdent  - \ru Идентификатор нити.
                             \en A thread identifier. \~
    \param[in] otherItem   - \ru Топологический объект.
                             \en A topological object. \~
    \param[in] otherName   - \ru Имя топологического объекта.
                             \en A name of a topological object. \~
    \param[in] type        - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
    \param[in] subType     - \ru Подтип аннотационных ребер.
                             \en A subtype of annotative edges. \~
  */
  MbAnnotationEdgeVestige *  AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                       const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType );

  /** \brief \ru Добавить MbSymbolVestige в массив.
             \en Add the MbSymbolVestige to an array. \~
    \details \ru Добавить MbSymbolVestige в массив через закрытые конструкторы. \n
             \en Add the MbSymbolVestige to an array by the private constructors. \n \~
    \param[in] otherComp  - \ru Компонент.
                            \en A component. \~
    \param[in] otherIdent - \ru Идентификатор нити.
                            \en A thread identifier. \~
    \param[in] otherItem  - \ru Топологический объект.
                            \en A topological object. \~
    \param[in] otherName  - \ru Имя.
                            \en A name. \~   
    \param[in] _bvisible  - \ru Признак видимости.
                            \en A visibility attribute. \~
  */
  MbSymbolVestige * AddVestigeSymbol( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible = true );

private:
  bool operator == ( const MbVEFVestiges & ); // \ru Не реализован. \en Not implemented. 
  bool operator != ( const MbVEFVestiges & ); // \ru Не реализован. \en Not implemented. 

DECLARE_NEW_DELETE_CLASS( MbVEFVestiges )
DECLARE_NEW_DELETE_CLASS_EX( MbVEFVestiges )
KNOWN_OBJECTS_RW_REF_OPERATORS_EX ( MbVEFVestiges, MATH_FUNC_EX ) // \ru Для работы со ссылками и объектами класса \en For working with references and objects of the class 
OBVIOUS_PRIVATE_COPY( MbVEFVestiges )
};


//------------------------------------------------------------------------------
//
// ---
inline MbVertexVestige * MbVEFVestiges::AddVestigeVertex( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, const MbCartPoint & uv, bool vis )
{
  MbVertexVestige * vestige = new MbVertexVestige( otherComp, otherIdent, otherItem, vis );
  if ( vestige ) {
    vestige->point = uv;
    vertexVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbVertexVestige * MbVEFVestiges::AddVestigePoint( uint otherComp, size_t otherIdent, const MbName & otherName, const MbCartPoint & uv, bool vis )
{
  MbVertexVestige * vestige = new MbVertexVestige( otherComp, otherIdent, otherName, vis );
  if ( vestige ) {
    vestige->point = uv;
    pointVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeEdge( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem, bool isSmooth, bool isSection ) 
{
  MbEdgeVestige * vestige = new MbEdgeVestige( otherComp, otherIdent, otherItem, isSmooth, isSection );
  if ( vestige ) {
    edgeVestiges.Add( vestige );
  }
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeCurve( uint otherComp, size_t otherIdent, const MbName & otherName ) {
  return ::AddVestigeCurve( otherComp, otherIdent, otherName, curveVestiges );
}

//------------------------------------------------------------------------------
//
// ---
inline MbEdgeVestige * MbVEFVestiges::AddVestigeCurve( uint otherComp, size_t otherIdent, const RPArray<MbCurve> & mapCurves, bool visible,
                                                       const MbName & otherName ) 
{
  MbEdgeVestige * ev = NULL;

  ev = ::AddVestigeCurve( otherComp, otherIdent, otherName, curveVestiges );
  // BUG_93683 ev = ::AddVestigeEdge( otherComp, otherIdent, otherName, MbBaseVestige::vt_Edge, edgeVestiges );

  if ( ev != NULL ) {
    MbCurveVestige & vc = ev->curveInfo;
    for ( size_t m = 0, mapCurvesCnt = mapCurves.Count(); m < mapCurvesCnt; m++ ) {
      MbCurve * mapCurve = mapCurves[m];
      if ( mapCurve != NULL )
        vc.AddSegment( *mapCurve, visible );
    }
  }

  return ev;
}

//------------------------------------------------------------------------------
//
// ---
inline MbFaceVestige * MbVEFVestiges::AddVestigeFace( uint otherComp, size_t otherIdent, const MbTopologyItem & otherItem ) 
{
  MbFaceVestige * vestige = new MbFaceVestige( otherComp, otherIdent, otherItem );
  if ( vestige )
    faceVestiges.Add( vestige );
  return vestige;
}

//------------------------------------------------------------------------------
//
// ---
inline
MbAnnotationEdgeVestige * MbVEFVestiges::AddVestigeAnnotationEdge( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, 
                                                                   const MbName & otherName, MbBaseVestige::Type type, MbBaseVestige::SubType subType )
{
  return ::AddVestigeAnnotationEdge( otherComp, otherIdent, otherItem, otherName, type, subType, annotateVestiges );
}

//------------------------------------------------------------------------------
//
// ---
inline MbSymbolVestige * MbVEFVestiges::AddVestigeSymbol( uint otherComp, size_t otherIdent, const MbTopologyItem * otherItem, const MbName & otherName, bool _bvisible )
{
  MbSymbolVestige * vestige = new MbSymbolVestige( otherComp, otherIdent, otherItem, otherName, _bvisible );
  if ( vestige )
    symbolVestiges.Add( vestige );
  return vestige;
}


//------------------------------------------------------------------------------
/** \brief \ru Доступ к элементу массива.
           \en The access to an array element. \~
  \details \ru Доступ к элементу массива по индексу.\n
           \en The access to an array element by an index.\n \~
  \param[in] i     - \ru Индекс.
                     \en An index. \~
  \param[in] array - \ru Множество следов вершин.
                     \en An array of vertices vestiges. \~
  \return \ru След вершины по индексу
          \en A vestige of a vertex by an index. \~
  \ingroup Mapping
*/ 
// --- 
inline MbVertexVestige & GetVertexI( size_t i, RPArray<MbVertexVestige> & array ) {
  return *array[i];
}


#endif // __MAP_VESTIGE_H
