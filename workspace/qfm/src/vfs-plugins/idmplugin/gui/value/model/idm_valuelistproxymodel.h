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
#ifndef IDM_VALUELISTPROXYMODEL_H_
#define IDM_VALUELISTPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class ValueListProxyModel : public QSortFilterProxyModel
{
    Q_DISABLE_COPY(ValueListProxyModel)

public:
    ValueListProxyModel(QObject *parent = 0);

    void setFilter(const QString &filter);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString m_filter;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUELISTPROXYMODEL_H_ */
