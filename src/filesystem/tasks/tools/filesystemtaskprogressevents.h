#ifndef FILESYSTEMTASKPROGRESSEVENTS_H_
#define FILESYSTEMTASKPROGRESSEVENTS_H_

#include "../filesystembasetask.h"


FILE_SYSTEM_NS_BEGIN

class UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(BaseTask::Event::Progress),
		fileName(fileName),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	QString fileName;
	quint64 progress;
	quint64 timeElapsed;
};


class CompletedProgressEvent : public BaseTask::Event
{
public:
	CompletedProgressEvent(const QString &fileName, quint64 timeElapsed) :
		BaseTask::Event(BaseTask::Event::Completed),
		fileName(fileName),
		timeElapsed(timeElapsed)
	{}

	QString fileName;
	quint64 timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKPROGRESSEVENTS_H_ */
