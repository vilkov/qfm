/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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
#ifndef DEFAULT_GLOBALACTIONS_H_
#define DEFAULT_GLOBALACTIONS_H_

#include <vfs/actions/vfs_action.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class CopyAction;
class CutAction;
class PasteAction;
class PasteIntoFolderAction;
class PropertiesAction;
class PasteClipboardAction;


class GlobalActions
{
public:
    GlobalActions();

    Action *copyAction() const;
    Action *cutAction() const;
    Action *pasteAction() const;
    Action *pasteIntoFolderAction() const;
    Action *propertiesAction() const;
    Action *pasteClipboardAction() const;

    static GlobalActions *instance();

private:
    friend class BaseNode;
    void addRef();
    void release();

private:
    CopyAction *m_copyAction;
    CutAction *m_cutAction;
    PasteAction *m_pasteAction;
    PasteIntoFolderAction *m_pasteIntoFolderAction;
    PropertiesAction *m_propertiesAction;
    PasteClipboardAction *m_pasteClipboardAction;
    int m_ref;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_GLOBALACTIONS_H_ */
