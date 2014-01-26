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
#ifndef IDM_COMPOSITEVALUEDELEGATE_H_
#define IDM_COMPOSITEVALUEDELEGATE_H_

#include "../../../model/idm_delegate.h"
#include "../../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueDelegate : public Delegate
{
    Q_DISABLE_COPY(CompositeValueDelegate)

public:
    CompositeValueDelegate(const EntityValue &value, const IdmContainer &container, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    const Entity &m_entity;
    mutable IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEDELEGATE_H_ */
