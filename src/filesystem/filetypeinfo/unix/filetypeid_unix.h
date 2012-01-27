#ifndef FILETYPEID_UNIX_H_
#define FILETYPEID_UNIX_H_

#include <QtCore/QString>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

struct FileTypeId_Unix
{
	FileTypeId_Unix()
	{}

	bool operator==(const FileTypeId_Unix &other) const { return mime == other.mime; }
	bool operator<(const FileTypeId_Unix &other) const { return mime < other.mime; }
	bool isValid() const { return !mime.isEmpty(); }

    QString mime;
};

FILE_SYSTEM_NS_END

#endif /* FILETYPEID_UNIX_H_ */
