#ifndef FILESYSTEMFOLDERNODEROOT_H_
#define FILESYSTEMFOLDERNODEROOT_H_

#include "filesystemfoldernodeitem.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeRoot : public FolderNodeItem
{
public:
	explicit FolderNodeRoot(const Info &info) :
		FolderNodeItem(info),
		m_label(QString::fromLatin1(".."))
	{}

	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_label;
		else
			return QVariant();
	}
	virtual bool isRootItem() const { return true; }

private:
	QVariant m_label;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEROOT_H_ */
