#ifndef DEFILETYPES_H_
#define DEFILETYPES_H_

#include "de_ns.h"
#include "filetypeinfo.h"


DE_NS_BEGIN

struct FileTypes
{
	enum Type
	{
		Unknown = 0
	};

	struct Audio
	{
		enum { _from = 1, _count = 1 };
		enum Type
		{
			MpegUrl = _from
		};
	};

	struct Application
	{
		enum { _from = Audio::_count, _count = 19 };
		enum Type
		{
			GZipFile              = _from,
			TarFile               = _from + 1,
			CompressedTarFile     = _from + 2,
			BZipCompressedTarFile = _from + 3,
			ZipFile               = _from + 4,
			BZipFile              = _from + 5,
			RarFile               = _from + 6,
			TarzFile              = _from + 7,
			BZip2File             = _from + 8,
			JavaArchiveFile       = _from + 9,
			DebFile               = _from + 10,
			Arch7zCompressedFile  = _from + 11,
			CompressFile          = _from + 12,
			ZipCompressedFile     = _from + 13,
			LzmaFile              = _from + 14,
			ServicepackFile       = _from + 15,
			XzCompressedTarFile   = _from + 16,
			LzmaCompressedTarFile = _from + 17,
			CdImageFile           = _from + 18
		};
	};
};

FileTypeId fileTypeId(FileTypes::Audio::Type id);
FileTypeId fileTypeId(FileTypes::Application::Type id);

DE_NS_END

#endif /* DEFILETYPES_H_ */
