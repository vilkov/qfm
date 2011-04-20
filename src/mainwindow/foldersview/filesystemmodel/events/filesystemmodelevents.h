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
		FirstListFiles = QEvent::User + 2,
		UpdateFiles = QEvent::User + 3,
		ScanFilesForRemove = QEvent::User + 4,
		RemoveFilesComplete = QEvent::User + 5,
		RemoveFilesCanceled = QEvent::User + 6,
		ScanFilesForSize = QEvent::User + 7,
		ScanFilesForCopy = QEvent::User + 8,
		CopyFilesComplete = QEvent::User + 9,
		CopyFilesCanceled = QEvent::User + 10,
		ScanFilesForMove = QEvent::User + 11,
		QuestionAnswer = QEvent::User + 12,
		NewEntry = QEvent::User + 13
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
