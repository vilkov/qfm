#include "queryconstraintsdelegate.h"
#include "../../../storage/constraints/group/idmgroupconstraint.h"
#include "../../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QComboBox>


IDM_PLUGIN_NS_BEGIN

QueryConstraintsDelegate::QueryConstraintsDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

QWidget *QueryConstraintsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			PScopedPointer<QComboBox> editor(new QComboBox(parent));

			editor->addItem(GroupConstraint::typeToString(GroupConstraint::And), GroupConstraint::And);
	    	editor->addItem(GroupConstraint::typeToString(GroupConstraint::Or), GroupConstraint::Or);
			editor->setCurrentIndex(0);

			return editor.take();
		}
	}

	return 0;
}

void QueryConstraintsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			static_cast<QComboBox*>(editor)->setCurrentIndex(static_cast<GroupConstraint*>(index.internalPointer())->type());
			break;
		}
	}
}

void QueryConstraintsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	switch (index.column())
	{
		case 0:
			break;

		case 1:
		{
			static_cast<GroupConstraint*>(index.internalPointer())->setType(static_cast<GroupConstraint::Type>(static_cast<QComboBox*>(editor)->itemData(static_cast<QComboBox*>(editor)->currentIndex(), Qt::UserRole).toInt()));
			break;
		}
	}
}

void QueryConstraintsDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize QueryConstraintsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

IDM_PLUGIN_NS_END
