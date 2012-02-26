#ifndef FILESYSTEMIPLUGIN_H_
#define FILESYSTEMIPLUGIN_H_

#include "filesystemifileinfo.h"
#include "../filesystemnode.h"
#include "../filetypeinfo/filetypeinfo.h"


FILE_SYSTEM_NS_BEGIN

class IPlugin
{
public:
	virtual ~IPlugin();

	virtual void registered() = 0;
	virtual Node *node(const IFileInfo *info, Node *parent) const = 0;
};


class IFileReaderPlugin : public IPlugin
{
public:
	typedef QList<FileTypeId> FileTypeIdList;

public:
	virtual FileTypeIdList fileTypes() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIPLUGIN_H_ */
