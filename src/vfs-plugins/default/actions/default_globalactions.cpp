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
#include "default_globalactions.h"
#include "default_copyaction.h"
#include "default_cutaction.h"
#include "default_pasteaction.h"
#include "default_pasteintofolderaction.h"
#include "default_propertiesaction.h"
#include "default_pasteclipboardaction.h"


DEFAULT_PLUGIN_NS_BEGIN

static GlobalActions s_instance;


GlobalActions::GlobalActions() :
    m_copyAction(NULL),
    m_cutAction(NULL),
    m_pasteAction(NULL),
    m_pasteIntoFolderAction(NULL),
    m_propertiesAction(NULL),
    m_pasteClipboardAction(NULL),
    m_ref(0)
{}

Action *GlobalActions::copyAction() const
{
    return m_copyAction;
}

Action *GlobalActions::cutAction() const
{
    return m_cutAction;
}

Action *GlobalActions::pasteAction() const
{
    return m_pasteAction;
}

Action *GlobalActions::pasteIntoFolderAction() const
{
    return m_pasteIntoFolderAction;
}

Action *GlobalActions::propertiesAction() const
{
    return m_propertiesAction;
}

Action *GlobalActions::pasteClipboardAction() const
{
    return m_pasteClipboardAction;
}

GlobalActions *GlobalActions::instance()
{
    return &s_instance;
}

void GlobalActions::addRef()
{
    if (m_ref++ == 0)
    {
        m_copyAction = new CopyAction();
        m_cutAction = new CutAction();
        m_pasteAction = new PasteAction();
        m_pasteIntoFolderAction = new PasteIntoFolderAction();
        m_propertiesAction = new PropertiesAction();
        m_pasteClipboardAction = new PasteClipboardAction();
    }
}

void GlobalActions::release()
{
    if (--m_ref == 0)
    {
        delete m_copyAction;
        delete m_cutAction;
        delete m_pasteAction;
        delete m_pasteIntoFolderAction;
        delete m_propertiesAction;
        delete m_pasteClipboardAction;
    }
}

DEFAULT_PLUGIN_NS_END
