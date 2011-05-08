#ifndef M3UENTRY_H_
#define M3UENTRY_H_

#include "m3uitem.h"
#include "../../../filesystem/info/filesysteminfo.h"


class M3uEntry : public M3uItem
{
public:
	M3uEntry(const FileSystem::Info &info) :
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
	virtual bool isRoot() const { return false; }

private:
	FileSystem::Info m_info;
};

#endif /* M3UENTRY_H_ */
