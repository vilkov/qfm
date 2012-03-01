/* avltree.h: Private file. Declaration of AVL trees.
 *
 * Copyright (C) 2011,2012  Dmitriy Vilkov <dav.daemon@gmail.com>
 *
 * Licensed under the Academic Free License version 2.0
 * Or under the following terms:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <stddef.h>
#include <stdint.h>
#include <new>


/**
 * Implementation of AVL tree.
 */
template <typename Key, typename Value>
class AvlTree
{
public:
	typedef Key   key_type;
	typedef Value value_type;

	typedef int32_t (*CompareKeys)(const key_type &key1, const key_type &key2);

public:
	inline explicit AvlTree(CompareKeys compareKeys);
	inline ~AvlTree();

	inline value_type &operator[](const key_type &key);

	bool contains(const key_type &key) const;
	void insert(const key_type &key, const value_type &value);
	value_type value(const key_type &key, const value_type &defaultValue = value_type()) const;

private:
	AvlTree(const AvlTree &other);
	const AvlTree &operator=(const AvlTree &other);

private:
	enum Balanced
	{
		BALANCED = 0,
		LEFT_IS_HEAVY = -1,
		RIGHT_IS_HEAVY = 1
	};

	enum Visit
	{
		NONE_IS_VISITED = 0,
		LEFT_IS_VISITED = 1,
		RIGHT_IS_VISITED = 2,
		ALL_IS_VISITED = 3
	};

	struct Node;
	struct Links
	{
		Node *left;
		Node *right;
		Node *parent;
	};

	struct Node
	{
		Node() :
			visit(NONE_IS_VISITED),
			balance(BALANCED),
			key(key),
			value()
		{
			links.left = NULL;
			links.right = NULL;
			links.parent = NULL;
		}
		Node(const key_type &key, const value_type &value, Node *parent) :
			visit(NONE_IS_VISITED),
			balance(BALANCED),
			key(key),
			value(value)
		{
			links.left = NULL;
			links.right = NULL;
			links.parent = parent;
		}
		Node(const key_type &key, Node *parent) :
			visit(NONE_IS_VISITED),
			balance(BALANCED),
			key(key),
			value()
		{
			links.left = NULL;
			links.right = NULL;
			links.parent = parent;
		}

		uint32_t visit;
		Links links;
		Balanced balance;
		key_type key;
		value_type value;
	};

private:
	static void left_left_rotation(Node *parent, Node *child, Node **root);
	static void right_left_rotation(Node *parent, Node *child, Node *grandchild, Node **root);
	static void right_right_rotation(Node *parent, Node *child, Node **root);
	static void left_right_rotation(Node *parent, Node *child, Node *grandchild, Node **root);
	static void rebalance_grew(Node *this_node, Node **root);
	static void rebalance_shrunk(Node *this_node, Node **root);

private:
	void destroy_subtree(Node **subtree_root);
	Node **search_routine(const key_type &key) const;
	Node **search_routine(const key_type &key, Node **out_parent_node) const;
	value_type &search_or_create_node(const key_type &key);
	void search_or_create_node(const key_type &key, const value_type &value);
	void delete_node(const key_type &key);

private:
	Node *m_root;
	CompareKeys m_compareKeys;
};


template <typename Key, typename Value>
AvlTree<Key, Value>::AvlTree(CompareKeys compareKeys) :
	m_root(NULL),
	m_compareKeys(compareKeys)
{}

template <typename Key, typename Value>
AvlTree<Key, Value>::~AvlTree()
{
	destroy_subtree(&m_root);
}

template <typename Key, typename Value>
typename AvlTree<Key, Value>::value_type &AvlTree<Key, Value>::operator[](const key_type &key)
{
	return search_or_create_node(key);
}

template <typename Key, typename Value>
bool AvlTree<Key, Value>::contains(const key_type &key) const
{
	return *search_routine(key);
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::insert(const key_type &key, const value_type &value)
{
	search_or_create_node(key, value);
}

template <typename Key, typename Value>
typename AvlTree<Key, Value>::value_type AvlTree<Key, Value>::value(const key_type &key, const value_type &defaultValue) const
{
	if (Node *node = *search_routine(key))
		return node->value;
	else
		return defaultValue;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::left_left_rotation(Node *parent, Node *child, Node **root)
{
	parent->balance = BALANCED;
	child->balance = BALANCED;

	if (child->links.parent = parent->links.parent)
		if (child->links.parent->links.left == parent)
			child->links.parent->links.left = child;
		else
			child->links.parent->links.right = child;
	else
		(*root) = child;

	if (parent->links.right = child->links.left)
		parent->links.right->links.parent = parent;

	child->links.left = parent;
	parent->links.parent = child;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::right_left_rotation(Node *parent, Node *child, Node *grandchild, Node **root)
{
	parent->balance = BALANCED;
	child->balance = BALANCED;
	grandchild->balance = BALANCED;

	if (grandchild->links.parent = parent->links.parent)
		if (grandchild->links.parent->links.left == parent)
			grandchild->links.parent->links.left = grandchild;
		else
			grandchild->links.parent->links.right = grandchild;
	else
		(*root) = grandchild;

	if (parent->links.right = grandchild->links.left)
		parent->links.right->links.parent = parent;

	if (child->links.left = grandchild->links.right)
		child->links.left->links.parent = child;

	grandchild->links.left = parent;
	grandchild->links.right = child;

	parent->links.parent = grandchild;
	child->links.parent = grandchild;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::right_right_rotation(Node *parent, Node *child, Node **root)
{
	parent->balance = BALANCED;
	child->balance = BALANCED;

	if (child->links.parent = parent->links.parent)
		if (child->links.parent->links.left == parent)
			child->links.parent->links.left = child;
		else
			child->links.parent->links.right = child;
	else
		(*root) = child;

	if (parent->links.left = child->links.right)
		parent->links.left->links.parent = parent;

	child->links.right = parent;
	parent->links.parent = child;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::left_right_rotation(Node *parent, Node *child, Node *grandchild, Node **root)
{
	parent->balance = BALANCED;
	child->balance = BALANCED;
	grandchild->balance = BALANCED;

	if (grandchild->links.parent = parent->links.parent)
		if (grandchild->links.parent->links.left == parent)
			grandchild->links.parent->links.left = grandchild;
		else
			grandchild->links.parent->links.right = grandchild;
	else
		(*root) = grandchild;

	if (parent->links.left = grandchild->links.right)
		parent->links.left->links.parent = parent;

	if (child->links.right = grandchild->links.left)
		child->links.right->links.parent = child;

	grandchild->links.right = parent;
	grandchild->links.left = child;

	parent->links.parent = grandchild;
	child->links.parent = grandchild;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::rebalance_grew(Node *this_node, Node **root)
{
	Node *previous_node = this_node;
	this_node = this_node->links.parent;

	while (this_node)
	{
		if (this_node->links.left == previous_node)
			if (this_node->balance == RIGHT_IS_HEAVY)
			{
				this_node->balance = BALANCED;
				return;
			}
			else
				if (this_node->balance == BALANCED)
					this_node->balance = LEFT_IS_HEAVY;
				else
				{
					if (previous_node->balance == LEFT_IS_HEAVY)
						right_right_rotation(this_node, previous_node, root);
					else
						left_right_rotation(this_node, previous_node, previous_node->links.right, root);

					return;
				}
		else
			if (this_node->balance == LEFT_IS_HEAVY)
			{
				this_node->balance = BALANCED;
				return;
			}
			else
				if (this_node->balance == BALANCED)
					this_node->balance = RIGHT_IS_HEAVY;
				else
				{
					if (previous_node->balance == RIGHT_IS_HEAVY)
						left_left_rotation(this_node, previous_node, root);
					else
						right_left_rotation(this_node, previous_node, previous_node->links.left, root);

					return;
				}

		previous_node = this_node;
		this_node = this_node->links.parent;
	}
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::rebalance_shrunk(Node *this_node, Node **root)
{
	Links previous_node_links = {};
	Node *previous_node = 0;
	Node *node_to_remove = this_node;
	this_node = this_node->links.left;

	while (this_node)
	{
		previous_node = this_node;
		this_node = this_node->links.right;
	}

	if (previous_node == 0)
		previous_node = node_to_remove->links.right;

	if (previous_node)
	{
		if (previous_node->links.parent == node_to_remove)
			this_node = previous_node;
		else
			this_node = previous_node->links.parent;

		previous_node_links = previous_node->links;
		previous_node->balance = node_to_remove->balance;
		previous_node->links = node_to_remove->links;

		if (previous_node->links.parent == 0)
			(*root) = previous_node;
		else
			if (previous_node->links.parent->links.left == node_to_remove)
				previous_node->links.parent->links.left = previous_node;
			else
				previous_node->links.parent->links.right = previous_node;
	}
	else
	{
		this_node = node_to_remove->links.parent;
		previous_node = node_to_remove;
	}

	if (this_node)
	{
		if (this_node->links.left == previous_node)
		{
			this_node->links.left = previous_node_links.left;

			if (this_node->balance == LEFT_IS_HEAVY)
				this_node->balance = BALANCED;
			else
				if (this_node->balance == BALANCED)
					this_node->balance = RIGHT_IS_HEAVY;
				else
					if (this_node->links.right->balance == RIGHT_IS_HEAVY)
					{
						node_to_remove = this_node->links.right;
						left_left_rotation(this_node, this_node->links.right, root);
						this_node = node_to_remove;
					}
					else
					{
						node_to_remove = this_node->links.right->links.left;
						right_left_rotation(this_node, this_node->links.right, this_node->links.right->links.left, root);
						this_node = node_to_remove;
					}
		}
		else
		{
			this_node->links.right = previous_node_links.right;

			if (this_node->balance == RIGHT_IS_HEAVY)
				this_node->balance = BALANCED;
			else
				if (this_node->balance == BALANCED)
					this_node->balance = LEFT_IS_HEAVY;
				else
					if (this_node->links.left->balance == LEFT_IS_HEAVY)
					{
						node_to_remove = this_node->links.left;
						right_right_rotation(this_node, this_node->links.left, root);
						this_node = node_to_remove;
					}
					else
					{
						node_to_remove = this_node->links.left->links.right;
						left_right_rotation(this_node, this_node->links.left, this_node->links.left->links.right, root);
						this_node = node_to_remove;
					}
		}

		previous_node = this_node;
		this_node = this_node->links.parent;

		while (this_node)
		{
			if (this_node->links.left == previous_node)
				if (this_node->balance == LEFT_IS_HEAVY)
					this_node->balance = BALANCED;
				else
					if (this_node->balance == BALANCED)
						this_node->balance = RIGHT_IS_HEAVY;
					else
						if (this_node->links.right->balance == RIGHT_IS_HEAVY)
						{
							node_to_remove = this_node->links.right;
							left_left_rotation(this_node, this_node->links.right, root);
							this_node = node_to_remove;
						}
						else
						{
							node_to_remove = this_node->links.right->links.left;
							right_left_rotation(this_node, this_node->links.right, this_node->links.right->links.left, root);
							this_node = node_to_remove;
						}
			else
				if (this_node->balance == RIGHT_IS_HEAVY)
					this_node->balance = BALANCED;
				else
					if (this_node->balance == BALANCED)
						this_node->balance = LEFT_IS_HEAVY;
					else
						if (this_node->links.left->balance == LEFT_IS_HEAVY)
						{
							node_to_remove = this_node->links.left;
							right_right_rotation(this_node, this_node->links.left, root);
							this_node = node_to_remove;
						}
						else
						{
							node_to_remove = this_node->links.left->links.right;
							left_right_rotation(this_node, this_node->links.left, this_node->links.left->links.right, root);
							this_node = node_to_remove;
						}

			previous_node = this_node;
			this_node = this_node->links.parent;
		}
	}
	else
		(*root) = 0;
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::destroy_subtree(Node **subtree_root)
{
	if ((*subtree_root) == 0)
		return;

	Node *this_node = (*subtree_root);

	while (true)
		if (this_node->links.right)
			this_node = this_node->links.right;
		else
			if (this_node->links.left)
				this_node = this_node->links.left;
			else
			{
				Node *node_to_delete = this_node;
				this_node = this_node->links.parent;

				delete node_to_delete;

				if (node_to_delete == (*subtree_root) || this_node == 0)
					break;
				else
					if (node_to_delete == this_node->links.right)
						this_node->links.right = 0;
					else
						this_node->links.left = 0;
			}

	(*subtree_root) = 0;
}

template <typename Key, typename Value>
typename AvlTree<Key, Value>::Node **AvlTree<Key, Value>::search_routine(const key_type &key) const
{
	int32_t res;
	Node **this_node_pointer = const_cast<Node **>(&m_root);

	while ((*this_node_pointer) && (res = m_compareKeys((*this_node_pointer)->key, key)) != 0)
	{
		if (res < 0)
			this_node_pointer = &(*this_node_pointer)->links.right;
		else
			this_node_pointer = &(*this_node_pointer)->links.left;
	}

	return this_node_pointer;
}

template <typename Key, typename Value>
typename AvlTree<Key, Value>::Node **AvlTree<Key, Value>::search_routine(const key_type &key, Node **out_parent_node) const
{
	int32_t res;
	Node *this_parent_node = 0;
	Node **this_node_pointer = const_cast<Node **>(&m_root);

	while ((*this_node_pointer) && (res = m_compareKeys((*this_node_pointer)->key, key)) != 0)
	{
		this_parent_node = (*this_node_pointer);

		if (res < 0)
			this_node_pointer = &(*this_node_pointer)->links.right;
		else
			this_node_pointer = &(*this_node_pointer)->links.left;
	}

	(*out_parent_node) = this_parent_node;
	return this_node_pointer;
}

template <typename Key, typename Value>
typename AvlTree<Key, Value>::value_type &AvlTree<Key, Value>::search_or_create_node(const key_type &key)
{
	Node *parent_node = 0;
	Node **this_node = search_routine(key, &parent_node);

	if ((*this_node))
		return (*this_node)->value;
	else
	{
		if (((*this_node) = parent_node = new Node(key, parent_node)))
			rebalance_grew(parent_node, &m_root);

		return parent_node->value;
	}
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::search_or_create_node(const key_type &key, const value_type &value)
{
	Node *parent_node = 0;
	Node **this_node = search_routine(key, &parent_node);

	if ((*this_node))
		(*this_node)->value = value;
	else
		if (((*this_node) = parent_node = new Node(key, value, parent_node)))
			rebalance_grew(parent_node, &m_root);
}

template <typename Key, typename Value>
void AvlTree<Key, Value>::delete_node(const key_type &key)
{
	if (Node *this_node = *search_routine(key))
	{
		rebalance_shrunk(this_node, &m_root);
		delete this_node;
	}
}

#endif /* AVLTREE_H_ */
