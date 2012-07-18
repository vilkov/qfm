#include "vfs_tasksnodedelegate.h"
#include "../../tools/vfs_commontools.h"
#include "../../../application.h"


VFS_NS_BEGIN

TasksNodeDelegate::TasksNodeDelegate(QObject *parent) :
	Delegate(parent)
{}

void TasksNodeDelegate::paintProgressInMb(const TasksNodeItem *entry, QPainter *painter, const QStyleOptionViewItem &option) const
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
			Application::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
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
			Application::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
	}
}

VFS_NS_END
