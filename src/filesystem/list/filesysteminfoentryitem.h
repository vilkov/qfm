#ifndef FILESYSTEMINFOENTRYITEM_H_
#define FILESYSTEMINFOENTRYITEM_H_

#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

class InfoEntryItem : public InfoItem
{
public:
	explicit InfoEntryItem(const Info &info);

	virtual bool isList() const;
	virtual qint64 totalSize() const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFOENTRYITEM_H_ */
