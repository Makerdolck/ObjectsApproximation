////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Cортированный одномерный массив указателей на обьекты.
         \en Sorted one-dimensional array of pointers to objects. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_SFDP_ARRAY_H
#define __TEMPL_SFDP_ARRAY_H


#include <templ_fdp_array.h>


FORVARD_DECL_TEMPLATE_TYPENAME( class SFDPArray );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t add_to_array              ( SFDPArray<Type> & arr, Type& el, Type *& found ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t add_to_array_if_not_exist ( SFDPArray<Type> & arr, Type& el, Type *& found ) );
FORVARD_DECL_TEMPLATE_TYPENAME( size_t find_in_array             ( const SFDPArray<Type>&, const Type&, Type *&found ) );
FORVARD_DECL_TEMPLATE_TYPENAME( reader & CALL_DECLARATION operator >> ( reader& in, SFDPArray<Type> & ref ) );         
FORVARD_DECL_TEMPLATE_TYPENAME( writer & CALL_DECLARATION operator << ( writer& out, const SFDPArray<Type> & ref ) );


//------------------------------------------------------------------------------
/** \brief \ru Cортированный одномерный массив указателей на обьекты.
           \en Sorted one-dimensional array of pointers to objects. \~
  \details \ru Cортированный одномерный массив указателей на обьекты. \n
    Нет повторного добавления. Без функции сравнения массив бесполезен.
           \en Sorted one-dimensional array of pointers to objects. \n
    There is no repeat adding. The array is useless without comparison function. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
class  SFDPArray : private FDPArray<Type> {

protected :
  typedef int   (*CompareFunc)( const Type &d1, const Type &d2 ); // \ru функция сравнения для поиска и наполнения \en a comparison function for search and filling 
  typedef int   (*SearchFunc) ( const Type &d1, size_t d );       // \ru функция сравнения для поиска \en a comparison function for search 

  CompareFunc   fCompare; // \ru функция сравнения \en a comparison function 

public :
  SFDPArray( size_t i_upper, uint16 i_delta, CompareFunc fc, typename FDPArray<Type>::DestroyFunc fd );
  virtual ~SFDPArray();

using  FDPArray<Type>::AddCArray;
using  FDPArray<Type>::OwnsElem;
using  FDPArray<Type>::Delta;
using  FDPArray<Type>::Upper;
using  FDPArray<Type>::SetMaxDelta;
using  FDPArray<Type>::Flush;
using  FDPArray<Type>::Clear;
using  FDPArray<Type>::Adjust;
using  FDPArray<Type>::Reserve;
using  FDPArray<Type>::RemoveInd;
using  FDPArray<Type>::DetachInd;
using  FDPArray<Type>::DestroyInd;
using  FDPArray<Type>::DestroyObj;
using  FDPArray<Type>::Count;
using  FDPArray<Type>::MaxIndex;
using  FDPArray<Type>::operator[];

using  RPArray<Type>::size;
using  RPArray<Type>::begin;
using  RPArray<Type>::end;
using  RPArray<Type>::cbegin;
using  RPArray<Type>::cend;
using  RPArray<Type>::front;
using  RPArray<Type>::back;


  void                   Init    ( const SFDPArray<Type> & other );  // \ru сбросить себя и скопировать other \en reset itself and copy 'other' 

  size_t                 AddTry  ( Type &ent, Type *&found );  // \ru попробовать добавить элемент с сортировкой, \en try to add an element with sorting, 
                                                               // \ru нельзя добавить NULL pointer \en a NULL pointer con not be added 
                                                               // \ru нельзя добавить повторно существующий объект \en an existed object can not be added repeatedly 
                                                               // \ru если (ent != found) - значит найден точно такой-же \en if (ent != found) then there is found the same one 
                                                               // \ru и добавление не произошло, вернет индекс найденного \en and adding was not happened, the index of the found one is returned 
                                                               // \ru иначе вернет индекс добавленного \en otherwise the index of added one is returned 

  bool                   AddExact( Type & );                   // \ru добавить элемент с сортировкой, вернуть индекс добавленного \en add an element with sorting, return the index of the added element 
                                                               // \ru нельзя добавить NULL pointer \en a NULL pointer con not be added 
                                                               // \ru нельзя добавить повторно существующий объект \en an existed object can not be added repeatedly 
                                                               // \ru ФД К8+ 24.03.2006 при этом все равно вернет true \en ФД К8+ 24.03.2006 returns true 
                                                               // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 

  bool                   AddIfNotExist( Type & );              // \ru добавить элемент с сортировкой, вернуть индекс добавленного \en add an element with sorting, return the index of the added element 
                                                               // \ru нельзя добавить NULL pointer \en a NULL pointer con not be added 
                                                               // \ru нельзя добавить повторно существующий объект \en an existed object can not be added repeatedly 
                                                               // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
                                                               // \ru ФД К8+ 24.03.2006 Исправление ошибки  17199 \en ФД К8+ 24.03.2006 fix for the bug 17199 

  void                   AddSimple  ( Type &ent ) { FDPArray<Type>::Add( &ent ); } // \ru Доступ к функции базового класса - добавить элемент в конец массива \en An access to the function of the base class - add an element to the end of the array 
  size_t                 FindNearest( const Type &el, Type *&found ) const;   // \ru найти индекс элемента, используя функцию сравнения \en find the index of the element using the comparison function 
                                                               // \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
                                                               // \ru вернет -1 если ближайший \en returns -1 if this is the nearest 
                                                               // \ru или индекс точно такого-же \en or the index of the same element 

  Type  *                FindExact  ( const Type &el ) const;                 // \ru найти индекс элемента, используя функцию сравнения \en find the index of the element using the comparison function 
                                                                              // \ru вернет точно найденный \en returns the found one 

  bool                   IsExist ( const Type &el ) const;                    // \ru true если элемент найден \en true if the element was found 
  Type*                  RemoveObj( Type *delObject, DelType=defDelete );
  bool                   DetachObj( const Type *delObject );
  size_t                 SearchIt( size_t , SearchFunc, Type *&found ) const;  // \ru найти индекс элемента, используя функцию поиска \en find the index of the element using the search function 
                                                               // \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
                                                               // \ru вернет -1 если ближайший \en returns -1 if this is the nearest 
                                                               // \ru или индекс точно такого-же \en or the index of the same element 
  void                   Sort( size_t /*OV_x64 int*/ minInd = SYS_MAX_T/*OV_x64 -1*/, size_t /*OV_x64 int*/ maxInd = SYS_MAX_T/*OV_x64 -1*/ ); // \ru сортировать массив, используя функцию сравнения \en sort an array using the comparison function 

private:
  SFDPArray( const SFDPArray<Type> & );                        // \ru запрещено !!! \en forbidden !!! 
  SFDPArray<Type>& operator = ( const SFDPArray<Type> & );     // \ru запрещено !!! \en forbidden !!! 

  TEMPLATE_FRIEND size_t add_to_array              TEMPLATE_SUFFIX ( SFDPArray<Type>& arr, Type& el, Type *&found );
  TEMPLATE_FRIEND size_t add_to_array_if_not_exist TEMPLATE_SUFFIX ( SFDPArray<Type>& arr, Type& el, Type *&found );
  // \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
  TEMPLATE_FRIEND size_t find_in_array             TEMPLATE_SUFFIX ( const SFDPArray<Type>&, const Type&, Type *&found );

  // \ru Т.к. наследование от базового класса сделано private, то делаю доступ к операторам базового класса.
  // \en Since there is a private inheritance from the base class, I give an access to the operators of the base class.
  TEMPLATE_FRIEND reader& CALL_DECLARATION operator >> TEMPLATE_SUFFIX ( reader& in, SFDPArray<Type> & ref );         
  TEMPLATE_FRIEND writer& CALL_DECLARATION operator << TEMPLATE_SUFFIX ( writer& out, const SFDPArray<Type> & ref );  

#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
public:
  void * operator new    ( size_t );
  void   operator delete ( void *, size_t );
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_
};


#ifdef __DEBUG_MEMORY_ALLOCATE_FREE_
//------------------------------------------------------------------------------
// \ru Перегрузка оператора new. \en Overloading of the "new" operator. 
// ---
template <class Type>
inline void * SFDPArray<Type>::operator new( size_t size ) {
  return ::Allocate( size, typeid(SFDPArray<Type>).name() );
}

//------------------------------------------------------------------------------
// \ru Перегрузка оператора delete. \en Overloading of the "delete" operator. 
// ---
template <class Type>
inline void SFDPArray<Type>::operator delete( void * ptr, size_t size ) {
  ::Free( ptr, size, typeid(SFDPArray<Type>).name() );
}
#endif // __DEBUG_MEMORY_ALLOCATE_FREE_


//------------------------------------------------------------------------------
// \ru конструктор массива \en constructor of an array 
// ---
template <class Type>
inline SFDPArray<Type>::SFDPArray( size_t i_upper, uint16 i_delta, CompareFunc fc, typename FDPArray<Type>::DestroyFunc fd )
  : FDPArray<Type>( i_upper, i_delta, fd ),
    fCompare ( fc ) // \ru функция сравнения \en a comparison function 
{}


//------------------------------------------------------------------------------
// \ru деструктор массива \en destructor of array 
// ---
template <class Type>
inline SFDPArray<Type>::~SFDPArray() {}


//------------------------------------------------------------------------------
// \ru сбросить себя и скопировать other \en reset itself and copy 'other' 
// ---
template <class Type>
void SFDPArray<Type>::Init( const SFDPArray<Type> & other ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  
  Flush(); // \ru сбросить себя \en reset itself 

  AddCArray( other.GetAddr(), other.count );
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// ---
template <class Type>
inline size_t  SFDPArray<Type>::AddTry( Type& ent, Type *&found ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  return add_to_array( *this, ent, found );
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
// ---
template <class Type>
inline bool SFDPArray<Type>::AddExact( Type& ent ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  Type * found = NULL;
  add_to_array( *this, ent, found );
  return ( &ent == found ); // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
}


//------------------------------------------------------------------------------
// \ru добавить элемент \en add element 
// \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
// ---
template <class Type>
inline bool SFDPArray<Type>::AddIfNotExist( Type& ent ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
// \ru CatchMemory(); ЯТ \en CatchMemory(); ЯТ 

  Type * found = NULL;
  add_to_array_if_not_exist( *this, ent, found );
  return ( &ent == found ); // \ru вернет true - добавлен, false - не добавлен \en if returns true then the element has been added, it has not been added otherwise 
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline size_t SFDPArray<Type>::FindNearest( const Type &el, Type *&found ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  return find_in_array( *this, el, found );
}


//------------------------------------------------------------------------------
// \ru найти индекс элемента, используя функцию сравнения \en find the index of the element using the comparison function 
// \ru вернет точно найденный \en returns the found one 
// ---
template <class Type>
inline Type * SFDPArray<Type>::FindExact( const Type &el ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  Type * found = NULL;
  size_t  foundInd = find_in_array( *this, el, found );
  return ( foundInd != SYS_MAX_T ) ? found : NULL;
}


//------------------------------------------------------------------------------
// \ru удалить элемент из массива \en delete an element from array 
// ---
template <class Type>
inline Type* SFDPArray<Type>::RemoveObj( Type *delObject, DelType del ) {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  if ( !delObject )
    return NULL;
  
  Type * found = NULL;
  size_t  i = find_in_array( *this, *delObject, found );
  return ( i != SYS_MAX_T ) ? RemoveInd(i, del) : 0;
}


//------------------------------------------------------------------------------
// \ru отсоединить элемент от массива (по указателю) \en detach an element from the array (by the pointer) 
// ---
template <class Type>
inline bool SFDPArray<Type>::DetachObj( const Type *delObject ) {
  if ( !delObject )
    return false;
  Type * found = NULL;
  size_t i = find_in_array( *this, *delObject, found );
  
  if ( i != SYS_MAX_T ) {
    DetachInd( i );
    return true;
  }
  
  return false;
}


//------------------------------------------------------------------------------
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// ---
template <class Type>
inline bool SFDPArray<Type>::IsExist( const Type &el ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  Type * found = NULL;
  return find_in_array( *this, el, found ) != SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
// \ru снаружи отвести памяти \en allocate memory from the outside 
// ---
template <class Type>
size_t add_to_array ( SFDPArray<Type>& arr, Type& el, Type *&found ) {
  PRECONDITION( arr.fCompare ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 

  if ( !arr.count ) {
    found = &el;
//arr.parr[arr.count++] = &el; // \ru count вырос на 1 \en 'count' increased by 1 
    arr.Insert( 0, &el );
    return 0;
  }

  found = arr/*.parr*/[0];
  int res = (*arr.fCompare)( *found, el );
  if ( res > 0 ) { // \ru нулевой БОЛЬШЕ пришедшего - вставим ПЕРЕД нулевым \en the first element is GREATER than the sent one - set it BEFORE the first element 
    found = &el;
    arr.Insert( 0, &el );
    return 0;
  }
  else if ( res == 0 ) {
    return 0; // \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
  }

  if ( arr.count == 1 ) { // \ru есть всего один \en only one exists 
    if ( res < 0 ) { // \ru нулевой МЕНЬШЕ пришедшего - вставим ПОСЛЕ нулевого \en the first element is LESS than the sent one - set it AFTER the first element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого \en by this time the sent element is GREATER than the first element 
  size_t mx = arr.count - 1;

  found = arr/*.parr*/[mx];
  res = (*arr.fCompare)( *found, el );
  if ( res < 0 ) { // \ru последний МЕНЬШЕ пришедшего - вставим ПОСЛЕ последнего \en the last element is LESS than the sent one - set it AFTER the last element 
    found = &el;
    arr.Insert( mx+1, &el );
    return mx+1;
  }
  else if ( res == 0 ) {
    return mx; // \ru повторно добавить нельзя!!! \en it cannot be added repeatedly!!! 
  }

  if ( arr.count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    if ( res > 0 ) { // \ru последний БОЛЬШЕ пришедшего - вставим ПЕРЕД последним \en the last element is GREATER than the sent one - set it BEFORE the last element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого, но МЕНЬШЕ последнего \en by the moment the sent element is GREATER than the first element and LESS than the last element 
  // \ru вставим где-то между \en put it somewhere between 
  size_t  mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    size_t md = ( mn + mx ) / 2;

    found = arr/*.parr*/[md];
    res = (*arr.fCompare)( *found, el );
    if ( res > 0 ) { // \ru md БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'md' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
      mx = md;
    }
    else if ( res < 0 ) { // \ru md МЕНЬШЕ пришедшего - поиск справа \en 'md' is LESS than the sent one - search on the right 
      mn = md;
    }
    else {
      return md; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
    }

  } // end while

  found = &el;
  arr.Insert( mx, &el );
  return mx;
}


//------------------------------------------------------------------------------
// \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
// \ru снаружи отвести памяти \en allocate memory from the outside 
// ---
template <class Type>
size_t add_to_array_if_not_exist ( SFDPArray<Type>& arr, Type& el, Type *&found ) {
  PRECONDITION( arr.fCompare ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 

  if ( !arr.count ) {
    found = &el;
//arr.parr[arr.count++] = &el; // \ru count вырос на 1 \en 'count' increased by 1 
    arr.Insert( 0, &el );
    return 0;
  }

  found = arr/*.parr*/[0];
  int res = (*arr.fCompare)( *found, el );
  if ( res > 0 ) { // \ru нулевой БОЛЬШЕ пришедшего - вставим ПЕРЕД нулевым \en the first element is GREATER than the sent one - set it BEFORE the first element 
    found = &el;
    arr.Insert( 0, &el );
    return 0;
  }
  else if ( res == 0 ) {
    found = NULL;
    return 0; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
  }

  if ( arr.count == 1 ) { // \ru есть всего один \en only one exists 
    if ( res < 0 ) { // \ru нулевой МЕНЬШЕ пришедшего - вставим ПОСЛЕ нулевого \en the first element is LESS than the sent one - set it AFTER the first element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого \en by this time the sent element is GREATER than the first element 
  size_t mx = arr.count - 1;

  found = arr/*.parr*/[mx];
  res = (*arr.fCompare)( *found, el );
  if ( res < 0 ) { // \ru последний МЕНЬШЕ пришедшего - вставим ПОСЛЕ последнего \en the last element is LESS than the sent one - set it AFTER the last element 
    found = &el;
    arr.Insert( mx+1, &el );
    return mx+1;
  }
  else if ( res == 0 ) {
    found = NULL;
    return mx; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
  }

  if ( arr.count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    if ( res > 0 ) { // \ru последний БОЛЬШЕ пришедшего - вставим ПЕРЕД последним \en the last element is GREATER than the sent one - set it BEFORE the last element 
      found = &el;
      arr.Insert( 1, &el );
      return 1;
    }
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого, но МЕНЬШЕ последнего \en by the moment the sent element is GREATER than the first element and LESS than the last element 
  // \ru вставим где-то между \en put it somewhere between 
  size_t mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    size_t md = ( mn + mx ) / 2;

    found = arr/*.parr*/[md];
    res = (*arr.fCompare)( *found, el );
    if ( res > 0 ) { // \ru md БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'md' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
      mx = md;
    }
    else if ( res < 0 ) { // \ru md МЕНЬШЕ пришедшего - поиск справа \en 'md' is LESS than the sent one - search on the right 
      mn = md;
    }
    else {
      found = NULL;
      return md; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
    }

  } // end while

  found = &el;
  arr.Insert( mx, &el );
  return mx;
}


//------------------------------------------------------------------------------
// \ru бинарный поиск \en binary search 
// ---
template <class Type>
size_t find_in_array( const SFDPArray<Type>& arr, const Type& el, Type *&found ) {
  PRECONDITION( arr.fCompare ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 
  // \ru общий случай - элементов больше двух \en the common case - the number of elements is more than two 
  int res = 0;
  if ( arr.count > 11 ) {
    size_t mx = arr.count - 1;

    size_t mxc = mx;
    size_t mn = 0;

    while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
      size_t md = ( mn + mx ) / 2;
      found = arr[md];
      res = (*arr.fCompare)( *found, el );
      switch ( res ) {
        case 1  : mx = md; break;
        case -1 : mn = md; break;
        case 0  : return md;
        default : PRECONDITION( false );
      }
    }

    // \ru проверка по границам \en check by bounds 
    found = arr[0];
    res = (*arr.fCompare)( *found, el );
    switch ( res ) {
      case 0 : return 0;
      case 1 : return SYS_MAX_T;
      case -1 : break;
    }

    found = arr[mxc];
    res = (*arr.fCompare)( *found, el );
    switch ( res ) {
      case 0  : return mxc;
      default : return SYS_MAX_T;
    }
  }
  else {
    // \ru специальные случаи count ==   >0 & < 11 \en special cases  count ==   >0 & < 11 
    for ( size_t i = 0, count = arr.count; i < count; i++ ) {
      found = arr[i];
      switch ( (*arr.fCompare)( *found, el ) ) {
        case 0  : return i;
        case 1  : return SYS_MAX_T;
        case -1 : break;
      }
    }
  }

  return SYS_MAX_T;
}


//------------------------------------------------------------------------------
// \ru бинарный поиск \en binary search 
// \ru найти индекс элемента, используя функцию поиска \en find the index of the element using the search function 
// \ru вернуть индекс элемента в массиве \en return an index of the element in the array 
// \ru в found будет лежать найденный, или ближайший к искомому \en in the 'found' object the found element or the nearest to the required element will be stored 
// ---
template <class Type>
size_t SFDPArray<Type>::SearchIt ( size_t data, typename SFDPArray<Type>::SearchFunc fSearch, Type *&found ) const {
  PRECONDITION( FDPArray<Type>::nowDeletedElem  == 0 ); // \ru ЯТ - временно \en ЯТ - temporarily 
  PRECONDITION( fSearch ); // \ru без функции сравнения массив бессмысленен \en the array is useless without comparison function 

  if ( !FDPArray<Type>::count )
    return SYS_MAX_T;

  found = (*this)/*parr*/[0]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
  int res = (*fSearch)( *found, data );
  if ( res == 0 )
    return 0;

  if ( FDPArray<Type>::count == 1 )
    return SYS_MAX_T;

  if ( res > 0 ) // \ru пришедший МЕНЬШЕ нулевого \en the sent element is LESS than the first element 
    return SYS_MAX_T;

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого \en by this time the sent element is GREATER than the first element 
  size_t mx = FDPArray<Type>::count - 1;

  found = (*this)/*parr*/[mx]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
  res = (*fSearch)( *found, data );
  if ( res < 0 ) { // \ru последний МЕНЬШЕ пришедшего \en the last element is LESS than the sent element 
    return SYS_MAX_T;
  }
  else if ( res == 0 ) {
    return mx; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
  }

  if ( FDPArray<Type>::count == 2 ) { // \ru значит между 0  и 1 \en between 0 and 1 
    return SYS_MAX_T;
  }

  // \ru к этому моменту пришедший БОЛЬШЕ нулевого, но МЕНЬШЕ последнего \en by the moment the sent element is GREATER than the first element and LESS than the last element 
  // \ru вставим где-то между \en put it somewhere between 
  size_t mn = 0;

  while ( mn + 1 < mx ) {  // \ru пока не нашли - ищем \en seek until do not find 
    size_t md = ( mn + mx ) / 2;

    found = (*this)/*parr*/[md]; // \ru или найденный, или ближайший к искомому \en either found or the nearest to the required one 
    res = (*fSearch)( *found, data );
    if ( res > 0 ) { // \ru md БОЛЬШЕ пришедшего, но mn МЕНЬШЕ пришедшего - поиск слева \en 'md' is GREATER than the sent one and 'mn' is LESS than the sent one - search on the left 
      mx = md;
    }
    else if ( res < 0 ) { // \ru md МЕНЬШЕ пришедшего - поиск справа \en 'md' is LESS than the sent one - search on the right 
      mn = md;
    }
    else {
      return md; // \ru повторно добавить нельзя!!! \en it can not be added repeatedly!!! 
    }

  } // end while

  return SYS_MAX_T;
}


//-----------------------------------------------------------------------------
// \ru Сортировка массива \en Array sorting 
// \ru Н.Вирт "Алгоритмы и структуры данных" 2е издание, Санкт-Петербург, 2001г., стр.111 \en see N.Wirth "Algorithms and Data Structures" 
// ---
template <class Type>
inline void SFDPArray<Type>::Sort( size_t /*OV_x64 int*/ minInd /*= -1*/, size_t /*OV_x64 int*/ maxInd /*= -1*/ )
{
  // K10 SP2 33872
  if ( Count() == 0 )
    return;

  if ( minInd == SYS_MAX_T )
    minInd = 0;
  if ( maxInd == SYS_MAX_T )
    maxInd = MaxIndex();  // \ru OV_x64 проверено - count > 0 \en OV_x64 validated - count > 0 

  size_t i = minInd, j = maxInd; // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  
  size_t im = (i + j)/2;         // \ru OV_x64 приводить к знаковому значению будем только в операторах > и < \en OV_x64 cast to signed value only in operators > and <  

  Type * middle = (*this)[im];

  do {
    while( (*fCompare)(*((*this)[i]), *middle    ) == -1 ) i++;
    while( (*fCompare)(      *middle, *(*this)[j]) == -1 ) j--;
    if ( (ptrdiff_t)i <= (ptrdiff_t)j ) {
      if ( i != j ) {
        Type * wi = (*this)[i];
        (*this)[i] = (*this)[j];
        (*this)[j] = wi;
      }
      i++;
      j--;
    }
  } while( !((ptrdiff_t)i > (ptrdiff_t)j) );


  if ( (ptrdiff_t)minInd < (ptrdiff_t)j )
    Sort( minInd, j );
  if ( (ptrdiff_t)i < (ptrdiff_t)maxInd )
    Sort( i, maxInd );
}


#endif  // __TEMPL_SFDP_ARRAY_H
