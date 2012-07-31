#ifndef VFS_IPLUGIN_H_
#define VFS_IPLUGIN_H_

#include <QtCore/QModelIndex>
#include "vfs_ifilecontainer.h"
#include "../tools/vfs_uri.h"
#include "../../tools/settings/options/gui/page/settingspage.h"


VFS_NS_BEGIN
class Node;


class IPlugin
{
public:
	virtual ~IPlugin();

	virtual void registered() = 0;
	virtual ::Tools::Settings::Page *settings() = 0;
};


class IContainerPlugin : public virtual IPlugin
{
public:
	virtual QString shema() const = 0;
	virtual Node *open(const Uri::Iterator &path, QModelIndex &selected) const = 0;
};


class IFilePlugin : public virtual IPlugin
{
public:
	typedef QList<FileTypeId> FileTypeIdList;

public:
	virtual FileTypeIdList fileTypes() const = 0;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const = 0;
};

VFS_NS_END

#endif /* VFS_IPLUGIN_H_ */
