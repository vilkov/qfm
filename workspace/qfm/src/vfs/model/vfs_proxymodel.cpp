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
#include "vfs_proxymodel.h"


VFS_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ProxyModel::compareByFileNames(const IFileInfo *v1, const IFileInfo *v2)
{
	if (v1->isFile())
		if (v2->isFile())
			return compareFileNames(v1->fileName(), v2->fileName());
		else
			return false;
	else
		if (v2->isFile())
			return true;
		else
			return compareFileNames(v1->fileName(), v2->fileName());
}


#include <QtCore/QChar>
#define nat_char QChar
#define nat_isdigit(a) (a).isDigit()
#define nat_isspace(a) (a).isSpace()
#define nat_toupper(a) (a).toUpper()
#include <tools/strings/strnatcmp.h>

bool ProxyModel::compareFileNames(const QString &str1, const QString &str2)
{
	return strnatcasecmp(str1.data(), str2.data()) < 0;
}


#undef nat_char
#undef nat_isdigit
#undef nat_isspace
#undef nat_toupper
#include <tools/strings/strnatcmp.h>

bool ProxyModel::compareFileNames(const char *str1, const char *str2)
{
    return strnatcasecmp(str1, str2) < 0;
}

VFS_NS_END
