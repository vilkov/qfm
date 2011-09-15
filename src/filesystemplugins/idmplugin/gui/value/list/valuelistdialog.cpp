#include "valuelistdialog.h"
#include <QtGui/QMessageBox>


ValueListDialog::ValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_query(query),
	m_handler(this),
	m_view(&m_handler, this),
	m_model(m_container.prepare(query, m_lastError), this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Values of \"%1\"").arg(query.entity()->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &ValueListDialog::addValue);
    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &ValueListDialog::removeValue);

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);

	if (!m_model.isValid())
		QMessageBox::critical(this, windowTitle(), m_lastError);
}

void ValueListDialog::accept()
{
	QDialog::accept();
}

void ValueListDialog::addValue()
{

}

void ValueListDialog::removeValue()
{

}
