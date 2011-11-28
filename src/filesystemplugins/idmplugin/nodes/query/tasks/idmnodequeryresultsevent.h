#ifndef IDMNODEQUERYRESULTSEVENT_H_
#define IDMNODEQUERYRESULTSEVENT_H_

#include <QtCore/QEvent>
#include "../../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultsEvent : public QEvent
{
	Q_DISABLE_COPY(QueryResultsEvent)

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
	QueryResultsEvent(Type type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSEVENT_H_ */
