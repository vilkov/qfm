#ifndef FILESYSTEMFOLDERNODEROOT_H_
#define FILESYSTEMFOLDERNODEROOT_H_

#include "filesystemfoldernodeitem.h"
#include "../../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeRoot : public FolderNodeItem
{
	Q_DISABLE_COPY(FolderNodeRoot)

public:
	FolderNodeRoot(const Info &info) :
		m_info(info),
		m_label(QString::fromLatin1(".."))
	{}

	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_label;
		else
			return QVariant();
	}
	virtual bool isRoot() const { return true; }

	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }

private:
	Info m_info;
	QVariant m_label;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEROOT_H_ */
