#include "filesystemtasksnodedelegate.h"
#include "../tools/filesystemcommontools.h"
#include <QtGui/QApplication>


FILE_SYSTEM_NS_BEGIN

TasksNodeDelegate::TasksNodeDelegate(QObject *parent) :
	Delegate(parent)
{}

void TasksNodeDelegate::paintProgressInMb(const TaskNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const
{
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
	}
	else
	{
		QStyleOptionProgressBarV2 progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = entry->progress();

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
	}
}

void TasksNodeDelegate::paintProgressInMb(const TaskNodeListItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const
{
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
	}
	else
	{
		QStyleOptionProgressBarV2 progressBarOption;
		progressBarOption.rect = option.rect;
		progressBarOption.minimum = 0;
		progressBarOption.maximum = 100;
		progressBarOption.progress = entry->progress();

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
	}
}

FILE_SYSTEM_NS_END
