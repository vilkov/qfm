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
#ifndef VFS_PROXYMODEL_H_
#define VFS_PROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../interfaces/vfs_ifileinfo.h"


VFS_NS_BEGIN

class ProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

	static bool compareByFileNames(const IFileInfo *v1, const IFileInfo *v2);
	static bool compareFileNames(const QString &str1, const QString &str2);
    static bool compareFileNames(const char *str1, const char *str2);
};

VFS_NS_END

#endif /* VFS_PROXYMODEL_H_ */
