#ifndef FILESYSTEMCOMMONTOOLS_H_
#define FILESYSTEMCOMMONTOOLS_H_

#include <QtGui/QWidget>
#include <QtCore/QString>
#include "../filesystem_ns.h"
#include "../../tools/strings/hierarchy/stringshierarchytree.h"


FILE_SYSTEM_NS_BEGIN

struct Tools
{
	static QString humanReadableTime(quint64 msecs);
	static QString humanReadableSize(quint64 size);
	static QString humanReadableShortSize(quint64 size);
	static quint64 freeSpace(const QByteArray &utf8AbsoluteFolderPath);

	class DestinationFromPathList : public ::Tools::Strings::Hierarchy::Tree
	{
	public:
		DestinationFromPathList();

		QString choose(const QString &title, QWidget *parent) const;
		void add(const QString &file, qint32 excludeLastEntries);
		void add(const QString &file);
	};
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCOMMONTOOLS_H_ */
