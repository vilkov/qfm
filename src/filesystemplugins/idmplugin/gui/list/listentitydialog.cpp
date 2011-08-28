#include "listentitydialog.h"


ListEntityDialog::ListEntityDialog(QWidget *parent) :
	QDialog(parent),
	m_verticatLayout(this),
	m_addEntity(tr("Add entity"), this),
	m_removeEntity(tr("Remove entity"), this),
	m_addProperty(tr("Add property"), this),
	m_removeProperty(tr("Remove property"), this),
	m_view(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(tr("List of all entities"));

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_addEntity);
	m_horizontalLayout.addWidget(&m_removeEntity);
	m_horizontalLayout.addWidget(&m_addProperty);
	m_horizontalLayout.addWidget(&m_removeProperty);

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ListEntityDialog::accept()
{
	QDialog::accept();
}
