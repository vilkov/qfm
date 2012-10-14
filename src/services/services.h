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
#ifndef SERVICES_H_
#define SERVICES_H_

#include <QtCore/QString>


class Services
{
public:
	class Service
	{
	public:
		typedef void *  File;
		typedef quint64 size_type;

	public:
		virtual ~Service() {}
	};

	class View : public Service
	{
	public:
		virtual void view(const File file, size_type size) = 0;
	};

	class Edit : public Service
	{
	public:
		virtual void edit(const QString &filePath) = 0;
	};

	class Open : public Service
	{
	public:
		virtual void open(const File file, size_type size) = 0;
	};


public:
	Services();

//	void view(const ::VFS::Info &file);
//	void edit(const ::VFS::Info &file);
//	void open(const ::VFS::Info &file);

public:
	static Services *instance();
	static void registerService(View *service);
	static void registerService(Edit *service);
	static void registerService(Open *service);
};

#endif /* SERVICES_H_ */
