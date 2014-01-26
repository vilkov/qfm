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
#include "idm_queryresultscopycontrol.h"
#include "../../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsCopyControl::IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const EntityValue &value, const Entity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index) :
    CopyControlBase(dest, container),
    m_value(value),
    m_property(property),
    m_model(model),
    m_index(index)
{}

bool IdmQueryResultsCopyControl::start(const Snapshot &files, bool move)
{
    if (m_dest->start(files, move) && m_container.transaction())
    {
        EntityValue localValue;
        EntityValue::List list;

        for (auto i : files)
        {
            localValue = m_container.addValue(m_property.entity, fromUnicode(QString(m_storage).append(i.second->info()->fileName())).data());

            if (localValue.isValid())
                list.push_back(localValue);
            else
            {
                m_container.rollback();
                return false;
            }
        }

        if (m_container.addValue(m_value, list))
            if (m_container.commit())
            {
                m_model->add(m_index, list);
                return true;
            }
            else
            {
                QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
                m_container.rollback();
            }
        else
        {
            QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));
            m_container.rollback();
        }
    }
    else
        QMessageBox::critical(Application::mainWindow(), tr("Error"), toUnicode(m_container.lastError()));

    return false;
}

void IdmQueryResultsCopyControl::done(bool error)
{
    m_model->refresh(m_index);
}

void IdmQueryResultsCopyControl::canceled()
{
    m_model->refresh(m_index);
}

IDM_PLUGIN_NS_END
