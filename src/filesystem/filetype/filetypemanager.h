#ifndef FILETYPEMANAGER_H_
#define FILETYPEMANAGER_H_

#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class FileTypeManager
{
public:
	FileTypeManager();

	void info(const QString &absoluteFilePath);
};

FILE_SYSTEM_NS_END

#endif /* FILETYPEMANAGER_H_ */
