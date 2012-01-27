#ifndef FILESYSTEMIFILETYPE_H_
#define FILESYSTEMIFILETYPE_H_

#include <QtCore/QString>
#include <QtGui/QIcon>
#include "../filesystem_ns.h"
#include "../filetypeinfo/filetypeinfo.h"


FILE_SYSTEM_NS_BEGIN

class IFileType
{
public:
	virtual ~IFileType() {}

	virtual FileTypeId id() const = 0;
	virtual QIcon icon() const = 0;
	virtual QString name() const = 0;
	virtual QString description() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILETYPE_H_ */
