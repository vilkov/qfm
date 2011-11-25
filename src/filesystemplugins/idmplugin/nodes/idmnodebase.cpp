#include "idmnodebase.h"
#include "idmnode.h"
#include "query/idmnodequeryresults.h"
#include "control/idmcopycontrol.h"
#include "../items/idmroot.h"
#include "../items/idmmessage.h"
#include "../items/idmseparator.h"
#include "../items/idmitemslist.h"
#include "../gui/list/listentitydialog.h"
#include "../gui/create/createentitydialog.h"
#include "../gui/choose/choosefileentitydialog.h"
#include "../gui/query/create/createquerydialog.h"
#include "../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmNodeBase::IdmNodeBase(const IdmContainer &container, const Info &info, Node *parent) :
	FolderNodeBase(info, m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_proxy(this),
	m_delegate(&m_proxy),
	m_container(container)
{
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

IdmNodeBase::~IdmNodeBase()
{
	m_items.removeAt(0);
	qDeleteAll(m_items);
}

int IdmNodeBase::columnCount(const QModelIndex &parent) const
{
	return 2;
}

IFileInfo *IdmNodeBase::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *IdmNodeBase::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, &Application::instance()->mainWindow()))
		return new IdmCopyControl(m_container, entity, absoluteFilePath());
	else
		return 0;
}

void IdmNodeBase::menuAction(QAction *action, INodeView *view)
{
	switch (static_cast<IdmContainer::MenuId>(action->data().toInt()))
	{
		case IdmContainer::Create:
		{
			CreateEntityDialog dialog(m_container, QString(), &Application::instance()->mainWindow());

			if (dialog.exec() == CreateEntityDialog::Accepted)
				if (m_container.transaction())
					if (IdmEntity *entity = m_container.createEntity(dialog.name(), dialog.type(), dialog.shortFormat()))
						if (entity->type() == Database::Composite)
						{
							bool ok = true;

							for (CreateEntityDialog::size_type i = 0, size = dialog.size(); i < size; ++i)
								if (!m_container.addProperty(entity, dialog.property(i), dialog.propertyName(i)))
								{
									ok = false;
									m_container.rollback();
									QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
									break;
								}

							if (ok && !m_container.commit())
							{
								m_container.rollback();
								QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
							}
						}
						else
						{
							if (!m_container.commit())
							{
								m_container.rollback();
								QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
							}
						}
					else
					{
						m_container.rollback();
						QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
					}
				else
					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());

			break;
		}
		case IdmContainer::Find:
		{
			if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, &Application::instance()->mainWindow()))
			{
				CreateQueryDialog dialog(m_container, entity, &Application::instance()->mainWindow());

				if (dialog.exec() == CreateQueryDialog::Accepted)
					switchTo(new IdmNodeQueryResults(m_container, dialog.query(), absoluteFilePath(), this), view);
			}

			break;
		}
		case IdmContainer::List:
		{
			if (m_container.transaction())
			{
				ListEntityDialog dialog(m_container, &Application::instance()->mainWindow());

				if (dialog.exec() == ListEntityDialog::Accepted)
				{
					if (!m_container.commit())
					{
						m_container.rollback();
						QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
					}
				}
				else
					m_container.rollback();
			}
			else
				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());

			break;
		}
	}
}

void IdmNodeBase::createFile(const QModelIndex &index, INodeView *view)
{

}

void IdmNodeBase::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void IdmNodeBase::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeBase::remove(const QModelIndexList &list, INodeView *view)
{
	if (m_container.transaction())
		if (!processIndexList(list, IdmFunctors::callTo(this, &IdmNodeBase::processRemoveItem)))
			m_container.rollback();
		else
			if (!m_container.commit())
			{
				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
}

void IdmNodeBase::cancel(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeBase::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeBase::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeBase::copy(const INodeView *source, INodeView *destination)
{

}

void IdmNodeBase::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex IdmNodeBase::rootIndex() const
{
	return QModelIndex();
}

QAbstractItemModel *IdmNodeBase::proxyModel() const
{
	return &((IdmNodeBase *)this)->m_proxy;
}

QAbstractItemDelegate *IdmNodeBase::itemDelegate() const
{
	return &((IdmNodeBase *)this)->m_delegate;
}

const INodeView::MenuActionList &IdmNodeBase::menuActions() const
{
	return m_container.menuActions();
}

Node *IdmNodeBase::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<IBaseItem*>(index.internalPointer())->isRoot())
		return Node::parent();
	else
		return 0;
}

Node *IdmNodeBase::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

UpdatesList::Map IdmNodeBase::updateFilesMap() const
{
	return UpdatesList::Map();
}

void IdmNodeBase::updateFilesEvent(const UpdatesList &updates)
{

}

void IdmNodeBase::scanForSizeEvent(bool canceled, const InfoListItem *entries)
{

}

bool IdmNodeBase::scanForCopyEvent(bool canceled, const InfoListItem *entries, ICopyControl *control, bool move)
{
	return false;
}

bool IdmNodeBase::scanForRemoveEvent(bool canceled, const InfoListItem *entries)
{
	return false;
}

bool IdmNodeBase::performCopyEvent(bool canceled, const InfoListItem *entries, bool move)
{
	return false;
}

void IdmNodeBase::performRemoveEvent(const InfoListItem *entries)
{

}

void IdmNodeBase::updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed)
{

}

void IdmNodeBase::completedProgressEvent(const QString &fileName, quint64 timeElapsed)
{

}

QModelIndex IdmNodeBase::index(IdmItem *item) const
{
	if (item->parent())
		return createIndex(static_cast<IdmItemsList*>(item->parent())->indexOf(item), 0, item);
	else
		return createIndex(m_items.indexOf(item), 0, item);
}

bool IdmNodeBase::processIndexList(const QModelIndexList &list, const IdmFunctors::Functor &functor) const
{
	QModelIndex index;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (list.at(i).column() == 0 && (index = m_proxy.mapToSource(list.at(i))).isValid())
			if (!functor(index, static_cast<IdmItem*>(index.internalPointer())))
				return false;

	return true;
}

bool IdmNodeBase::processRemoveItem(const QModelIndex &idx, IdmItem *item)
{

	return true;
}

IDM_PLUGIN_NS_END
