#include "arcdelegate.h"


ARC_PLUGIN_NS_BEGIN

ArcDelegate::ArcDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

void ArcDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize ArcDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

ARC_PLUGIN_NS_END
