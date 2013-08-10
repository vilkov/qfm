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
#ifndef DEFAULT_DELEGATE_H_
#define DEFAULT_DELEGATE_H_

#include <QtGui/QAbstractProxyModel>
#include <vfs/tasks/model/vfs_tasksnodedelegate.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class Delegate : public TasksNodeDelegate
{
	Q_DISABLE_COPY(Delegate)

public:
	Delegate(QAbstractProxyModel *proxy, QObject *parent = 0);

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QAbstractProxyModel *m_proxy;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_DELEGATE_H_ */
