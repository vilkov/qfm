#include "editablevaluelistdialog.h"


EditableValueListDialog::EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	NestedPlainDialog(parent),
	m_handler(this),
	m_widget(&m_handler, container, query, this)
{
	const QRect &geometry = query.entity()->listGeometry();

	if (geometry.isValid())
		setGeometry(geometry);

	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListDialog::removeValue);

    setCentralWidget(&m_widget);
}

IdmEntityValue::Holder EditableValueListDialog::takeValue()
{
	return m_widget.takeValue();
}

QModelIndex EditableValueListDialog::currentIndex() const
{
	return m_widget.currentIndex();
}

void EditableValueListDialog::addValue()
{
	m_widget.addValue();
}

void EditableValueListDialog::removeValue()
{
	m_widget.removeValue();
}
