#include "createquerydialog.h"
#include "../create/createentitydialog.h"
#include <QtGui/QMessageBox>


CreateQueryDialog::CreateQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_handler(this),
	m_toolBar(this),
	m_view(&m_handler, this),
	m_view2(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Find \"%1\"").arg(entity->name()));

    m_toolBar.addAction(tr("Create entity"))->setData(Create);
    m_toolBar.addAction(tr("Remove entity"))->setData(Remove);
    m_toolBar.addAction(tr("Add property"))->setData(AddProperty);
    m_toolBar.addAction(tr("Remove property"))->setData(RemoveProperty);

	connect(&m_toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addWidget(&m_toolBar);

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_view);
	m_horizontalLayout.addWidget(&m_view2);

	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
    	m_model.add(entity->at(i).name, entity->at(i).entity);

    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);

//    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CreateQueryDialog::removeEntity);
//    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CreateQueryDialog::insertProperty);
}

void CreateQueryDialog::accept()
{
	QDialog::accept();
}

void CreateQueryDialog::actionTriggered(QAction *action)
{
	switch (static_cast<ActionId>(action->data().toInt()))
	{
		case Create:
		{
			break;
		}
		case Remove:
		{
			break;
		}
		case AddProperty:
		{
			break;
		}
		case RemoveProperty:
		{
			break;
		}
	}
}

QModelIndex CreateQueryDialog::currentIndex()
{
	return m_view.selectionModel()->currentIndex();
}