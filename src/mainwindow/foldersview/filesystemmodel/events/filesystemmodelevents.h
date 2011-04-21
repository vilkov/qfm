#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QEvent>


class FileSystemModelEvent : public QEvent
{
public:
	struct EventParams {};

	enum EventType
	{
		ListFiles = QEvent::User + 1,
		UpdateFiles = QEvent::User + 2,
		ScanFilesForRemove = QEvent::User + 3,
		RemoveFilesComplete = QEvent::User + 4,
		RemoveFilesCanceled = QEvent::User + 5,
		ScanFilesForSize = QEvent::User + 6,
		ScanFilesForCopy = QEvent::User + 7,
		CopyFilesComplete = QEvent::User + 8,
		CopyFilesCanceled = QEvent::User + 9,
		ScanFilesForMove = QEvent::User + 10,
		QuestionAnswer = QEvent::User + 11
	};

public:
	FileSystemModelEvent(EventType type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}

	virtual const EventParams *parameters() const = 0;
};


template <typename T>
class FileSystemModelEventTemplate : public FileSystemModelEvent
{
public:
	FileSystemModelEventTemplate(EventType type) :
		FileSystemModelEvent(type)
	{}

	virtual const EventParams *parameters() const { return &m_params; }

	const T &params() const { return m_params; }
	T &params() { return m_params; }

private:
	T m_params;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
