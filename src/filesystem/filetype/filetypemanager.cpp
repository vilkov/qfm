#include "filetypemanager.h"
#include "xdgmime/src/xdgmime.h"
#include <kmimetype.h>
#include <QtCore/QDebug>
#include <QtCore/QFile>

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
{
	xdg_mime_shutdown();
}

FileTypeManager::MimeType FileTypeManager::type(const QString &absoluteFilePath) const
{
	return MimeType();
}

Info FileTypeManager::info(const QString &absoluteFilePath) const
{
	QFile file(absoluteFilePath);

	if (file.open(QFile::ReadOnly))
	{
		KMimeType::Ptr type = KMimeType::findByNameAndContent(file.fileName(), &file);
		qDebug() << type->iconName();
		qDebug() << type->comment();
		qDebug() << type->userSpecifiedIconName();
	}

	struct stat st;
	stat(absoluteFilePath.toUtf8().data(), &st);

	qDebug() << xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st);
	qDebug() << xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data());

	qDebug() << xdg_mime_get_icon(xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st));
	qDebug() << xdg_mime_get_icon(xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data()));

	qDebug() << xdg_mime_get_generic_icon(xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st));
	qDebug() << xdg_mime_get_generic_icon(xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data()));

	return Info();
}

FILE_SYSTEM_NS_END
