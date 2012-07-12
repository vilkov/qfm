#ifndef VFS_COMMONTOOLS_H_
#define VFS_COMMONTOOLS_H_

#include <QtGui/QWidget>
#include <QtCore/QString>
#include "../vfs_ns.h"
#include "../../tools/strings/hierarchy/stringshierarchytree.h"


VFS_NS_BEGIN

struct Tools
{
	static QString humanReadableTime(quint64 msecs);
	static QString humanReadableSize(quint64 size);
	static QString humanReadableShortSize(quint64 size);

	class DestinationFromPathList : public ::Tools::Strings::Hierarchy::Tree
	{
	public:
		DestinationFromPathList();

		QString choose(const QString &title, QWidget *parent) const;
		void add(const QString &file, qint32 excludeLastEntries);
		void add(const QString &file);
	};
};

VFS_NS_END

#endif /* VFS_COMMONTOOLS_H_ */
