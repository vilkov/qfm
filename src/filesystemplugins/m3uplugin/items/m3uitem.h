#ifndef M3UITEM_H_
#define M3UITEM_H_

#include "../../../filesystem/interfaces/filesystemifileinfo.h"


class M3uItem : public FileSystem::IFileInfo
{
public:
	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;
};

#endif /* M3UITEM_H_ */
