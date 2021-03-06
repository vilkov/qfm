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
#ifndef ARC_PLUGIN_H_
#define ARC_PLUGIN_H_

#include <vfs/interfaces/vfs_iplugin.h>
#include <tools/settings/options/settingsoption.h>
#include "libarchive/actions/arc_libarchive_unpackintosubdiraction.h"
#include "libunrar/actions/arc_libunrar_unpackintosubdiraction.h"


ARC_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual ::Tools::Settings::Page *settings();
};

ARC_PLUGIN_NS_END


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class Plugin : public Arc::Plugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};

ARC_PLUGIN_LIBARCHIVE_NS_END


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class Plugin : public Arc::Plugin
{
public:
	Plugin(::Tools::Settings::Option *parentOption);

	/* IPlugin */
	virtual void registered();

	/* IFilePlugin */
	virtual FileTypeIdList fileTypes() const;
	virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	UnPackIntoSubdirAction m_unPackIntoSubdirAction;
};

ARC_PLUGIN_LIBUNRAR_NS_END


#endif /* ARC_PLUGIN_H_ */
