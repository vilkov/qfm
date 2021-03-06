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
#ifndef IDM_QUERYRESULTSCOPYCONTROL_H_
#define IDM_QUERYRESULTSCOPYCONTROL_H_

#include <liquiddb/EntityValue>
#include <QtCore/QCoreApplication>
#include "../interface/idm_inodequeryresults.h"
#include "../../../control/idm_copycontrolbase.h"
#include "../../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class IdmQueryResultsCopyControl : public CopyControlBase
{
    Q_DECLARE_TR_FUNCTIONS(IdmQueryResultsCopyControl)

public:
    IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const EntityValue &value, const Entity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index);

    /* ICopyControl */
    virtual bool start(const Snapshot &files, bool move);
    virtual void done(bool error);
    virtual void canceled();

private:
    EntityValue m_value;
    Entity::Property m_property;
    IQueryResultsUpdater *m_model;
    QModelIndex m_index;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTSCOPYCONTROL_H_ */
