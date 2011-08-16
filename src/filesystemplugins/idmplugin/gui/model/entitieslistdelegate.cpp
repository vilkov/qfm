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

	for (IdmEntityTypes::const_iterator it = m_container->entityTypes().constBegin(), end = m_container->entityTypes().constEnd(); it != end; ++it)
    	editor->addItem(it->label, it.key());

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

FILE_SYSTEM_NS_END
