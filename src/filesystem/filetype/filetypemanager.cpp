#include "filetypemanager.h"
#include "../../de/desktopenvironment.h"
#ifndef Q_OS_WIN
#	include <unistd.h>
#endif


FILE_SYSTEM_NS_BEGIN

FileTypeManager::MimeType::MimeType()
{}

FileTypeManager::MimeType::MimeType(const QString &type, const QString &subtype) :
	m_type(type),
	m_subtype(subtype)
{}

FileTypeManager::FileTypeManager() :
#ifndef Q_OS_WIN
	m_userId(getuid()),
	m_groupId(getgid())
#endif
{}

FileTypeManager::~FileTypeManager()
{}

FileTypeManager::MimeType FileTypeManager::type(const QString &absoluteFilePath) const
{
	return MimeType();
}

Info FileTypeManager::info(const QString &absoluteFilePath) const
{
	DesktopEnvironment().info(absoluteFilePath);
	return Info();
}

FILE_SYSTEM_NS_END
