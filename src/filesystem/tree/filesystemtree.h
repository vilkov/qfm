#ifndef FILESYSTEMTREE_H_
#define FILESYSTEMTREE_H_

#include "filesystementry.h"
#include "../../tools/metatemplates.h"


class FileSystemTree : public FileSystemEntry
{
	Q_DISABLE_COPY(FileSystemTree)

public:
	FileSystemTree(FileSystemItem *parent = 0) :
		FileSystemEntry(parent)
	{}

	virtual FileSystemItem *child(size_type index) const { return m_childs.at(index).entry; }
	virtual size_type size() const { return m_childs.size(); }
	virtual size_type indexOf(FileSystemItem *item) const { return m_childs.indexOf(item); }
	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }

	virtual FileSystemTree *activated(size_type index) = 0;
	virtual FileSystemTree *activated(FileSystemEntry *entry) = 0;

protected:
	struct Value : public MemoryManagerTag
	{
		Value() :
			entry(0),
			subtree(0)
		{}
		Value(FileSystemEntry *entry) :
			entry(entry),
			subtree(0)
		{}

		bool operator==(const Value &other) const
		{
			return entry == other.entry;
		}

		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
    typedef QList<Value> ValueList;

protected:
	template <typename T, typename S> void add(const S &value)
	{
		using namespace Templates;
		must_have_base<T, FileSystemEntry>();
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
	ValueList m_childs;
};

#endif /* FILESYSTEMTREE_H_ */
