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
#ifndef IDM_PLUGIN_H_
#define IDM_PLUGIN_H_

#include <QtCore/QString>
#include <vfs/interfaces/vfs_iplugin.h>
#include "actions/idm_createdbaction.h"
#include "settings/idm_pluginsettings.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IFilePlugin
{
public:
    Plugin(::Tools::Settings::Option *parentOption);

    /* IPlugin */
    virtual void registered();
    virtual ::Tools::Settings::Page *settings();

    /* IFilePlugin */
    virtual FileTypeIdList fileTypes() const;
    virtual Node *open(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

    static const QString &fileName();

private:
    Settings m_settings;
    mutable QString m_error;
    CreateDbAction m_createDbAction;
};

IDM_PLUGIN_NS_END

#endif /* IDM_PLUGIN_H_ */
