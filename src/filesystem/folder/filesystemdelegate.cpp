#include "filesystemdelegate.h"
#include "items/filesystemfoldernodeentry.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

Delegate::Delegate(QAbstractProxyModel *proxy, QObject *parent) :
	QStyledItemDelegate(parent),
    m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		QModelIndex idx = m_proxy->mapToSource(index);

		if (!static_cast<FolderNodeItem*>(idx.internalPointer())->isRoot() &&
			static_cast<FolderNodeEntry*>(idx.internalPointer())->isInProgress())
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(idx.internalPointer());

			if (qint32 progress = entry->progress())
			{
				QStyleOptionProgressBarV2 progressBarOption;
				progressBarOption.rect = option.rect;
				progressBarOption.minimum = 0;
				progressBarOption.maximum = 100;
				progressBarOption.progress = progress;
				progressBarOption.text =
						FolderNodeEntry::humanReadableShortSize(entry->doneSize().toULongLong()).
						append(QString::fromLatin1(" / ")).
						append(FolderNodeEntry::humanReadableShortSize(entry->totalSize().toULongLong())).
						append(QString::fromLatin1("  ")).
						append(FolderNodeEntry::humanReadableTime((entry->totalSize().toULongLong() / entry->doneSize().toULongLong()) * entry->timeElapsed().toULongLong()));
				progressBarOption.textAlignment = Qt::AlignCenter;
				progressBarOption.textVisible = true;

				if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
					v3->widget->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
				else
					QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

				return;
			}
		}
	}

	QStyleOptionViewItemV4 opt = option;
	initStyleOption(&opt, index);

	if (index.row() % 2 == 0)
		opt.backgroundBrush = QBrush(Qt::white);
	else
		opt.backgroundBrush = QBrush(QColor(245, 245, 245));

	if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
		v3->widget->style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, v3->widget);
	else
		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
}

//QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QSize size = QStyledItemDelegate::sizeHint(option, index);
//	size.setHeight(size.height() + 3);
//	return size;
//}

FILE_SYSTEM_NS_END
