#include "arhdelegate.h"


ARH_PLUGIN_NS_BEGIN

ArhDelegate::ArhDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

void ArhDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize ArhDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

ARH_PLUGIN_NS_END
