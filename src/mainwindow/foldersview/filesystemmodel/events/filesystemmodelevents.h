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
		ScanFilesForSize = QEvent::User + 4,
		ScanFilesForCopy = QEvent::User + 5,
		ScanFilesForMove = QEvent::User + 6,
		QuestionAnswer = QEvent::User + 7
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
