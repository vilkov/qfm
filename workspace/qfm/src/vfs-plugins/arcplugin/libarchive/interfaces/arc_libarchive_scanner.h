/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef ARC_LIBARCHIVE_SCANNER_H_
#define ARC_LIBARCHIVE_SCANNER_H_

#include <QtCore/QMutex>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../arc_libarchive_ns.h"


struct archive;


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class Scanner : public IFileContainerScanner
{
public:
	class Enumerator;

public:
	Scanner(const IFileContainer *container, IFileAccessor::Holder &file);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

private:
	friend class Enumerator;
	class ReadArchive
	{
	public:
		ReadArchive(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive);
		~ReadArchive();

		/* Callbacks */
		static int open(struct archive *archive, void *_client_data);
		static ssize_t read(struct archive *archive, void *_client_data, const void **_buffer);
		static int64_t skip(struct archive *archive, void *_client_data, int64_t request);
		static int close(struct archive *archive, void *_client_data);

	private:
		ReadArchive(const ReadArchive &);
		ReadArchive &operator=(const ReadArchive &);

	private:
		IFileAccessor::value_type *m_buffer;
		const IFileAccessor::Holder &m_file;
		struct archive *m_archive;
		QMutex m_mutex;
	};

private:
	const IFileContainer *m_container;
	IFileAccessor::Holder m_file;
	struct archive *m_archive;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_SCANNER_H_ */
