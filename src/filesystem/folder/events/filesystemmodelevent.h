#ifndef FILESYSTEMMODELEVENT_H_
#define FILESYSTEMMODELEVENT_H_

#include <QtCore/QEvent>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class ModelEvent : public QEvent
{
	Q_DISABLE_COPY(ModelEvent)

public:
	enum Type
	{
		UpdateFiles = QEvent::User + 1,
		ScanFilesForSize = QEvent::User + 2,
		ScanFilesForRemove = QEvent::User + 3,
		ScanFilesForCopy = QEvent::User + 4,
		RemoveFiles = QEvent::User + 5,
		CopyFiles = QEvent::User + 6,
		QuestionAnswer = QEvent::User + 7,
		UpdateProgress = QEvent::User + 8,
		CompletedProgress = QEvent::User + 9
	};

protected:
	ModelEvent(Type type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMMODELEVENT_H_ */
