#include "filetypemanager.h"
#include "xdgmime/src/xdgmime.h"
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
	QByteArray fileName = absoluteFilePath.toUtf8();
	const char *mimeType = xdg_mime_get_mime_type_from_file_name(fileName.data());

	if (mimeType == XDG_MIME_TYPE_UNKNOWN)
	{
		struct stat st;
		stat(fileName.data(), &st);

		if ((mimeType = xdg_mime_get_mime_type_for_file(fileName.data(), &st)) != XDG_MIME_TYPE_UNKNOWN)
		{
			if (const XdgAppArray *apps = xdg_mime_user_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}

			if (const XdgAppArray *apps = xdg_mime_default_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}

			if (const XdgAppArray *apps = xdg_mime_known_apps_lookup(mimeType))
			{
				const XdgAppGroup *group;
				const XdgEntryValueArray *values;

				for (int i = 0, size = xdg_mime_app_array_size(apps); i < size; ++i)
					if (group = xdg_mime_app_group_lookup(xdg_mime_app_array_item_at(apps, i), "Desktop Entry"))
						if (values = xdg_mime_app_entry_lookup(group, "Name"))
							for (int i = 0, size = xdg_mime_entry_value_array_size(values); i < size; ++i)
								qDebug() << xdg_mime_entry_value_array_item_at(values, i);
			}
		}
	}

	return Info();
}

FILE_SYSTEM_NS_END
