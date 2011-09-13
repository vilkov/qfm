#include "newfilevaluedialog.h"


NewFileValueDialog::NewFileValueDialog(const IdmContainer &container, IdmEntity *entity, const QStringList &files, QWidget *parent) :
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

    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &NewFileValueDialog::addValue);

    for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
    	if (entity->at(i).entity->type() == Database::Path)
    	{
    		QModelIndex index = m_model.index(i, 0);

			for (QStringList::size_type i = 0, size = files.size(); i < size; ++i)
				m_model.add(index, files.at(i));

			break;
    	}

	m_view.setHeaderHidden(true);
	m_view.setModel(&m_model);
}

void NewFileValueDialog::accept()
{
	QDialog::accept();
}

void NewFileValueDialog::addValue()
{

}
