#ifndef DESERVICE_H_
#define DESERVICE_H_

#include <QtCore/QLocale>
#include "de_ns.h"
#include "../filesystem/interfaces/filesystemifilecontainer.h"


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
	const QLocale &locale() const { return m_locale; }

	QIcon processingIcon(int iconSize) const;
	QIcon cancelingIcon(int iconSize) const;

	QIcon copyActionIcon(int iconSize) const;
	QIcon cutActionIcon(int iconSize) const;
	QIcon pasteActionIcon(int iconSize) const;
	QIcon propertiesActionIcon(int iconSize) const;

	QIcon packActionIcon(int iconSize) const;
	QIcon unpackActionIcon(int iconSize) const;

	QIcon searchIcon(int iconSize) const;
	QIcon openDataIcon(int iconSize) const;

	QIcon missingIcon(int iconSize) const;

	::FileSystem::FileTypeInfo fileTypeInfo(const QString &absoluteFilePath, bool isDir, int iconSize) const;
	::FileSystem::FileTypeInfo fileTypeInfoFromFileName(const QString &fileName, bool isDir, int iconSize) const;

	void open(const ::FileSystem::IFileContainer *container, const ::FileSystem::IFileInfo *file) const;


	void test(const QString &absoluteFilePath) const;

private:
	QByteArray themeName() const;
	::FileSystem::FileTypeInfo fileTypeInfo(int iconSize) const;
	::FileSystem::FileTypeInfo fileTypeInfo(const char *mimeType, int iconSize) const;

private:
	Type m_type;
	QLocale m_locale;
};

DE_NS_END

#endif /* DESERVICE_H_ */
