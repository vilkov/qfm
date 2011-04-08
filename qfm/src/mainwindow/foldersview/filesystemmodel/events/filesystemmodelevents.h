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
		ListFilesType = QEvent::User + 1,
		ChangesListType = QEvent::User + 2,
		PopulateFilesForRemoveType = QEvent::User + 3
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


class PopulateFilesForRemoveEvent : public FileSystemModelBaseEvent
{
public:
	PopulateFilesForRemoveEvent(FileSystemTree *fileSystemTree, FileSystemTree *subtree);

	FileSystemTree *subtree() const { return m_subtree; }

private:
	FileSystemTree *m_subtree;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
