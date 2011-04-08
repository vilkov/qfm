#ifndef FILESYSTEMMODELEVENTS_H_
#define FILESYSTEMMODELEVENTS_H_

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include "../filesysteminfo.h"
#include "../filesystemchangeslist.h"
#include "../items/filesystemtree.h"


class FileSystemModelBaseEvent : public QEvent
{
public:
	enum EventType
	{
		NewFileInfo = QEvent::User + 1,
		UpdateFileInfo = QEvent::User + 2
	};

public:
	FileSystemModelBaseEvent(FileSystemTree *fileSystemTree, EventType type);

	FileSystemTree *fileSystemTree() const { return m_fileSystemTree; }

private:
	FileSystemTree *m_fileSystemTree;
};


class ListFilesEvent : public FileSystemModelBaseEvent
{
public:
	typedef QList<FileSystemInfo> value_type;

public:
	ListFilesEvent(FileSystemTree *fileSystemTree, const value_type &info);

	const value_type &info() const { return m_info; }

private:
	value_type m_info;
};


class ChangesListEvent : public FileSystemModelBaseEvent
{
public:
	typedef ChangesList value_type;

public:
	ChangesListEvent(FileSystemTree *fileSystemTree, const value_type &info);

	const value_type &info() const { return m_info; }

private:
	value_type m_info;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
