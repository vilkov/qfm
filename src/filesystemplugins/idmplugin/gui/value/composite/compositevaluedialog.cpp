#include "compositevaluedialog.h"


CompositeValueDialog::CompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent) :
	NestedPlainDialog(parent),
	m_handler(this),
	m_mainWidget(&m_handler, container, value)
{
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueDialog::removeValue);

	setCentralWidget(&m_mainWidget);
}

void CompositeValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
		doAddValue(index);
}

void CompositeValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && !static_cast<CompositeValueItem *>(index.internalPointer())->isProperty())
		doRemoveValue(index);
}
