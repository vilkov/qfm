#include "arc_delegate.h"


ARC_PLUGIN_NS_BEGIN

Delegate::Delegate(QAbstractProxyModel *proxy, QObject *parent) :
	TasksNodeDelegate(parent),
	m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		TasksNodeItem *entry = static_cast<TasksNodeItem *>(m_proxy->mapToSource(index).internalPointer());

		if (entry->isInProgress())
		{
			paintProgressInMb(entry, painter, option);
			return;
		}
	}

	TasksNodeDelegate::paint(painter, option, index);
}

void Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

ARC_PLUGIN_NS_END
