#ifndef FILESYSTEMCHANGESLIST_H_
#define FILESYSTEMCHANGESLIST_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "items/filesystemitem.h"
#include "../../../filesysteminfo/filesysteminfo.h"
#include "../../../tools/memory/memory_manager.h"


class Change : public MemoryManagerTag
{
public:
	enum Type
	{
		Added,
		Deleted,
		Updated,
		NoChange
	};

public:
	Change() :
		m_type(NoChange),
		m_entry(0)
	{}
	Change(Type type, FileSystemItem *entry) :
		m_type(type),
		m_entry(entry)
	{}
	Change(Type type, const FileSystemInfo &info) :
		m_type(type),
		m_entry(0),
		m_info(info)
	{}
	Change(Type type, FileSystemItem *entry, const FileSystemInfo &info) :
		m_type(type),
		m_entry(entry),
		m_info(info)
	{}

	Type type() const { return m_type; }
	FileSystemItem *entry() const { return m_entry; }

	const FileSystemInfo &info() const { return m_info; }
	void setInfo(const FileSystemInfo &info) { m_info = info; }

private:
	Type m_type;
	FileSystemItem *m_entry;
	FileSystemInfo m_info;
};
typedef QList<Change> ChangesList;

#endif /* FILESYSTEMCHANGESLIST_H_ */
