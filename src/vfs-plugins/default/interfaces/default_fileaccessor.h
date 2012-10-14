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
#ifndef DEFAULT_FILEACCESSOR_H_
#define DEFAULT_FILEACCESSOR_H_

#include <vfs/interfaces/vfs_ifileaccessor.h>
#include "../default_ns.h"

#ifdef Q_OS_UNIX
	typedef int FileDescriptor;
#else
	#error Unknown OS!
#endif


DEFAULT_PLUGIN_NS_BEGIN

class FileAccesor : public IFileAccessor
{
public:
	FileAccesor(const QByteArray &absoluteFilePath, int mode);
	virtual ~FileAccesor();

	bool isValid() const;

	/* IFileAccessor */
	virtual QString lastError() const;
	virtual int permissions() const;

	virtual size_type size();
	virtual bool setPermissions(int mode);
	virtual size_type read(value_type *data, size_type size);
	virtual size_type write(const value_type *data, size_type size);
	virtual size_type seek(size_type offset, Seek from = FromCurrent);

private:
	FileDescriptor m_file;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILEACCESSOR_H_ */
