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
#ifndef ARC_LIBUNRAR_SCANNER_H_
#define ARC_LIBUNRAR_SCANNER_H_

#include <QtCore/QMutex>
#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../arc_libunrar_ns.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class Scanner : public IFileContainerScanner
{
	Q_DECLARE_TR_FUNCTIONS(Scanner)

public:
	Scanner(const IFileContainer *container, const QByteArray &filePath);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

	static QString errorDescription(int code);

private:
	const IFileContainer *m_container;
	QByteArray m_filePath;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

ARC_PLUGIN_LIBUNRAR_NS_END

#endif /* ARC_LIBUNRAR_SCANNER_H_ */
