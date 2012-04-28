#include "defaultnoderootitem.h"


DEFAULT_PLUGIN_NS_BEGIN
static IFileInfo::Holder emptyInfo;


DefaultNodeRootItem::DefaultNodeRootItem(Base *parent) :
	DefaultNodeItem(emptyInfo, parent),
	m_label(QString::fromLatin1(".."))
{}

QVariant DefaultNodeRootItem::data(qint32 column, qint32 role) const
{
	if (column == 0 && role == Qt::DisplayRole)
		return m_label;
	else
		return QVariant();
}

bool DefaultNodeRootItem::isRootItem() const
{
	return true;
}

DEFAULT_PLUGIN_NS_END
