#ifndef FILESYSTEMITEMSCONTAINER_H_
#define FILESYSTEMITEMSCONTAINER_H_

#include "../base/items/filesystembaseitem.h"
#include "../../model/filesystemmodelcontainer.h"
#include "../../../tools/containers/hashedlist.h"


FILE_SYSTEM_NS_BEGIN

class ItemsContainer : public ModelContainer
{
public:
	typedef FileSystemBaseItem * value_type;

public:
	ItemsContainer();
	virtual ~ItemsContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

	const value_type &operator[](size_type index) const { return m_container.at(index); }

	const value_type &last() const { return m_container.last(); }
	value_type &last() { return m_container.last(); }

	size_type lastIndex() const { return m_container.size() - 1; }
	size_type indexOf(Node *item) const
	{
		for (List::size_type i = 0, size = m_container.size(); i < size; ++i)
			if (m_container.at(i)->node() == item)
				return i;

		return InvalidIndex;
	}
	size_type indexOf(const QString &fileName) const { return m_container.indexOf(fileName); }

	void add(FileSystemBaseItem *item) { m_container.add(item->fileName(), item); }
	void remove(size_type index) { m_container.remove(index); }
	value_type take(size_type index) { return m_container.take(index); }
	void replace(size_type index, const QString &oldHash, const QString &newHash) { m_container.replace(index, oldHash, newHash); }

private:
	typedef ::Tools::Containers::HashedList<QString, value_type> List;

private:
	List m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMSCONTAINER_H_ */
