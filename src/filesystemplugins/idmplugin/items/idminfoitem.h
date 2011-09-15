#ifndef IDMINFOITEM_H_
#define IDMINFOITEM_H_

#include "idmitem.h"
#include "../../../filesystem/info/filesysteminfo.h"


IDM_PLUGIN_NS_BEGIN

class IdmInfoItem : public IdmItem, public IFileInfo
{
public:
	IdmInfoItem(const Info &info, IdmItem *parent = 0);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	/* IdmItem */
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isValueItem() const;
	virtual bool isEntityItem() const;

private:
	Info m_info;
};

IDM_PLUGIN_NS_END

#endif /* IDMINFOITEM_H_ */
