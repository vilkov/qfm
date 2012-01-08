#ifndef DESKTOPENVIRONMENT_H_
#define DESKTOPENVIRONMENT_H_

#include "filesystemfiletypeinfo.h"
#include "../filesystem/tools/filesystemfileinfo.h"


/**
 * DesktopEnvironment - singleton, must be initialized
 * before any calls to it's methods!
 *
 **/
class DesktopEnvironment
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

	struct FileTypes
	{
		struct Audio
		{
			enum Type
			{
				M3uFile
			};
		};

		struct Application
		{
			enum Type
			{
				GZipFile,
				TarFile,
				CompressedTarFile,
				BZipCompressedTarFile,
				ZipFile,
				BZipFile,
				RarFile,
				TarzFile,
				BZip2File,
				JavaArchiveFile,
				DebFile,
				Arch7zCompressedFile,
				CompressFile,
				ZipCompressedFile,
				LzmaFile,
				ServicepackFile,
				XzCompressedTarFile,
				LzmaCompressedTarFile,
				CdImageFile
			};
		};
	};

public:
	DesktopEnvironment();
	~DesktopEnvironment();

	Type type() const { return m_type; }
	FileTypeId fileTypeId(FileTypes::Audio::Type id) const;
	FileTypeId fileTypeId(FileTypes::Application::Type id) const;
	FileTypeInfo fileTypeInfo(const FileSystem::FileInfo &fileInfo, const QString &absoluteFilePath, int iconSize) const;

private:
	Type m_type;
#if defined(DESKTOP_ENVIRONMENT_IS_KDE)
	int m_version;
#endif
};

#endif /* DESKTOPENVIRONMENT_H_ */
