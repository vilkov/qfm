#include "createquerydialog.h"
#include "../create/createentitydialog.h"
#include <QtGui/QMessageBox>


CreateQueryDialog::CreateQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_container(container),
	m_handler(this),
	m_toolBar1(this),
	m_view(&m_handler, this),
	m_view2(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this),
	m_verticatLayout(this)
{
	setWindowTitle(tr("Find \"%1\"").arg(entity->name()));

    m_toolBar1.addAction(tr("Add condition"))->setData(AddCondition);
    m_toolBar2.addAction(tr("Add group"))->setData(AddGroup);

	connect(&m_toolBar1, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
	connect(&m_toolBar2, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_view);
	m_horizontalLayout.addWidget(&m_view2);

	m_horizontalLayout2.setMargin(3);
	m_horizontalLayout2.setSpacing(1);
	m_horizontalLayout2.addWidget(&m_toolBar1);
	m_horizontalLayout2.addWidget(&m_toolBar2);

	m_verticatLayout.addLayout(&m_horizontalLayout2);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
    	m_model.add(entity->at(i).name, entity->at(i).entity);

    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);

    m_view2.setItemDelegate(&m_delegate2);
    m_view2.setModel(&m_model2);

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
		case AddGroup:
		{
			m_model2.add(currentIndex2());
			break;
		}
		case AddCondition:
		{
			QModelIndex index = currentIndex1();

			if (index.isValid())
			{
//				static_cast<IdmEntityItem*>(index.internalPointer());
			}

			break;
		}
	}
}

QModelIndex CreateQueryDialog::currentIndex1()
{
	return m_view.selectionModel()->currentIndex();
}

QModelIndex CreateQueryDialog::currentIndex2()
{
	return m_view2.selectionModel()->currentIndex();
}
