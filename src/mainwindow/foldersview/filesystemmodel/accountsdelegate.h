#ifndef ACCOUNTSDELEGATE_H_
#define ACCOUNTSDELEGATE_H_

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>


class AccountsDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	AccountsDelegate(QObject *parent = 0);

    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
	virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif /* ACCOUNTSDELEGATE_H_ */
