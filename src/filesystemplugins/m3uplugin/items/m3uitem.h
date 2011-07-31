#ifndef M3UITEM_H_
#define M3UITEM_H_

#include "../../../filesystem/interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class M3uItem : public IFileInfo
{
public:
	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* M3UITEM_H_ */
