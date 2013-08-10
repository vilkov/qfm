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
#ifndef IDM_VALUELISTMODEL_H_
#define IDM_VALUELISTMODEL_H_

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include "../../../storage/values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

class ValueListModel : public QAbstractItemModel
{
public:
	typedef QList<EntityValue::Holder> List;
	typedef List::size_type               size_type;

public:
	ValueListModel(const IdmContainer &container, const Select &query, QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual void fetchMore(const QModelIndex &parent);
    virtual bool canFetchMore(const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	bool isValid() const { return m_reader.isValid(); }
	const QString &lastError() const { return m_reader.lastError(); }

	size_type size() const { return m_items.size(); }
	const EntityValue::Holder &at(size_type index) const { return m_items.at(index); }
	EntityValue::Holder take(const QModelIndex &index);

	void close() { m_reader.close(); }

protected:
    enum { PrefetchLimit = 64 };

protected:
    void add(const List &list);
    QModelIndex add(const EntityValue::Holder &value);
	void remove(const QModelIndex &index);

protected:
	ValueReader m_reader;
	List m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUELISTMODEL_H_ */
