#include "entitieslistdelegate.h"
#include <QtGui/QComboBox>


FILE_SYSTEM_NS_BEGIN

EntitiesListDelegate::EntitiesListDelegate(const IdmContainer *container, QObject *parent) :
	QStyledItemDelegate(parent),
    m_container(container)
{
}

QWidget *EntitiesListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QComboBox *editor = new QComboBox(parent);

	for (IdmContainer::size_type i = 0, size = m_container->size(); i < size; ++i)
    	editor->addItem(m_container->at(i)->name(), m_container->at(i)->id());

	editor->setCurrentIndex(0);

	return editor;
}

void EntitiesListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void EntitiesListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

void EntitiesListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize EntitiesListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

FILE_SYSTEM_NS_END
