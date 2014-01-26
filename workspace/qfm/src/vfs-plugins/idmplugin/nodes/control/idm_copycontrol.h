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
#ifndef IDM_COPYCONTROL_H_
#define IDM_COPYCONTROL_H_

#include <liquiddb/Entity>
#include <QtCore/QCoreApplication>
#include "../../control/idm_copycontrolbase.h"


IDM_PLUGIN_NS_BEGIN
using namespace LiquidDb;


class CopyControl : public CopyControlBase
{
    Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
    CopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const Entity &entity);

    /* ICopyControl */
    virtual bool start(const Snapshot &files, bool move);

private:
    const Entity &m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COPYCONTROL_H_ */
