#ifndef STDAVLTREE_H_
#define STDAVLTREE_H_

#include "avltree.h"


/*
 * Declaration
 *
 */

template <typename Key, typename Value, typename SizeType = unsigned int>
class StdAvlTree : protected AvlTree<Key, SizeType>
{
public:
	typedef AvlTree<Key, SizeType>        base_type;
	typedef typename base_type::key_type  key_type;
	typedef typename base_type::size_type size_type;
	typedef Value                         value_type;

	//----------------------------------------------------------
	struct StdAvlNode : public base_type::AvlNode
	{
		value_type value;

		StdAvlNode(const key_type &key, AvlNode *parent_node) :
			base_type::AvlNode(key, parent_node)
		{}
	};

	//----------------------------------------------------------
	StdAvlTree() :
		base_type()
	{}

    //----------------------------------------------------------
	value_type &operator[](const key_type &key)
    {
		return static_cast<StdAvlNode*>(search_or_create_node(key))->value;
    }

    //----------------------------------------------------------
    const value_type operator[](const key_type &key) const
    {
		if (base_type::AvlNode *node = search_node(key))
			return static_cast<StdAvlNode*>(node)->value;
		else
			return value_type();
    }

    //----------------------------------------------------------
	const value_type value(const key_type &key, const value_type &defaultValue = value_type()) const
	{
		if (base_type::AvlNode *node = search_node(key))
			return static_cast<StdAvlNode*>(node)->value;
		else
			return defaultValue;
	}

	//----------------------------------------------------------
	bool contains(const key_type &key) const
	{
		return search_node(key);
	}

	//----------------------------------------------------------
	void remove(const key_type &key)
	{
		delete_node(key);
	}

	//----------------------------------------------------------
	size_type size() const
	{
		return base_type::size();
	}

protected:
	virtual AvlNode* create_node(const key_type &value, AvlNode *&parent) = 0;

private:
	//----------------------------------------------------------
	// Очистка дерева
	//----------------------------------------------------------
	inline void destroy_tree()
	{
		destroy_subtree(tree_root);
	}

	//---------------------------------------------------
	void destroy_subtree(AvlNode *&subtree_root);

	//---------------------------------------------------
	// Вспомогательная функция, которая только ищет узел,
	// но больше ничего не делает
	//---------------------------------------------------
	static AvlNode **search_routine(
		const key_type &value_to_search,
		AvlNode *&root,
		AvlNode *&out_parent_node
	);

	//---------------------------------------------------
	// Механизм удаления узла
	//---------------------------------------------------
	void delete_routine(
		const key_type &value_to_delete,
		AvlNode *&root
	);

	//---------------------------------------------------
	// Функция ищет узел и создает его в случае,
	// если узла с заданным ключом нет
	//---------------------------------------------------
	AvlNode * __fastcall search_or_create(
		const key_type &value_to_search,
		AvlNode *&root
	);

	//---------------------------------------------------
	// Функция ищет узел
	//---------------------------------------------------
	static AvlNode * __fastcall search(
		const key_type &value_to_search,
		AvlNode *&root
	);

private:
	AvlNode *tree_root;
	size_type node_counter;
};

#endif /* STDAVLTREE_H_ */
