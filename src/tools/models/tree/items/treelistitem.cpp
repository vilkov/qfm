#include "treelistitem.h"


MODELS_NS_BEGIN

TreeListItem::TreeListItem(TreeItem *parent) :
	TreeItem(parent)
{}

TreeListItem::~TreeListItem()
{
	qDeleteAll(m_items);
}

bool TreeListItem::isList() const
{
	return true;
}

MODELS_NS_END
