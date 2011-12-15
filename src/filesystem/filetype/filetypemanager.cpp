#include "filetypemanager.h"
#include "xdgmime/src/xdgmime.h"
#include <kmimetype.h>
#include <QtCore/QDebug>


FILE_SYSTEM_NS_BEGIN

FileTypeManager::FileTypeManager()
{

}

FileTypeManager::~FileTypeManager()
{
	xdg_mime_shutdown();
}

void FileTypeManager::info(const QString &absoluteFilePath)
{
	struct stat st;
	stat(absoluteFilePath.toUtf8().data(), &st);

	KMimeType::findByNameAndContent();


	qDebug() << xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st);
	qDebug() << xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data());

	qDebug() << xdg_mime_get_icon(xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st));
	qDebug() << xdg_mime_get_icon(xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data()));

	qDebug() << xdg_mime_get_generic_icon(xdg_mime_get_mime_type_for_file(absoluteFilePath.toUtf8().data(), &st));
	qDebug() << xdg_mime_get_generic_icon(xdg_mime_get_mime_type_from_file_name(absoluteFilePath.toUtf8().data()));
}

FILE_SYSTEM_NS_END
