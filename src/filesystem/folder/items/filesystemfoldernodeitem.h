#ifndef FILESYSTEMITEMFOLDERNODEITEM_H_
#define FILESYSTEMITEMFOLDERNODEITEM_H_

#include <QtCore/QVariant>
#include "../info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeItem : public Info
{
public:
	explicit FolderNodeItem(const Info &info) :
		Info(info)
	{}

	void operator=(const Info &info) { Info::operator=(info); }

	/* IFileInfo */
	virtual QString fileName() const { return isRoot() ? absoluteFilePath() : Info::fileName(); }

	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRootItem() const = 0;
	virtual bool isListItem() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMFOLDERNODEITEM_H_ */
