#include "createquerydialog.h"
#include "model/items/queryentitiesmodelitem.h"
#include "../constraint/constraintquerydialog.h"
#include <QtGui/QMessageBox>


CreateQueryDialog::CreateQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	QDialog(parent),
	m_verticatLayout(this),
	m_splitter(this),
	m_container(container),
	m_entity(entity),
	m_handler(this),
	m_toolBar1(this),
	m_view(&m_handler, this),
	m_view2(this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setWindowTitle(tr("Find \"%1\"").arg(m_entity->name()));

    m_toolBar1.addAction(tr("Add constraint"))->setData(AddConstraint);
    m_toolBar2.addAction(tr("Add group"))->setData(AddGroup);

	connect(&m_toolBar1, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
	connect(&m_toolBar2, SIGNAL(actionTriggered(QAction*)), this, SLOT(actionTriggered(QAction*)));

	m_splitter.addWidget(&m_view);
	m_splitter.addWidget(&m_view2);

	m_horizontalLayout.setMargin(3);
	m_horizontalLayout.setSpacing(1);
	m_horizontalLayout.addWidget(&m_toolBar1);
	m_horizontalLayout.addWidget(&m_toolBar2);

	m_verticatLayout.setMargin(3);
	m_verticatLayout.setSpacing(1);
	m_verticatLayout.addLayout(&m_horizontalLayout);
	m_verticatLayout.addWidget(&m_splitter);
	m_verticatLayout.addWidget(&m_buttonBox);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    for (IdmEntity::size_type i = 0, size = m_entity->size(); i < size; ++i)
    	m_model.add(m_entity->at(i));

    m_view.setHeaderHidden(true);
    m_view.setModel(&m_model);

    m_view2.setItemDelegate(&m_delegate2);
    m_view2.setModel(&m_model2);

//    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Delete, &CreateQueryDialog::removeEntity);
//    m_handler.registerShortcut(Qt::NoModifier, Qt::Key_Insert, &CreateQueryDialog::insertProperty);
}

Select CreateQueryDialog::query()
{
	return Select(m_entity, m_model2.take());
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
		case AddConstraint:
		{
			QModelIndex index1 = currentIndex1();

			if (index1.isValid())
				if (static_cast<QueryEntitiesModelItem*>(index1.internalPointer())->entity()->type() != Database::Composite)
				{
					QModelIndex index2 = currentIndex2();

					if (index2.isValid() && static_cast<BaseConstraint*>(index2.internalPointer())->isGroup())
					{
						ConstraintQueryDialog dialog(m_container, static_cast<QueryEntitiesModelItem*>(index1.internalPointer())->property(), this);

						if (dialog.exec() == ConstraintQueryDialog::Accepted)
							m_model2.add(dialog.takeConstraint(static_cast<BaseConstraint*>(index2.internalPointer())), index2);
					}
					else
						QMessageBox::warning(this, windowTitle(), tr("You must select a destination group!"));
				}
				else
					QMessageBox::warning(this, windowTitle(), tr("You must select a not composite property!"));
			else
				QMessageBox::warning(this, windowTitle(), tr("You must select a property!"));

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
