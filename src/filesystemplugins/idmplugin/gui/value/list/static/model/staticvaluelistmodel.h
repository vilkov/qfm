#ifndef STATICVALUELISTMODEL_H_
#define STATICVALUELISTMODEL_H_

#include <QtCore/QList>
#include <QtCore/QAbstractItemModel>
#include "../../../../../storage/values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

class StaticValueListModel : public QAbstractItemModel
{
public:
	typedef QList<IdmEntityValue*> List;
	typedef List::size_type        size_type;

public:
	StaticValueListModel(const IdmContainer &container, const Select &query, QObject *parent = 0);
	virtual ~StaticValueListModel();

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

	void add(const List &list);
	void add(IdmEntityValue *value);
	void add(Database::id_type id, const QVariant &value);
	void remove(const QModelIndex &index);
	IdmEntityValue *take(const QModelIndex &index);

private:
    enum { PrefetchLimit = 256 };

private:
	IdmValueReader m_reader;
	List m_items;
};

IDM_PLUGIN_NS_END

#endif /* STATICVALUELISTMODEL_H_ */
