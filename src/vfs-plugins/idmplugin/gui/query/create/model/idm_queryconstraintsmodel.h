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
#ifndef IDM_QUERYCONSTRAINTSMODEL_H_
#define IDM_QUERYCONSTRAINTSMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/idm_queryconstraintsrootitem.h"
#include "../../../../storage/constraints/value/idm_constraint.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsModel : public QAbstractItemModel
{
public:
	QueryConstraintsModel(QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	void add(const QModelIndex &index);
	void add(Constraint *constraint, const QModelIndex &index);
	void remove(const QModelIndex &index);

	GroupConstraint *take();

private:
	QVariant data(BaseConstraint *item, int column, int role) const;

private:
	QueryConstraintsRootItem m_root;
	QVariant m_andTypeLabel;
	QVariant m_orTypeLabel;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYCONSTRAINTSMODEL_H_ */
