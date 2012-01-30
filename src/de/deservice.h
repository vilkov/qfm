#ifndef DESERVICE_H_
#define DESERVICE_H_

#include "de_ns.h"
#include "defiletypes.h"
#include "../filesystem/tools/filesystemfileinfo.h"


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

public:
	Service();
	~Service();

	Type type() const { return m_type; }

	QIcon processingIcon(int iconSize) const;
	QIcon cancelingIcon(int iconSize) const;

	QIcon copyActionIcon(int iconSize) const;
	QIcon cutActionIcon(int iconSize) const;
	QIcon pasteActionIcon(int iconSize) const;
	QIcon propertiesActionIcon(int iconSize) const;

	::FileSystem::FileTypeInfo fileTypeInfo(const QString &absoluteFilePath, bool isDir, int iconSize) const;
	::FileSystem::FileTypeInfo fileTypeInfoFromFileName(const QString &fileName, bool isDir, int iconSize) const;

private:
	QByteArray themeName() const;
	::FileSystem::FileTypeInfo fileTypeInfo(int iconSize) const;
	::FileSystem::FileTypeInfo fileTypeInfo(const char *mimeType, int iconSize) const;
	QIcon findIcon(const char *name, int iconSize, int context) const;
	QIcon findMimeTypeIcon(const char *mimeType, int iconSize) const;

private:
	Type m_type;
};

DE_NS_END

#endif /* DESERVICE_H_ */
