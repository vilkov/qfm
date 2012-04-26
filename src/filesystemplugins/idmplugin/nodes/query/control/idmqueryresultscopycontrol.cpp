#include "idmqueryresultscopycontrol.h"
#include "../../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsCopyControl::IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const IdmEntityValue::Holder &value, const IdmEntity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index) :
	m_dest(dest.take()),
	m_container(container),
	m_value(value),
	m_property(property),
	m_model(model),
	m_index(index)
{}

QString IdmQueryResultsCopyControl::location() const
{
	return m_dest->location();
}

QString IdmQueryResultsCopyControl::location(const QString &fileName) const
{
	return m_dest->location(fileName);
}

bool IdmQueryResultsCopyControl::isPhysical() const
{
	return m_dest->isPhysical();
}

IFileInfo::size_type IdmQueryResultsCopyControl::freeSpace() const
{
	return m_dest->freeSpace();
}

ICopyControl *IdmQueryResultsCopyControl::createControl(INodeView *view) const
{
	return NULL;
}

bool IdmQueryResultsCopyControl::contains(const QString &fileName) const
{
	return m_dest->contains(fileName);
}

bool IdmQueryResultsCopyControl::remove(const QString &fileName, QString &error) const
{
	return m_dest->remove(fileName, error);
}

bool IdmQueryResultsCopyControl::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return m_dest->rename(oldName, newName, error);
}

bool IdmQueryResultsCopyControl::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	return m_dest->move(source, fileName, error);
}

IFileContainer *IdmQueryResultsCopyControl::open() const
{
	return m_dest->open();
}

IFileAccessor *IdmQueryResultsCopyControl::open(const QString &fileName, int mode, QString &error) const
{
	return m_dest->open(fileName, mode, error);
}

IFileContainer *IdmQueryResultsCopyControl::open(const QString &fileName, bool create, QString &error) const
{
	return m_dest->open(fileName, create, error);
}

const IFileContainerScanner *IdmQueryResultsCopyControl::scanner() const
{
	return m_dest->scanner();
}

INode *IdmQueryResultsCopyControl::node() const
{
	return m_dest->node();
}

bool IdmQueryResultsCopyControl::start(const Snapshot::List &files, bool move)
{
	if (m_dest->start(files, move) && m_container.transaction())
	{
		IdmEntityValue::Holder localValue;
		IdmCompositeEntityValue::List list;
		list.reserve(files.size());

		for (Snapshot::List::size_type i = 0, size = files.size(); i < size; ++i)
			if (localValue = m_container.addValue(m_property.entity, m_container.container()->location(files.at(i).second->info()->fileName())))
				list.push_back(localValue);
			else
			{
				m_container.rollback();
				return false;
			}

		if (m_container.addValue(m_value, list))
			if (m_container.commit())
			{
				m_model->add(m_index, list);
				return true;
			}
			else
			{
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
		else
		{
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
			m_container.rollback();
		}
	}
	else
		QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());

	return false;
}

void IdmQueryResultsCopyControl::done(bool error)
{
	m_dest->done(error);
}

void IdmQueryResultsCopyControl::canceled()
{
	m_dest->canceled();
}

IDM_PLUGIN_NS_END
