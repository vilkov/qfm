#ifndef FILESYSTEMMODELEVENT_H_
#define FILESYSTEMMODELEVENT_H_

#include <QtCore/QEvent>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class ModelEvent : public QEvent
{
public:
	enum Type
	{
		ListFiles = QEvent::User + 1,
		UpdateFiles = QEvent::User + 2,
		ScanFilesForRemove = QEvent::User + 3,
		RemoveFilesCompleted = QEvent::User + 4,
		RemoveFilesCanceled = QEvent::User + 5,
		ScanFilesForSize = QEvent::User + 6,
		ScanFilesForCopy = QEvent::User + 7,
		CopyFilesCompleted = QEvent::User + 8,
		QuestionAnswer = QEvent::User + 9,
		UpdatePerformProgress = QEvent::User + 10
	};

public:
	struct Params {};
	virtual const Params *parameters() const = 0;

protected:
	ModelEvent(Type type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMMODELEVENT_H_ */
