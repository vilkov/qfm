#ifndef FILESYSTEMTREE_H_
#define FILESYSTEMTREE_H_

#include <QFileInfo>
#include "filesystemitem.h"
#include "filesystementry.h"
#include "filesystemroot.h"
#include "../filesystemchangeslist.h"
#include "../../../../tools/metatemplates.h"


class FileSystemTree : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemTree)

private:
	struct FileSystemPair : public MemoryManagerTag
	{
		FileSystemPair() :
			entry(0),
			subtree(0)
		{}
		FileSystemPair(FileSystemItem *entry) :
			entry(entry),
			subtree(0)
		{}

		FileSystemItem *entry;
		FileSystemItem *subtree;
	};
    typedef QList<FileSystemPair> value_type;

public:
	FileSystemTree(const QString &directory, FileSystemItem *parent = 0) :
		FileSystemItem(parent),
		m_fileInfo(directory),
		m_parentEntry(0)
	{
		if (!m_fileInfo.isRoot())
			m_childs.push_back(new FileSystemRoot(this));
	}
	FileSystemTree(const QFileInfo &info, FileSystemItem *parent = 0) :
		FileSystemItem(parent),
		m_fileInfo(info),
		m_parentEntry(0)
	{
		if (!m_fileInfo.isRoot())
			m_childs.push_back(new FileSystemRoot(this));
	}

	virtual ~FileSystemTree()
	{
		for (value_type::size_type i = 0, size = m_childs.size(); i < size; ++i)
		{
			delete m_childs.at(i).entry;
			delete m_childs.at(i).subtree;
		}
	}

	virtual FileSystemItem *child(size_type index) const { return m_childs.at(index).entry; }
	virtual size_type size() const { return m_childs.size(); }
	virtual size_type indexOf(FileSystemItem *item) const
	{
		for (value_type::size_type i = 0, size = m_childs.size(); i < size; ++i)
			if (m_childs.at(i).entry == item)
				return static_cast<size_type>(i);

		return InvalidIndex;
	}
	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }

	FileSystemItem *last() const { return m_childs.last().entry; }

	const QFileInfo &fileInfo() const { return m_fileInfo; }
	QFileInfo &fileInfo() { return m_fileInfo; }

	FileSystemItem *parentEntry() const { return m_parentEntry; }
	void setParentEntry(FileSystemItem *value) { m_parentEntry = value; }

	FileSystemItem *subtree(FileSystemItem *entry) const
	{
		Q_ASSERT(indexOf(entry) != InvalidIndex);
		return m_childs.at(indexOf(entry)).subtree;
	}
	void setSubtree(FileSystemItem *entry, FileSystemItem *tree)
	{
		Q_ASSERT(indexOf(entry) != InvalidIndex);
		FileSystemPair &pair = m_childs[indexOf(entry)];
		delete pair.subtree;
		pair.subtree = tree;
	}
	void setSubtree(FileSystemTree *tree)
	{
		Q_ASSERT(!m_childs.isEmpty());
		FileSystemPair &pair = m_childs.last();
		delete pair.subtree;
		pair.subtree = tree;
		tree->setParent(this);
	}

	ChangesList makeChangeSet() const
	{
		ChangesList list;

		if (m_fileInfo.isRoot())
		{
			list.reserve(m_childs.size());

			for (size_type i = 0, size = m_childs.size(); i < size; ++i)
				list.push_back(Change(Change::NoChange, m_childs.at(i).entry));
		}
		else
		{
			list.reserve(m_childs.size() - 1);

			for (size_type i = 1, size = m_childs.size(); i < size; ++i)
				list.push_back(Change(Change::NoChange, m_childs.at(i).entry));
		}

		return list;
	}

	template <typename T, typename S> void add(const S &value)
	{
		using namespace Templates;
		must_have_base<T, FileSystemItem>();
		addItem<T, S>(value, int_to_type<has_value_type<S>::value>());
	}
	void remove(size_type index)
	{
		Q_ASSERT(index != InvalidIndex);
		delete m_childs.at(index).entry;
		delete m_childs.at(index).subtree;
		m_childs.removeAt(index);
	}

protected:
	template <typename T, typename S> void addItem(const S &items, Templates::int_to_type<1>)
	{
		for (typename S::size_type i = 0, size = items.size(); i < size; ++i)
			m_childs.push_back(new T(items.at(i), this));
	}
	template <typename T, typename S> void addItem(const S &item, Templates::int_to_type<0>)
	{
		m_childs.push_back(new T(item, this));
	}

private:
	QFileInfo m_fileInfo;
	value_type m_childs;
	FileSystemItem *m_parentEntry;
};

#endif /* FILESYSTEMTREE_H_ */
