#include "treeitem.h"


MODELS_NS_BEGIN

TreeItem::TreeItem(TreeItem *parent) :
	m_parent(parent)
{}

TreeItem::~TreeItem()
{}

MODELS_NS_END
