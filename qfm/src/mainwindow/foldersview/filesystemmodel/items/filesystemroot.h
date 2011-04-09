#ifndef FILESYSTEMROOT_H_
#define FILESYSTEMROOT_H_

#include <QVariant>
#include "filesystemitem.h"


class FileSystemRoot : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemRoot)

public:
	FileSystemRoot(FileSystemItem *parent) :
		FileSystemItem(parent),
		m_directory(QString::fromLatin1(".."))
	{}

	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0)
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_directory;
			}

		return QVariant();
	}
	virtual bool isRoot() const { return true; }

private:
	QVariant m_directory;
};

#endif /* FILESYSTEMROOT_H_ */
