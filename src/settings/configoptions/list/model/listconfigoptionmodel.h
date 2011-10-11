#ifndef LISTCONFIGOPTIONMODEL_H_
#define LISTCONFIGOPTIONMODEL_H_

#include <QtCore/QStringList>
#include <QtCore/QAbstractItemModel>


class ListConfigOptionModel : public QAbstractItemModel
{
public:
	ListConfigOptionModel(const QVariant &defaultValue, QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	QVariant value() const;
	void setValue(const QVariant &value);

	void add(const QString &value);
	void remove(const QModelIndex &index);

protected:
	QStringList m_items;
};

#endif /* LISTCONFIGOPTIONMODEL_H_ */
