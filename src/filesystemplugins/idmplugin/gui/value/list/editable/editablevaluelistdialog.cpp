#include "editablevaluelistdialog.h"
#include "../../../tools/idmentityvaluecreationtools.h"
#include <QtGui/QMessageBox>


EditableValueListDialog::EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(query.entity()),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_container, query, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(m_entity->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &EditableValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &EditableValueListDialog::removeValue);

    m_view.setHeaderHidden(true);

	if (m_model.isValid())
		m_view.setModel(&m_model);
	else
		QMessageBox::critical(this, windowTitle(), m_model.lastError());
}

IdmEntityValue *EditableValueListDialog::takeValue()
{
	return m_model.take(currentIndex());
}

QModelIndex EditableValueListDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void EditableValueListDialog::addValue()
{
	bool declined = false;

	if (IdmEntityValue *value = CreationTools::createValue(
			tr("New value for \"%1\"").arg(m_entity->name()),
			tr("Value"),
			this,
			m_container,
			m_entity,
			declined))
		m_model.add(value);
	else
		if (!declined)
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
}

void EditableValueListDialog::removeValue()
{
	QModelIndex index = m_view.selectionModel()->currentIndex();

	if (index.isValid())
		if (m_container.removeValue(m_entity, IdmStorage::IdsList() << static_cast<IdmEntityValue*>(index.internalPointer())->id()))
			m_model.remove(index);
		else
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
}
