#ifndef IDM_VALUELISTMODEL_H_
#define IDM_VALUELISTMODEL_H_

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include "../../../storage/values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

class ValueListModel : public QAbstractItemModel
{
public:
	typedef QList<IdmEntityValue::Holder> List;
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
	const IdmEntityValue::Holder &at(size_type index) const { return m_items.at(index); }
	IdmEntityValue::Holder take(const QModelIndex &index);

	void close() { m_reader.close(); }

protected:
    enum { PrefetchLimit = 64 };

protected:
    void add(const List &list);
    QModelIndex add(const IdmEntityValue::Holder &value);
	void remove(const QModelIndex &index);

protected:
	IdmValueReader m_reader;
	List m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUELISTMODEL_H_ */
