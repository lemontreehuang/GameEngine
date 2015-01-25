﻿
#ifndef RB_TREE_H
#define RB_TREE_H

#include "libcommon/memorypool.h"

#define _BLACK	false
#define _RED	true

template <class K, class V>
class RBTree
{
public:
	RBTree(unsigned int increase = 64);
		
	void	Insert( K key);
	void	Insert(K key, V &val);
	V *		Find( K key );
	void	Erase( K key );
	V *		Begin(){ return LeftMost(m_root); }
	V *		End(){ return nil; }
	void	NextNode(V **node);
private:
	void	InsertFixup( V * );

	void	LeftRotate(  V * );
	void	RightRotate(  V * );

	void	Delete(  V * );
	void	DeleteFixup(  V * );

	V *		Uncle( V *);
	V *		TreeSuccessor(  V * );
	V *		LeftMost(V * parent);

	MemoryPool	m_mem_pool;
	V *nil;			//空结点
	V *m_root;
};

template <class K, class V>
RBTree<K, V>::RBTree(unsigned int increase /*= 64*/)
: m_mem_pool(sizeof(V), increase)
, nil((V *)m_mem_pool.Alloc())
, m_root(nil)
{
	nil->left = 0;
	nil->parent = 0;
	nil->right = 0;
	nil->color = _BLACK;
}

template <class K, class V>
V * RBTree<K, V>::Uncle(V * node)
{
	if (node->parent == node->parent->parent->left)
	{
		return node->parent->parent->right;
	}
	return node->parent->parent->left;
}


//////////////////////////////////////////////////////////////////////////
//  3  红黑树类成员定义  
//////////////////////////////////////////////////////////////////////////
/*
	◆ 插入节点必须是红色。（如果设为黑色，就会导致根到叶子的路径上有一条路上，多一个额外的黑节点，这个是很难调整的。但是设为红色节点后，可能会导致出现两个连续红色节点的冲突，那么可以通过颜色调换（color flips）和树旋转来调整。）
*/

template <class K, class V>
void RBTree<K, V>::Insert(K key)
{
	V* node = (V *)m_mem_pool.Alloc();
	V* n = m_root;
	V* parent = nil;
	node->key = key;
	while( n != nil )
	{
		parent = n;
		if( key < n->key )
		{
			n = n->left;
		}
		else
		{
			n = n->right;
		}
	}
	node->parent = parent;
	if( parent == nil )
	{
		m_root = node;
	}
	else
	{
		if ( node->key < parent->key )
		{
			parent->left = node;
		}
		else
		{
			parent->right = node;
		}
	}
	node->left	= nil;
	node->right	= nil;
	node->color	= _RED;
	InsertFixup( node );
}

template <class K, class V>
void RBTree<K, V>::Insert(K key, V &val)
{
	V* node = (V *)m_mem_pool.Alloc();
	memcpy(node, &val, sizeof(V));
	V* n = m_root;
	V* parent = nil;
	node->first = key;
	while (n != nil)
	{
		parent = n;
		if (key < n->first)
		{
			n = n->left;
		}
		else
		{
			n = n->right;
		}
	}
	node->parent = parent;
	if (parent == nil)
	{
		m_root = node;
	}
	else
	{
		if (node->first < parent->first)
		{
			parent->left = node;
		}
		else
		{
			parent->right = node;
		}
	}
	node->left = nil;
	node->right = nil;
	node->color = _RED;
	InsertFixup(node);
}

template <class K, class V>
void RBTree<K, V>::InsertFixup(V *node)
{
	while( node->parent->color  == _RED )
	{
		V *uncle = Uncle(node);
		if (uncle->color == _RED)
		{
			node->parent->color = _BLACK;
			uncle->color = _BLACK;
			node->parent->parent->color = _RED;
			node = node->parent->parent;
		}
		else
		{
			if( node->parent == node->parent->parent->left )
			{
				if( node == node->parent->right )
				{
					node = node->parent;
					LeftRotate( node );
				}//
				node->parent->color = _BLACK;
				node->parent->parent->color = _RED;
				node->parent->right->color = _BLACK;
				RightRotate( node->parent->parent );
			}
			else
			{
				if( node == node->parent->left )
				{
					node = node->parent;
					RightRotate( node );
				}
				node->parent->color = _BLACK;
				node->parent->parent->color = _RED;
				node->parent->left->color = _BLACK;
				LeftRotate( node->parent->parent );
			}
		}
	}
	m_root->color = _BLACK;
}

template <class K, class V>
V * RBTree<K, V>::Find( K key )
{
	V * node = m_root;
	while( node != nil && key != node->first )
	{
		if( key < node->first )
		{
			node = node->left;
		}
		else
		{
			node = node->right;
		}
	}
	return node;
}

template <class K, class V>
void RBTree<K, V>::Erase(K key)
{
	V *node = Find( key );
	if( node != nil )
	{
		Delete(node);
	}
}

template <class K, class V>
V * RBTree<K, V>::TreeSuccessor(V *x)
{
	if( x->right != nil )
	{
		while (x->left != nil)
		{
			x = x->left;
		}
		return x;
	}
	V * y = x->parent;
	while( y != nil && x == y->right )
	{
		x = y;
		y = y->parent;
	}
	return y;
}

template <class K, class V>
void RBTree<K, V>::LeftRotate(V *node)		// x->right != nil
{		
	V *r_child = node->right;
	if( node->parent == nil )
	{
		m_root = r_child;
	}
	else
	{
		if( node == node->parent->left )
		{
			node->parent->left = r_child;
		}
		else
		{
			node->parent->right = r_child;
		}
	}
	r_child->parent = node->parent;
	node->right = r_child->left;
	r_child->left->parent = node;
	r_child->left = node;
	node->parent = r_child;
}

template <class K, class V>
void RBTree<K, V>::RightRotate(V *node)
{
	V *l_child = node->left;
	if( node->parent == nil )
	{
		m_root = l_child;
	}
	else
	{
		if( node->parent->left == node )
		{
			node->parent->left = l_child;
		}
		else
		{
			node->parent->right = l_child;
		}
	}
	l_child->parent = node->parent;
	node->left = l_child->right;
	l_child->right->parent = node;
	l_child->right = node;
	node->parent = l_child;
}

template <class K, class V>
void RBTree<K, V>::Delete(V *z)
{
	V * x = nil;
	V * y = nil;
	if (z->left == nil || z->right == nil)
	{
		y = z;
	}
	else
	{
		y = TreeSuccessor(z);
	}
	if (y->left != nil)
	{
		x = y->left;
	}
	else
	{
		x = y->right;
	}
	x->parent = y->parent;
	if (y->parent == nil)
	{
		m_root = x;
	}
	else
	{
		if (y == y->parent->left)
		{
			y->parent->left = x;
		}
		else
		{
			y->parent->right = x;
		}
	}
	if (y != z)
	{
		z->first = y->first;
#ifdef GAME_MAP
		memcpy(&z->val, &y->val, sizeof(y->val));
#endif // DEBUG
	}
		
	if (y->color == _BLACK)
	{
		DeleteFixup( x );
	}
	//delete y;
	m_mem_pool.Free(y);
}

template <class K, class V>
void RBTree<K, V>::DeleteFixup(V * x)
{
	while( x != m_root && x->color == _BLACK )
	{
		V * w = 0;
		if( x->parent->left == x )
		{
			w = x->parent->right;
			if( w->color == _RED )
			{
				w->color = _BLACK;
				x->parent->color = _RED;
				LeftRotate( x->parent );
				w = x->parent->right;
			}
			if( w->left->color == _BLACK && w->right->color == _BLACK )
			{
				w->color = _RED;
				x = x->parent;
			}
			else
			{
				if( w->right->color == _BLACK )
				{
					w->left->color = _RED;
					w->color = _RED;
					RightRotate( w );
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = _BLACK;
				w->right->color = _BLACK;
				LeftRotate( x->parent );
				x = m_root;
			}
		}
		else
		{
			w = x->parent->left;
			if( w->color == _RED )
			{
				w->color = _BLACK;
				x->parent->color = _RED;
				RightRotate( x->parent );
				w = x->parent->left;
			}
			if( w->right->color == _BLACK && w->left->color == _BLACK )
			{
				w->color = _RED;
				x = x->parent;
			}
			else
			{
				if( w->left->color == _BLACK )
				{
					w->right->color = _BLACK;
					w->color = _RED;
					LeftRotate( w );
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = _BLACK;
				w->left->color = _BLACK;
				RightRotate( x->parent );
				x = m_root;
			}
		}
	}
	x->color = _BLACK;
}

template <class K, class V>
V * RBTree<K, V>::LeftMost(V * parent)
{
	V * node = parent;
	while (node->left != nil)
	{
		node = node->left;
	}
	if (node->right != nil)
	{
		return LeftMost(node->right);
	}
	return node;
}

template <class K, class V>
void RBTree<K, V>::NextNode(V **node)
{
	if ((*node)->parent == nil)
	{
		(*node) = End();
		return;
	}

	if ((*node) == (*node)->parent->left && (*node)->parent->right != nil)
	{
		(*node) = LeftMost((*node)->parent->right);
		return;
	}
	(*node) = (*node)->parent;
}

#endif