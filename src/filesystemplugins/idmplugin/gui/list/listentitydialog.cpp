#include "listentitydialog.h"


ListEntityDialog::ListEntityDialog(const IdmContainer *container, QWidget *parent) :
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
    m_view.setModel(&m_model);

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

    for (IdmContainer::size_type i = 0, size = container->size(); i < size; ++i)
    	m_model.add(container->at(i));
}

void ListEntityDialog::accept()
{
	QDialog::accept();
}

void ListEntityDialog::removeEntity()
{
	//	if (item->isEntityItem())
	//		if (item->parent()->isEntityItem())
	//		{
	//			if (QMessageBox::question(&Application::instance()->mainWindow(),
	//								  tr("Remove property"),
	//								  tr("Do you really want to remove property \"%1\" of entity \"%2\"?").
	//								  arg(static_cast<IdmEntityItem*>(item)->entity()->name()).
	//								  arg(static_cast<IdmEntityItem*>(item->parent())->entity()->name()),
	//								  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	//				if (m_container->removeProperty(static_cast<IdmEntityItem*>(item->parent())->entity(),
	//												static_cast<IdmEntityItem*>(item)->entity()))
	//				{
	//					IdmMenuEntities *entities = static_cast<IdmMenuEntities*>(m_container->menu()->at(IdmContainer::List));
	//
	//					beginRemoveRows(parent(idx), idx.row(), idx.row());
	//					entities->remove(static_cast<IdmEntityItem*>(item), idx.row());
	//					delete item;
	//					endRemoveRows();
	//				}
	//				else
	//				{
	//					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
	//					return false;
	//				}
	//		}
	//		else
	//		{
	//			if (QMessageBox::question(&Application::instance()->mainWindow(),
	//								  tr("Remove entity"),
	//								  tr("Do you really want to remove entity \"%1\"?").
	//								  arg(static_cast<IdmEntityItem*>(item)->entity()->name()),
	//								  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	//				if (m_container->removeEntity(static_cast<IdmEntityItem*>(item)->entity()))
	//				{
	//					IdmEntity *entity = static_cast<IdmEntityItem*>(item)->entity();
	//					IdmEntityItem *parent;
	//					IdmMenuEntities *entities = static_cast<IdmMenuEntities*>(m_container->menu()->at(IdmContainer::List));
	//					IdmMenuEntities::List items = entities->items(entity);
	//					IdmItemsList::size_type index;
	//
	//					for (IdmMenuEntities::List::size_type i = 0, size = items.size(); i < size; ++i)
	//					{
	//						parent = static_cast<IdmEntityItem*>(items.at(i)->parent());
	//						index = parent->indexOf(items.at(i));
	//
	//						beginRemoveRows(IdmNodeBase::index(parent), index, index);
	//						entities->remove(items.at(i), index);
	//						delete items.at(i);
	//						endRemoveRows();
	//					}
	//
	//					delete entity;
	//				}
	//				else
	//				{
	//					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
	//					return false;
	//				}
	//		}
}
