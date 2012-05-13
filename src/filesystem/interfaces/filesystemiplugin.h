#ifndef FILESYSTEMIPLUGIN_H_
#define FILESYSTEMIPLUGIN_H_

#include <QtCore/QModelIndex>
#include "filesystemifilecontainer.h"
#include "../tools/filesystempath.h"
#include "../../tools/settings/settingstab.h"


FILE_SYSTEM_NS_BEGIN
class Node;


class IPlugin
{
public:
	virtual ~IPlugin();

	virtual void registered() = 0;
	virtual const ::Tools::Settings::Tab *settings() const = 0;
};


class IContainerPlugin : public virtual IPlugin
{
public:
	virtual const QString &shema() const = 0;
	virtual Node *open(const Path::Iterator &path, QModelIndex &selected) const = 0;
};


class IFilePlugin : public virtual IPlugin
{
public:
	typedef QList<FileTypeId> FileTypeIdList;

public:
	virtual FileTypeIdList fileTypes() const = 0;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIPLUGIN_H_ */
