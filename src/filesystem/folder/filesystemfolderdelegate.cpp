#include "filesystemfolderdelegate.h"
#include "items/filesystemfoldernodeentry.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

FolderDelegate::FolderDelegate(QAbstractProxyModel *proxy, QObject *parent) :
	Delegate(parent),
    m_proxy(proxy)
{
	Q_ASSERT(m_proxy != 0);
}

void FolderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == 1)
	{
		QModelIndex idx = m_proxy->mapToSource(index);

		if (!static_cast<FolderNodeItem*>(idx.internalPointer())->isRootItem() &&
			static_cast<FolderNodeEntry*>(idx.internalPointer())->isInProgress())
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(idx.internalPointer());

			if (entry->isCompleted())
			{
				QStyleOptionProgressBarV2 progressBarOption;
				progressBarOption.rect = option.rect;
				progressBarOption.minimum = 0;
				progressBarOption.maximum = 100;
				progressBarOption.progress = 100;
				progressBarOption.text = FolderNodeEntry::humanReadableTime(entry->timeElapsed().toULongLong());
				progressBarOption.textAlignment = Qt::AlignCenter;
				progressBarOption.textVisible = true;

				if (const QStyleOptionViewItemV3 *v3 = qstyleoption_cast<const QStyleOptionViewItemV3 *>(&option))
					v3->widget->style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
				else
					QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);

				return;
			}
			else
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

	Delegate::paint(painter, option, index);
}

//QSize FolderDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//	QSize size = QStyledItemFolderDelegate::sizeHint(option, index);
//	size.setHeight(size.height() + 3);
//	return size;
//}

FILE_SYSTEM_NS_END
