#include "listconfigoptiondelegate.h"
#include <QtGui/QLineEdit>


ListConfigOptionDelegate::ListConfigOptionDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

QWidget *ListConfigOptionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return new QLineEdit(parent);
}

void ListConfigOptionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	static_cast<QLineEdit*>(editor)->setText(index.data().toString());
}

void ListConfigOptionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QString text = static_cast<QLineEdit*>(editor)->text().simplified();

	if (!text.isEmpty())
		model->setData(index, text);
}

void ListConfigOptionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize ListConfigOptionDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}
