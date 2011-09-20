#include "newcompositevaluedialog.h"
#include "../../list/valuelistdialog.h"
#include <QtGui/QMessageBox>


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(entity, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("New value for \"%1\"").arg(entity->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &NewCompositeValueDialog::addValue);
	m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &NewCompositeValueDialog::removeValue);

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void NewCompositeValueDialog::accept()
{
	QDialog::accept();
}

QModelIndex NewCompositeValueDialog::currentIndex() const
{
	return m_view.selectionModel()->currentIndex();
}

void NewCompositeValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		doAddValue(index);
}

void NewCompositeValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid())
		doRemoveValue(index);
}

void NewCompositeValueDialog::doAddValue(const QModelIndex &index)
{
	IdmEntity *entity = static_cast<IdmEntityItem*>(index.internalPointer())->entity();
	QByteArray name("NewCompositeValueDialog::doAddValue");

	if (m_container.savepoint(name))
	{
		ValueListDialog dialog(m_container, Select(entity), this);

		if (dialog.exec() == ValueListDialog::Accepted)
			if (m_container.release(name))
			{

			}
			else
			{
				m_container.rollback(name);
				QMessageBox::critical(this, windowTitle(), m_container.lastError());
			}
		else
			m_container.rollback(name);
	}
	else
		QMessageBox::critical(this, windowTitle(), m_container.lastError());
}

void NewCompositeValueDialog::doRemoveValue(const QModelIndex &index)
{

}
