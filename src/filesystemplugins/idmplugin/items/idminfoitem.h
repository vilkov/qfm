#ifndef IDMINFOITEM_H_
#define IDMINFOITEM_H_

#include <QtCore/QFileInfo>
#include "idmitem.h"
#include "../../../filesystem/tools/filesystemcommontools.h"
#include "../../../filesystem/interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class IdmInfoItem : public IdmItem, public IFileInfo
{
public:
	IdmInfoItem(const QFileInfo &info, IdmItem *parent = 0) :
		IdmItem(parent),
		m_info(info)
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
	virtual void refresh() { m_info.refresh(); }

	virtual bool isRoot() const { return false; }
	virtual bool isList() const { return false; }
	virtual bool isMenuItem() const { return false; }

private:
	QFileInfo m_info;
};

FILE_SYSTEM_NS_END

#endif /* IDMINFOITEM_H_ */
