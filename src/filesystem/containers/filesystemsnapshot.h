#ifndef FILESYSTEMSNAPSHOT_H_
#define FILESYSTEMSNAPSHOT_H_

#include <QtCore/QMap>
#include <QtCore/QSharedData>
#include "filesystemwrappednodeitem.h"


FILE_SYSTEM_NS_BEGIN

class Snapshot
{
public:
	typedef QPair<NodeItem *, WrappedNodeItem *> Pair;
	typedef QMap<QString, Pair>                  Container;

public:
	Snapshot(const IFileContainer *container);

	IFileInfo::size_type totalSize() const { return m_data->totalSize; }
	const IFileContainer *container() const { return m_data->m_container; }

	NodeItem *exists(const QString &fileName) const { return m_data->list.value(fileName).first; }

//	void push_back(NodeItem *item, InfoItem *info) { m_data->list.push_back(Container::value_type(item, info)); m_data->totalSize += info->totalSize(); }
//	void remove(Container::size_type index) { InfoItem *info = m_data->list.takeAt(index).second; m_data->totalSize -= info->totalSize(); delete info; }

private:
	struct Data : public QSharedData
	{
		Data(const IFileContainer *container);
		Data(const IFileContainer *container, Container::size_type reserver);
		~Data();

		Container list;
		IFileInfo::size_type totalSize;
		const IFileContainer *m_container;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMSNAPSHOT_H_ */
