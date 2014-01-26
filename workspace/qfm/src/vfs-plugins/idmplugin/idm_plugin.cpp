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
#include "idm_plugin.h"
#include "nodes/root/idm_rootnode.h"
#include "../../application.h"


IDM_PLUGIN_NS_BEGIN

Plugin::Plugin(::Tools::Settings::Option *parentOption) :
    m_settings(parentOption)
{}

void Plugin::registered()
{
    FileTypeId type;
    type.mime = QByteArray("inode/directory");

    Application::globalActions()->registerAction(&m_createDbAction, type, Actions::ForSingleFile);
}

::Tools::Settings::Page *Plugin::settings()
{
    return &m_settings;
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
    FileTypeId type;
    type.mime = QByteArray("inode/directory");

    return FileTypeIdList() << type;
}

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
    IFileContainer::Holder folder(container->open(file, m_error));

    if (folder && folder->contains(fileName()))
        return new RootNode(folder, parent);
    else
        return NULL;
}

const QString &Plugin::fileName()
{
    static const QString res = QString::fromLatin1(".storage.idm");
    return res;
}

IDM_PLUGIN_NS_END
