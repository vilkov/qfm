#include "accountsdelegate.h"
#include <QLineEdit>


AccountsDelegate::AccountsDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

QSize AccountsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize sz = QStyledItemDelegate::sizeHint(option, index);
	sz.setHeight(sz.height() + 5);
	return sz;
}

QWidget *AccountsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return new QLineEdit(parent);
}

void AccountsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	static_cast<QLineEdit*>(editor)->setText(index.data(Qt::EditRole).toString());
}

void AccountsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	model->setData(index, static_cast<QLineEdit*>(editor)->text().simplified(), Qt::EditRole);
}

void AccountsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}
