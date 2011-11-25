#include "compositevaluedialog.h"
#include "../model/items/compositevaluevalueitem.h"
#include "../model/items/compositevaluepropertyitem.h"
#include "../list/editable/editablevaluelistdialog.h"
#include "../../../storage/values/idmvaluereader.h"
#include "../../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


CompositeValueDialog::CompositeValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_value(value),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_value, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CompositeValueDialog::removeValue);

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void CompositeValueDialog::accept()
{
	QDialog::accept();
}

QModelIndex CompositeValueDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void CompositeValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<IdmItem*>(index.internalPointer())->isList())
		doAddValue(index);
}

void CompositeValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && !static_cast<IdmItem*>(index.internalPointer())->isList())
		doRemoveValue(index);
}

void CompositeValueDialog::doAddValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity();
	QByteArray name = Database::savepoint("CompositeValueDialog::doAddValue::");

	if (m_container.savepoint(name))
	{
		EditableValueListDialog dialog(m_container, Select(entity), this);

		if (dialog.exec() == EditableValueListDialog::Accepted)
		{
			IdmEntityValue *value;

			if (m_container.addValue(m_value, value = dialog.takeValue()))
				if (m_container.release(name))
					m_model.add(index, value);
				else
				{
					m_container.rollback(name);
					QMessageBox::critical(this, windowTitle(), m_container.lastError());
				}
			else
			{
				delete value;
				m_container.rollback(name);
				QMessageBox::critical(this, windowTitle(), m_container.lastError());
			}
		}
		else
			m_container.rollback(name);
	}
	else
		QMessageBox::critical(this, windowTitle(), m_container.lastError());
}

void CompositeValueDialog::doRemoveValue(const QModelIndex &index)
{
	IdmEntityValue *value = static_cast<CompositeValueValueItem*>(index.internalPointer())->value();
	QByteArray name = Database::savepoint("CompositeValueDialog::doRemoveValue::");

	if (m_container.savepoint(name))
		if (m_container.removeValue(m_value, value))
			if (m_container.release(name))
				m_model.remove(index);
			else
			{
				m_container.rollback(name);
				QMessageBox::critical(this, windowTitle(), m_container.lastError());
			}
		else
		{
			m_container.rollback(name);
			QMessageBox::critical(this, windowTitle(), m_container.lastError());
		}
	else
		QMessageBox::critical(this, windowTitle(), m_container.lastError());
}
