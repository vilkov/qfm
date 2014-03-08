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
#include "idm_copycontrol.h"
#include "../../gui/value/new/file/idm_newfilevaluedialog.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

CopyControl::CopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const Entity &entity) :
    CopyControlBase(dest, container),
    m_entity(entity)
{}

bool CopyControl::start(const Snapshot &files, bool move)
{
    if (m_dest->start(files, move) && m_container.transaction())
    {
        EntityValue value(m_container.addValue(m_entity));

        if (value.isValid())
        {
            Entity path;
            CompositeValueModel::Files possibleFiles;
            CompositeValueModel::ValueList list;

            for (auto i : m_entity.properties())
                if (m_container.schema(path = i.second.entity) == IdmContainer::Path)
                {
                    EntityValue localValue;

                    for (Snapshot::const_iterator i = files.begin(), end = files.end(); i != end; ++i)
                    {
                        localValue = m_container.addValue(path, fromUnicode(QString(m_storage).append((*i).second->info()->fileName())).data());

                        if (localValue.isValid())
                        {
                            list.push_back(localValue);
                            possibleFiles[localValue.id()] = (*i).second;
                        }
                        else
                        {
                            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                            m_container.rollback();
                            return false;
                        }
                    }

                    break;
                }

            if (m_container.addValue(value, list))
            {
                NewFileValueDialog dialog(m_container, value, possibleFiles, Application::mainWindow());

                if (dialog.exec() != NewFileValueDialog::Accepted)
                    m_container.rollback();
                else
                    if (m_container.commit())
                        return true;
                    else
                    {
                        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                        m_container.rollback();
                    }

                m_container.setEditorGeometry(m_entity, dialog.geometry());
            }
            else
            {
                QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                m_container.rollback();
            }
        }
        else
            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
    }
    else
        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));

    return false;
}

IDM_PLUGIN_NS_END
