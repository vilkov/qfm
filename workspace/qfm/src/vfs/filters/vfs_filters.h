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
#ifndef VFS_FILTERS_H_
#define VFS_FILTERS_H_

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "../interfaces/vfs_ifilecontainer.h"


VFS_NS_BEGIN

class Filters : public IFileContainerScanner::Filter
{
public:
	Filters(int size = 0)
	{
		m_filters.reserve(size);
	}

	virtual ~Filters();
	virtual bool match(const IFileInfo *info) const;

	void add(Filter *filter)
	{
		Holder holder(filter);
		m_filters.push_back(holder.data());
		holder.release();
	}

private:
	typedef QList<Filter *> Container;

private:
	Container m_filters;
};


class FileNameFilter : public IFileContainerScanner::Filter
{
public:
	FileNameFilter(const QString &pattern) :
		m_pattern(pattern)
	{
		m_pattern.setPatternSyntax(QRegExp::Wildcard);
	}

	virtual bool match(const IFileInfo *info) const;

private:
	QRegExp m_pattern;
};


class DateFilterRange : public IFileContainerScanner::Filter
{
public:
	DateFilterRange(const QDateTime &from, const QDateTime &to) :
		m_from(from),
		m_to(to)
	{}

protected:
	bool match(const QDateTime &dateTime) const
	{
		return dateTime >= m_from && dateTime <= m_to;
	}

private:
	QDateTime m_from;
	QDateTime m_to;
};


class ModificationTimeFilterRange : public DateFilterRange
{
public:
	ModificationTimeFilterRange(const QDateTime &from, const QDateTime &to) :
		DateFilterRange(from, to)
	{}

	virtual bool match(const IFileInfo *info) const;
};

VFS_NS_END

#endif /* VFS_FILTERS_H_ */
