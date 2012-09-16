#include "idm_delegate.h"


IDM_PLUGIN_NS_BEGIN

Delegate::Delegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

void Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize Delegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

IDM_PLUGIN_NS_END
