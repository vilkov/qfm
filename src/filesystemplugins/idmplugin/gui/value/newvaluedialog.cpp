#include "newvaluedialog.h"


NewValueDialog::NewValueDialog(IdmContainer *container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_entity(entity),
	m_view(this),
	m_buttonBox(this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("New value for \"%1\"").arg(entity->name()));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_view);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	m_view.setHeaderHidden(true);
}

void NewValueDialog::accept()
{
	QDialog::accept();
}
