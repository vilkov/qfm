#include "compositevaluedialog.h"
#include "../model/items/compositevaluevalueitem.h"
#include "../model/items/compositevaluepropertyitem.h"


CompositeValueDialog::CompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent) :
	NestedPlainDialog(parent),
	m_handler(this),
	m_mainWidget(&m_handler, container, value, this)
{
	const QRect &geometry = value->entity()->editorGeometry();

	if (geometry.isValid())
		setGeometry(geometry);

	m_handler.registerMouseDoubleClickEventHandler(&CompositeValueDialog::openFile);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueDialog::removeValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &CompositeValueDialog::openFile);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Enter,  &CompositeValueDialog::openFile);

	setCentralWidget(&m_mainWidget);
}

CompositeValueDialog::CompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent) :
	NestedPlainDialog(parent),
	m_handler(this),
	m_mainWidget(&m_handler, container, value, files, this)
{
	const QRect &geometry = value->entity()->editorGeometry();

	if (geometry.isValid())
		setGeometry(geometry);

	m_handler.registerMouseDoubleClickEventHandler(&CompositeValueDialog::openFile);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueDialog::removeValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Return, &CompositeValueDialog::openFile);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Enter,  &CompositeValueDialog::openFile);

	setCentralWidget(&m_mainWidget);
}

void CompositeValueDialog::openFile()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<CompositeValueItem *>(index.internalPointer())->isPath())
		doOpenFile(index);
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
