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
		PopulateFilesForRemoveType = QEvent::User + 3,
		PopulateFilesForSizeType = QEvent::User + 4
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
	PopulateFilesForRemoveEvent(FileSystemTree *fileSystemTree, FileSystemEntry *entry, FileSystemTree *subtree);

	FileSystemEntry *entry() const { return m_entry; }
	FileSystemTree *subtree() const { return m_subtree; }

private:
	FileSystemEntry *m_entry;
	FileSystemTree *m_subtree;
};


class PopulateFilesForSizeEvent : public FileSystemModelBaseEvent
{
public:
	PopulateFilesForSizeEvent(FileSystemTree *fileSystemTree, FileSystemEntry *entry, quint64 size);

	quint64 size() { return m_size; }
	FileSystemEntry *entry() const { return m_entry; }

private:
	quint64 m_size;
	FileSystemEntry *m_entry;
};

#endif /* FILESYSTEMMODELEVENTS_H_ */
