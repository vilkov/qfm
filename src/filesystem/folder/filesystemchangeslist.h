#ifndef FILESYSTEMCHANGESLIST_H_
#define FILESYSTEMCHANGESLIST_H_

#include <QtCore/QList>
#include "info/filesystemfoldernodeinfo.h"
#include "items/filesystemfoldernodeitem.h"


FILE_SYSTEM_NS_BEGIN

class Change
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
	Change(Type type, FolderNodeItem *entry) :
		m_type(type),
		m_entry(entry)
	{}
	Change(Type type, const Info &info) :
		m_type(type),
		m_entry(0),
		m_info(info)
	{}
	Change(Type type, FolderNodeItem *entry, const Info &info) :
		m_type(type),
		m_entry(entry),
		m_info(info)
	{}

	Type type() const { return m_type; }
	FolderNodeItem *entry() const { return m_entry; }

	const Info &info() const { return m_info; }
	void setInfo(const Info &info) { m_info = info; }

private:
	Type m_type;
	Info m_info;
	FolderNodeItem *m_entry;
};
typedef QList<Change> ChangesList;

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCHANGESLIST_H_ */
