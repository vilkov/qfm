#ifndef M3UROOT_H_
#define M3UROOT_H_

#include "m3uitem.h"
#include "../../../filesystem/info/filesysteminfo.h"


class M3uRoot : public M3uItem
{
public:
	M3uRoot(const FileSystem::Info &info) :
		m_info(info)
	{}

	/* IFileInfo */
	virtual bool exists() const { return m_info.exists(); }
	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QDateTime lastModified() const { m_info.lastModified(); }

	virtual void refresh() {}

	/* M3uItem */
	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }
	virtual bool isRoot() const { return true; }

private:
	FileSystem::Info m_info;
};

#endif /* M3UROOT_H_ */
