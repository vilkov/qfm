#include "arcdelegate.h"


ARC_PLUGIN_NS_BEGIN

ArcDelegate::ArcDelegate(QAbstractProxyModel *proxy, QObject *parent) :
	TasksNodeDelegate(parent),
	m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void ArcDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		QModelIndex idx = m_proxy->mapToSource(index);

		if (static_cast<TaskNodeItem::Base *>(idx.internalPointer())->isList())
		{
			const TaskNodeListItem *entry = static_cast<const TaskNodeListItem *>(idx.internalPointer());

			if (entry->isInProgress())
				paintProgressInMb(entry, painter, option);
		}
		else
		{
			TaskNodeItem *entry = static_cast<TaskNodeItem *>(idx.internalPointer());

			if (entry->isInProgress())
				paintProgressInMb(entry, painter, option);
		}
	}

	TasksNodeDelegate::paint(painter, option, index);
}

void ArcDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize ArcDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

ARC_PLUGIN_NS_END
