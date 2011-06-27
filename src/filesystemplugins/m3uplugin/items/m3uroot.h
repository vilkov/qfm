#ifndef M3UROOT_H_
#define M3UROOT_H_

#include <QtCore/QFileInfo>
#include "m3uitem.h"


class M3uRoot : public M3uItem
{
public:
	M3uRoot(const QFileInfo &info) :
		m_info(info),
		m_label(QString::fromLatin1(".."))
	{}

	/* IFileInfo */
	virtual bool isDir() const { return m_info.isDir(); }
	virtual bool isFile() const { return m_info.isFile(); }
	virtual bool exists() const { return m_info.exists(); }
	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QString absoluteFilePath(const QString &fileName) const { return QString(); }
	virtual QDateTime lastModified() const { return m_info.lastModified(); }
	virtual bool exists(IFileInfo *info) const { return false; }
	virtual void refresh() {}

	/* M3uItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return m_label;
		else
			return QVariant();
	}
	virtual bool isRoot() const { return true; }

private:
	QFileInfo m_info;
	QString m_label;
};

#endif /* M3UROOT_H_ */
