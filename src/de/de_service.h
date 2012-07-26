#ifndef DE_SERVICE_H_
#define DE_SERVICE_H_

#include <QtCore/QLocale>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "de_ns.h"


DE_NS_BEGIN

/**
 * Service - singleton, must be initialized
 * before any calls to it's methods!
 *
 **/
class Service
{
public:
	enum Type
	{
		DE_Cde,
		DE_Kde,
		DE_4Dwm,
		DE_Gnome,
		DE_Unknown
	};

	enum IconSize
	{
		Small = 16
	};

public:
	Service();
	~Service();

	Type type() const { return m_type; }
	const QLocale &locale() const { return m_locale; }

	QIcon processingIcon(int iconSize = Small) const;
	QIcon cancelingIcon(int iconSize = Small) const;

	QIcon copyActionIcon(int iconSize = Small) const;
	QIcon cutActionIcon(int iconSize = Small) const;
	QIcon pasteActionIcon(int iconSize = Small) const;
	QIcon propertiesActionIcon(int iconSize = Small) const;

	QIcon packActionIcon(int iconSize = Small) const;
	QIcon unpackActionIcon(int iconSize = Small) const;

	QIcon searchIcon(int iconSize = Small) const;
	QIcon openDataIcon(int iconSize = Small) const;

	QIcon missingIcon(int iconSize = Small) const;

	::VFS::IApplications::LinkedList userApplications(const ::VFS::FileTypeId &id) const;

	::VFS::FileTypeInfo fileTypeInfo(const QByteArray &absoluteFilePath, bool isDir, int iconSize = Small) const;
	::VFS::FileTypeInfo fileTypeInfoFromFileName(const QString &fileName, bool isDir, int iconSize = Small) const;

	void open(const ::VFS::IFileContainer *container, const ::VFS::IFileInfo *file) const;

private:
	QByteArray themeName() const;
	::VFS::FileTypeInfo fileTypeInfo(int iconSize) const;
	::VFS::FileTypeInfo fileTypeInfo(const char *mimeType, int iconSize) const;

private:
	Type m_type;
	QLocale m_locale;
};

DE_NS_END

#endif /* DE_SERVICE_H_ */
