#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QEvent>


class FileSystemModelEvent : public QEvent
{
public:
	struct Params {};

	enum EventType
	{
		ListFiles = QEvent::User + 1,
		UpdateFiles = QEvent::User + 2,
		ScanFilesForRemove = QEvent::User + 3,
		RemoveFilesComplete = QEvent::User + 4,
		RemoveFilesCanceled = QEvent::User + 5,
		ScanFilesForSize = QEvent::User + 6,
		ScanFilesForCopy = QEvent::User + 7,
		ScanFilesForMove = QEvent::User + 8,
		QuestionAnswer = QEvent::User + 9
	};

public:
	FileSystemModelEvent(EventType type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}

	virtual const Params *parameters() const = 0;
};


template <typename T>
class FileSystemModelEventTemplate : public FileSystemModelEvent
{
public:
	FileSystemModelEventTemplate(EventType type) :
		FileSystemModelEvent(type)
	{}

	virtual const Params *parameters() const { return &m_params; }

	const T &params() const { return m_params; }
	T &params() { return m_params; }

private:
	T m_params;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
