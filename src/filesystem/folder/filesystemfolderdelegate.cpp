#include "filesystemfolderdelegate.h"
#include "items/filesystementryitem.h"
#include "../tools/filesystemcommontools.h"
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

		if (!static_cast<FileSystemBaseItem*>(idx.internalPointer())->isRootItem() &&
			static_cast<FileSystemEntryItem*>(idx.internalPointer())->isInProgress())
		{
			FileSystemEntryItem *entry = static_cast<FileSystemEntryItem*>(idx.internalPointer());

			if (entry->isCompleted())
			{
				QStyleOptionProgressBarV2 progressBarOption;
				progressBarOption.rect = option.rect;
				progressBarOption.minimum = 0;
				progressBarOption.maximum = 100;
				progressBarOption.progress = 100;
				progressBarOption.text = Tools::humanReadableTime(entry->timeElapsed());
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
							Tools::humanReadableShortSize(entry->done()).
							append(QString::fromLatin1(" / ")).
							append(Tools::humanReadableShortSize(entry->total())).
							append(QString::fromLatin1("  ")).
							append(Tools::humanReadableTime((entry->total() / entry->done()) * entry->timeElapsed()));
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

FILE_SYSTEM_NS_END
