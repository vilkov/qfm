#include "filesystemtasksnodedelegate.h"
#include "items/filesystemtasknodeitemlist.h"
#include "../tools/filesystemcommontools.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

TasksNodeDelegate::TasksNodeDelegate(QAbstractProxyModel *proxy, uint progressColumn, QObject *parent) :
	Delegate(parent),
    m_proxy(proxy),
    m_progressColumn(progressColumn)
{
	Q_ASSERT(m_proxy != 0);
}

void TasksNodeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.column() == m_progressColumn)
	{
		QModelIndex idx = m_proxy->mapToSource(index);

		if (static_cast<TaskNodeItem::Base *>(idx.internalPointer())->isList())
		{
			TaskNodeListItem *entry = static_cast<TaskNodeListItem *>(idx.internalPointer());

			if (entry->isInProgress())
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
		else
		{
			TaskNodeItem *entry = static_cast<TaskNodeItem *>(idx.internalPointer());

			if (entry->isInProgress())
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
