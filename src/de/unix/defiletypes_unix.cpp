#include "../defiletypes.h"


DE_NS_BEGIN

::FileSystem::FileTypeId fileTypeId(FileTypes::Audio::Type id)
{
	::FileSystem::FileTypeId typeId;

	switch (id)
	{
		case FileTypes::Audio::MpegUrl:
			typeId.mime = QString::fromLatin1("audio/x-mpegurl");
			break;
	}

	return typeId;
}

::FileSystem::FileTypeId fileTypeId(FileTypes::Application::Type id)
{
	::FileSystem::FileTypeId typeId;

	switch (id)
	{
		case FileTypes::Application::GZipFile:
			typeId.mime = QString::fromLatin1("application/x-gzip");
			break;

		case FileTypes::Application::TarFile:
			typeId.mime = QString::fromLatin1("application/x-tar");
			break;

		case FileTypes::Application::CompressedTarFile:
			typeId.mime = QString::fromLatin1("application/x-compressed-tar");
			break;

		case FileTypes::Application::BZipCompressedTarFile:
			typeId.mime = QString::fromLatin1("application/x-bzip-compressed-tar");
			break;

		case FileTypes::Application::ZipFile:
			typeId.mime = QString::fromLatin1("application/zip");
			break;

		case FileTypes::Application::BZipFile:
			typeId.mime = QString::fromLatin1("application/x-bzip");
			break;

		case FileTypes::Application::RarFile:
			typeId.mime = QString::fromLatin1("application/x-rar");
			break;

		case FileTypes::Application::TarzFile:
			typeId.mime = QString::fromLatin1("application/x-tarz");
			break;

		case FileTypes::Application::BZip2File:
			typeId.mime = QString::fromLatin1("application/x-bzip2");
			break;

		case FileTypes::Application::JavaArchiveFile:
			typeId.mime = QString::fromLatin1("application/x-java-archive");
			break;

		case FileTypes::Application::DebFile:
			typeId.mime = QString::fromLatin1("application/x-deb");
			break;

		case FileTypes::Application::Arch7zCompressedFile:
			typeId.mime = QString::fromLatin1("application/x-7z-compressed");
			break;

		case FileTypes::Application::CompressFile:
			typeId.mime = QString::fromLatin1("application/x-compress");
			break;

		case FileTypes::Application::ZipCompressedFile:
			typeId.mime = QString::fromLatin1("application/x-zip-compressed");
			break;

		case FileTypes::Application::LzmaFile:
			typeId.mime = QString::fromLatin1("application/x-lzma");
			break;

		case FileTypes::Application::ServicepackFile:
			typeId.mime = QString::fromLatin1("application/x-servicepack");
			break;

		case FileTypes::Application::XzCompressedTarFile:
			typeId.mime = QString::fromLatin1("application/x-xz-compressed-tar");
			break;

		case FileTypes::Application::LzmaCompressedTarFile:
			typeId.mime = QString::fromLatin1("application/x-lzma-compressed-tar");
			break;

		case FileTypes::Application::CdImageFile:
			typeId.mime = QString::fromLatin1("application/x-cd-image");
			break;
	}

	return typeId;
}

DE_NS_END
