#ifndef M3UROOT_H_
#define M3UROOT_H_

#include <QtCore/QFileInfo>
#include "m3uitem.h"


M3U_PLUGIN_NS_BEGIN

class M3uRoot : public M3uItem
{
public:
	M3uRoot(const QFileInfo &info) :
		m_info(info),
		m_label(QString::fromLatin1(".."))
	{}

	/* IFileType */
	virtual FileTypeId id() const { return FileTypeId(); }
	virtual QIcon icon() const { return QIcon(); }
	virtual QString name() const { return QString(); }
	virtual QString description() const { return QString(); }

	/* IFileInfo */
	virtual bool isDir() const { return m_info.isDir(); }
	virtual bool isFile() const { return m_info.isFile(); }
	virtual bool isLink() const { return false; }
	virtual IFileInfo::size_type fileSize() const { return 0; }
	virtual const Location &fileName() const { return m_fileName; }
	virtual const IFileType *fileType() const { return NULL; }
	virtual QDateTime lastModified() const { return m_info.lastModified(); }
	virtual int permissions() const { return 0; }
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
	Location m_fileName;
	QString m_label;
};

M3U_PLUGIN_NS_END

#endif /* M3UROOT_H_ */
