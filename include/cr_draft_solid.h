////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Строитель оболочки с уклонёнными гранями.
         \en Constructor of a shell with drafted faces. 
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CR_DRAFT_SOLID_H
#define __CR_DRAFT_SOLID_H


#include <creator.h>


//------------------------------------------------------------------------------
/** \brief \ru Строитель оболочки с уклонёнными гранями.
           \en Constructor of a shell with drafted faces. \~
  \details \ru Строитель оболочки с уклонёнными гранями для создания литейных уклонов.\n
           \en Constructor of a shell with drafted faces for pattern drafts creation. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbDraftSolid: public MbCreator {
protected:
  double              angle;      ///< \ru Угол уклона. \en Draft angle. 
  SArray<ptrdiff_t>   faceNb;     ///< \ru Номера множества уклоняемых граней. \en Indices of faces to draft. 
  MbeFacePropagation  fp;         ///< \ru Признак захвата граней ( face propagation ). \en Flag of face propagation. 
  // \ru Атрибуты, определяющие направление тяги (pull direction) и нейтральную изолинию уклона. \en Attributes determining the pull direction and the neutral isoline of the draft. 
  MbPlacement3D     * np;         ///< \ru Нейтральная плоскость ( neutral plane )         ( не обязателен ). \en Neutral plane (optional). 
  ptrdiff_t           edgeNb;     ///< \ru Номер прямолинейного ребра, направляющего уклон ( не обязателен ). \en The index of straight edge specifying the draft (optional). 
  SArray<ptrdiff_t> * pl;         ///< \ru Линии разъема (ребра) ( parting line  )         ( не обязателен ). \en Parting lines (of edge) (optional). 
  bool                reverse;    ///< \ru Обратное направление тяги. \en Reverse pull direction. 
  bool                step;       ///< \ru Ступенчатый способ уклона. \en Stepwise method of draft.  

public:
  // \ru Конструктор уклона по известной нейтральной плоскости \en Constructor of drafting by the given neutral plane. 
  MbDraftSolid( const MbPlacement3D & np,
                double angle, 
                const SArray<ptrdiff_t> & nFace,
                MbeFacePropagation fp, 
                bool reverse, 
                const MbSNameMaker &n );
  // \ru Конструктор уклона по линии разъема \en Constructor of drafting by the parting line 
  MbDraftSolid( double angle, 
                const MbPlacement3D * np,
                ptrdiff_t edgeNb, 
                MbeFacePropagation fp, 
                const SArray<ptrdiff_t> & pl, 
                bool reverse, 
                bool step, 
                const MbSNameMaker & n );
private :
  MbDraftSolid( const MbDraftSolid & init, MbRegDuplicate * iReg );  // \ru Конструктор копирования \en Copy-constructor 
  // \ru Объявление конструктора копирования без реализации, чтобы не было копирования по умолчанию. \en Declaration without implementation of the copy-constructor to prevent copying by default. 
  MbDraftSolid( const MbDraftSolid & init ); 
public :
  virtual ~MbDraftSolid();

  // \ru Общие функции математического объекта \en Common functions of the mathematical object 

  virtual MbeCreatorType  IsA() const; // \ru Тип элемента \en A type of element 
  virtual MbCreator & Duplicate( MbRegDuplicate * = NULL ) const;
  virtual void        Transform( const MbMatrix3D & matr, MbRegTransform * = NULL );               // \ru Преобразовать элемент согласно матрице \en Transform element according to the matrix 
  virtual void        Move     ( const MbVector3D & to, MbRegTransform * = NULL   );               // \ru Сдвиг \en Translation 
  virtual void        Rotate   ( const MbAxis3D   & axis, double angle, MbRegTransform * = NULL ); // \ru Повернуть вокруг оси \en Rotate around an axis 

  virtual void        GetProperties( MbProperties & properties ); // \ru Выдать свойства объекта \en Get properties of the object 
  virtual void        SetProperties( const MbProperties & properties ); // \ru Записать свойства объекта \en Set properties of the object 
  virtual MbePrompt   GetPropertyName(); // \ru Выдать заголовок свойства объекта \en Get a name of object property 

  virtual bool        IsSame( const MbCreator & other, double accuracy ) const; // \ru Являются ли объекты равными? \en Determine whether an object is equal? 
  virtual bool        SetEqual( const MbCreator & );       // \ru Сделать равным \en Make equal 
  virtual bool        IsSimilar ( const MbCreator & ) const; // \ru Являются ли объекты подобными \en Whether the objects are similar 

  // \ru Общие функции твердого тела \en Common functions of solid 

  virtual bool        CreateShell( MbFaceShell *&shell, MbeCopyMode sameShell, 
                                   RPArray<MbSpaceItem> *items = NULL ); // \ru Построение \en Construction 

private :
          // \ru Объявление оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration of the assignment operator without implementation to prevent an assignment by default. 
          void        operator = ( const MbDraftSolid & ); 

  DECLARE_PERSISTENT_CLASS_NEW_DEL( MbDraftSolid )
};


//------------------------------------------------------------------------------
/** \brief \ru Построить оболочку с уклоном граней.
           \en Create a shell with drafted faces. \~
  \details \ru Для исходной оболочки построить оболочку с уклоном граней от нейтральной изоплоскости для создания литейных уклонов. \n
    Одновременно с построением оболочки функция создаёт её строитель.\n
           \en For the source shell create a shell with faces drafted from the neutral isoplane for pattern tapers creation. \n
    The function simultaneously creates the shell and its constructor.\n \~ 
  \param[in]  solid      - \ru Исходная оболочка.
                           \en The source shell. \~
  \param[in]  sameShell  - \ru Режим копирования исходной оболочки.
                           \en Mode of copying the source shell. \~
  \param[in]  np         - \ru Локальная система координат, плоскость XY которой является нейтральной плоскостью ( neutral plane ).
                           \en The local coordinate system XY plane of which is a neutral plane. \~
  \param[in]  angle      - \ru Угол уклона.
                           \en Draft angle. \~
  \param[in]  faces      - \ru Уклоняемые грани.
                           \en The faces to draft. \~
  \param[in]  fp         - \ru Признак захвата граней ( face propagation ).
                           \en Flag of face propagation. \~
  \param[in]  reverse    - \ru Флаг для обратного направления тяги.
                           \en Flag for reverse pull direction. \~
  \param[in]  names      - \ru Именователь операции.
                           \en An object defining a name of the operation. \~
  \param[out] res        - \ru Код результата операции выдавливания.
                           \en The extrusion operation result code. \~
  \param[out] shell      - \ru Построенная оболочка.
                           \en The resultant shell. \~
  \result \ru Возвращает строитель оболочки.
          \en Returns the shell constructor. \~
  \ingroup Model_Creators
*/
// ---
MATH_FUNC (MbCreator *) CreateDraft( MbFaceShell           * solid,
                                     MbeCopyMode             sameShell,
                                     const MbPlacement3D   & np,
                                     double                  angle,
                                     const RPArray<MbFace> & faces,
                                     MbeFacePropagation      fp,
                                     bool                    reverse,
                                     const MbSNameMaker    & names,
                                     MbResultType          & res,
                                     MbFaceShell          *& shell );


#endif // __CR_DRAFT_SOLID_H
