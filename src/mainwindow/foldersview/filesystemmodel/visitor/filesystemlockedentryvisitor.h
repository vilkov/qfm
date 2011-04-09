#ifndef FILESYSTEMLOCKEDENTRYVISITOR_H_
#define FILESYSTEMLOCKEDENTRYVISITOR_H_

#include <QMap>
#include <QList>
#include <QStringList>
#include "filesystemmodelvisitor.h"
#include "../items/filesystemtree.h"


class FileSystemLockedEntryVisitor : public FileSystemModelVisitor
{
public:
	typedef QList<const FileSystemEntry*>    EntryList;
	typedef QMap<FileSystemTree*, EntryList> EntryMap;

public:
	virtual void visit(const FileSystemEntry *entry)
	{
		if (entry->isLocked())
		{
			m_entrys[static_cast<FileSystemTree*>(entry->parent())].push_back(entry);
			m_directories.push_back(entry->fileInfo().absoluteFilePath());
		}
	}

	const EntryMap &entrys() const { return m_entrys; }
	const QStringList &directories() const { return m_directories; }

private:
	EntryMap m_entrys;
	QStringList m_directories;
};

#endif /* FILESYSTEMLOCKEDENTRYVISITOR_H_ */
