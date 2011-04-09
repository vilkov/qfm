#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QEvent>
#include "../../../../tools/memory/memory_manager.h"


class FileSystemModelEvent : public QEvent
{
public:
	struct Params : public MemoryManagerTag
	{};

	enum EventType
	{
		ListFilesType = QEvent::User + 1,
		ChangesListType = QEvent::User + 2,
		PopulateFilesForRemoveType = QEvent::User + 3,
		PopulateFilesForSizeType = QEvent::User + 4
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
