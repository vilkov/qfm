#ifndef VALUELISTMODEL_H_
#define VALUELISTMODEL_H_

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include "../../../../../storage/values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

class ValueListModel : public QAbstractItemModel
{
public:
	typedef QList<IdmEntityValue*> List;
	typedef List::size_type        size_type;

public:
	ValueListModel(const IdmContainer &container, const Select &query, QObject *parent = 0);
	virtual ~ValueListModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	bool isValid() const { return m_reader.isValid(); }
	const QString &lastError() const { return m_reader.lastError(); }

	void add(const List &list);
	void add(IdmEntityValue *value);
	void add(Database::id_type id, const QVariant &value);
	void remove(const QModelIndex &index);
	IdmEntityValue *take(const QModelIndex &index);

private:
	IdmValueReader m_reader;
	List m_items;
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTMODEL_H_ */
