#ifndef FILESYSTEMTREE_H_
#define FILESYSTEMTREE_H_

#include "filesystemitem.h"
#include "filesystementry.h"
#include "filesystemroot.h"
#include "../filesystemchangeslist.h"
#include "../../../../tools/metatemplates.h"


class FileSystemTree : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemTree)

public:
	FileSystemTree(const QString &directory, FileSystemItem *parent = 0);
	FileSystemTree(const FileSystemInfo &info, FileSystemItem *parent = 0);
	virtual ~FileSystemTree();

	virtual FileSystemItem *child(size_type index) const { return m_childs.at(index).entry; }
	virtual size_type size() const { return m_childs.size(); }
	virtual size_type indexOf(FileSystemItem *item) const;
	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }
	virtual void accept(FileSystemModelVisitor *visitor) const;

	FileSystemItem *last() const { return m_childs.last().entry; }

	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

	const FileSystemInfo &fileInfo() const { return m_fileInfo; }
	FileSystemInfo &fileInfo() { return m_fileInfo; }

	FileSystemItem *parentEntry() const { return m_parentEntry; }
	void setParentEntry(FileSystemItem *value) { m_parentEntry = value; }

	FileSystemItem *subtree(FileSystemItem *entry) const;
	void setSubtree(FileSystemItem *entry, FileSystemItem *tree);
	void setSubtree(FileSystemTree *tree);

	ChangesList makeChangeSet() const;

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

private:
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
	bool m_updating;
	FileSystemInfo m_fileInfo;
	value_type m_childs;
	FileSystemItem *m_parentEntry;
};

#endif /* FILESYSTEMTREE_H_ */
