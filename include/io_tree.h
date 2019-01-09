////////////////////////////////////////////////////////////////////////////////
/**
\file
\brief \ru Дерево геометрической модели.
\en Tree of geometric model. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __IO_TREE_H
#define __IO_TREE_H

#include <set>
#include <vector>
#include <math_define.h>
#include <tool_cstring.h>

class reader;
class writer;
struct ClusterReference;
class TapeBase;

namespace c3d // namespace C3D
{

//----------------------------------------------------------------------------------------
// \ru Предварительное объявление структуры для данных узла дерева.
// \en The forward declaration of a structure for the tree node data. \~
// ---
struct MbItemData;

//----------------------------------------------------------------------------------------
/** \brief   \ru Узел дерева.
             \en Tree node. \~
    \details \ru Узел дерева (может иметь несколько потомков). 
                 Умеет записывать в поток и читаться из потока. \n
             \en Tree node (can have several children). 
                 Can be written to a stream and read from a stream. \n \~
    \ingroup Base_Tools_IO
*/
// ---
class IModelTreeNode
{
protected:
  /// \ru Непосредственные потомки узла. \en The immediate children of the node. 
  std::set<IModelTreeNode*> m_children;
  /// \ru Непосредственные предки узла. \en The immediate parents of the node. 
  std::set<IModelTreeNode*> m_parents;
public:
  IModelTreeNode() {}
  virtual ~IModelTreeNode() {}

  /// \ru Доступ к непосредственным предкам узла. \en Access to the immediate node parents. 
  std::set<IModelTreeNode*>& GetParents() { return m_parents; }
  const std::set<IModelTreeNode*>& GetParents() const { return m_parents; }

  /// \ru Доступ к непосредственным потомкам узла. \en Access to the immediate node children. 
  std::set<IModelTreeNode*>& GetChildren() { return m_children; }
  const std::set<IModelTreeNode*>& GetChildren() const { return m_children; }

  /// \ru Добавить предка. \en Add a parent. 
  void AddParent( IModelTreeNode* parent ) { if (parent) m_parents.insert(parent); }

  /// \ru Добавить потомка. \en Add a child. 
  void AddChild( IModelTreeNode* child ) { if (child) { m_children.insert(child); child->AddParent(this); } }

  /// \ru Доступ к данным узла. \en Access to the node data. 
  virtual MbItemData& GetData() = 0;
  virtual const MbItemData& GetData() const = 0;

  /// \ru Доступ к позиции чтения/записи узла. \en Access to the node read/write position. 
  virtual ClusterReference& GetPosition() = 0;
  virtual const ClusterReference& GetPosition() const = 0;

  // \ru Признак частичного или полного чтения узла.
  //     При чтении объекта может возникнуть необходимость чтения некоторых данных его родителя.
  //     В этом случае объект родителя читается частично и имеет соответствующий флаг.
  // \en Indicator of partial reading of the current node.
  //     While reading an object there can be a need to read some data from its parent.
  //     In this case the parent object is read partially and has a corresponding flag.
  // \ru Узнать, читать ли только часть узла.
  // \en Check whether to read the node partially.
  virtual bool PartialRead() const = 0;
  // \ru Установить признак частичного или полного чтения узла.
  // \en Set indication of full or partial node reading.
  virtual void SetPartialRead ( bool partial ) const = 0;

  /// \ru Записать узел. \en Write the node. 
  virtual writer &   operator >> ( writer & ) = 0;
  /// \ru Прочитать узел. \en Read the node. 
  virtual reader &   operator << ( reader & ) = 0;

  /// \ru Операторы для записи узла дерева поток в xml формате. \en Operators to output tree node to a stream in xml format.
  friend c3d::t_ofstream& CALL_DECLARATION operator << (c3d::t_ofstream& file, IModelTreeNode& node);
  friend c3d::t_ofstream& CALL_DECLARATION operator << (c3d::t_ofstream& file, const IModelTreeNode& node);

OBVIOUS_PRIVATE_COPY(IModelTreeNode)
};

//----------------------------------------------------------------------------------------
/** \brief   \ru Обобщенное дерево.
             \en Generic tree. \~
    \details \ru Обобщенное дерево (может иметь несколько корней). 
                 Умеет записывать в поток и читаться из потока. \n
             \en Generic tree (can have several roots). 
                 Can be written to a stream and read from a stream. \n \~
    \ingroup Base_Tools_IO
*/
// ---
class IModelTree
{
public:

  /// \ru Тип, представляющий листовой узел с ветвью дерева, ведущей к нему, начиная с корневого узла дерева.
  /// \en A type which represents a leaf node with the tree branch, leading to it, starting from the root of the tree.
  typedef std::pair<IModelTreeNode*,std::vector<IModelTreeNode*> > NodeBranch;

  /// \ru Тип функции для выбора узлов дерева по фильтрам.
  /// \en The type of a function for selecting tree nodes by filters.
  typedef bool ( CALL_DECLARATION * FilterNodesFunc ) ( std::vector<NodeBranch>&, const std::vector<MbItemData>&, IModelTree* );

  /// \ru Тип функции для определения, нужно ли добавлять объект в дерево модели, и заполнения данных узла.
  /// \en The type of a function for determining, whether to add the object to the model tree, and filling the node data.
  typedef bool ( CALL_DECLARATION *NodeToAddFunc ) ( const TapeBase* mem, MbItemData& data );

protected:
  // \ru Функция для определения, нужно ли добавлять объект в дерево модели, и заполнения данных узла.
  // \en A function for determining, whether to add the object to the model tree, and filling the node data.
  NodeToAddFunc                m_nodeToAddFunc;

  // \ru Функция для выбора объектов по фильтрам.
  // \en A function for for selecting objects by filters.
  FilterNodesFunc              m_filterFunc;

  // \ru Корни дерева. \en The tree roots.
  std::vector<IModelTreeNode*> m_roots;
public:

  IModelTree() {}
  virtual ~IModelTree() {}

  /// \ru Добавить узел. \en Add a node. 
  virtual void AddNode ( const TapeBase* mem, const ClusterReference& ref ) = 0;

  /// \ru Нотификация об окончании чтения/записи текущего узла.
  /// \en Nofification about the end of current node writing/reading. 
  virtual void CloseNode( const TapeBase* mem ) = 0;

  /// \ru Построить дерево из узлов, выбранных по фильтрам. Вызывающая функция несет ответственность за уничтожение дерева.
  //  \en Build a tree with nodes, selected by filters. Caller is responsible for destroying the tree.
  virtual IModelTree* GetFilteredTree ( const std::vector<MbItemData>& filters ) = 0;

  /// \ru Построить дерево по заданным узлам. Вызывающая функция несет ответственность за уничтожение дерева.
  //  \en Build a tree for given nodes. Caller is responsible for destroying the tree.
  virtual IModelTree* GetFilteredTree ( std::vector<IModelTreeNode*>& nodes ) = 0;

  // \ru Установить функцию для выбора геометрического объекта для добавления в дерево модели, и заполнения данных узла.
  // \en Define a function for selecting a geometric object for adding to the model tree, and filling the node data.
  virtual void SetNodeToAddFunction( NodeToAddFunc callback ) { if ( callback ) m_nodeToAddFunc = callback; }

  // \ru Установить функцию для выбора узлов из дерева модели.
  // \en Define a function for selecting nodes from the model tree.
  virtual void SetFilterFunction( FilterNodesFunc callback ) { if ( callback ) m_filterFunc = callback; }

  /// \ru Записать дерево. \en Write the tree. 
  virtual writer &   operator >> ( writer & ) = 0;
  /// \ru Прочитать дерево. \en Read the tree. 
  virtual reader &   operator << ( reader & ) = 0;


  /// \ru Доступ к корням дерева.
  /// Узел дерева может быть рекурсивно вложен
  /// (например, Instance может содержать сборку, которая содержит другой Instance, ссылающийся на эту же сборку).
  /// \en Access to the tree roots.
  /// Tree node could be nested recursively
  /// (e.g. Instance can contain an Assembly which contains another Instance which includes this Assembly).
  const std::vector<IModelTreeNode*>& GetRoots() const { return m_roots; }
  std::vector<IModelTreeNode*>& GetRoots() { return m_roots; }

  /// \ru Версия дерева. \en Tree version.
  virtual VERSION GetVersion() = 0;
  virtual void SetVersion( VERSION ) = 0;

  /// \ru Создать экземпляр дерева. \en Create a tree instance. 
  static EXPORT_DECLARATION IModelTree* CreateModelTree();

  /// \ru Операторы для записи дерева в поток в xml формате. \en Operators to output a tree to a stream in xml format.
  friend c3d::t_ofstream& CALL_DECLARATION operator << (c3d::t_ofstream& file, IModelTree& tree);
  friend c3d::t_ofstream& CALL_DECLARATION operator << (c3d::t_ofstream& file, const IModelTree& tree);

OBVIOUS_PRIVATE_COPY(IModelTree)
};

} //namespace c3d

#endif // __IO_TREE_H
