#include "idmqueryresultscopycontrol.h"
#include "../../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsCopyControl::IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const IdmEntityValue::Holder &value, const IdmEntity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index) :
	IdmCopyControlBase(dest, container),
	m_value(value),
	m_property(property),
	m_model(model),
	m_index(index)
{}

bool IdmQueryResultsCopyControl::start(const Snapshot::List &files, bool move)
{
	if (m_dest->start(files, move) && m_container.transaction())
	{
		IdmEntityValue::Holder localValue;
		IdmCompositeEntityValue::List list;
		list.reserve(files.size());

		for (Snapshot::List::size_type i = 0, size = files.size(); i < size; ++i)
			if (localValue = m_container.addValue(m_property.entity, QString(m_storage).append(files.at(i).second->info()->fileName())))
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
	m_model->refresh(m_index);
}

void IdmQueryResultsCopyControl::canceled()
{
	m_model->refresh(m_index);
}

IDM_PLUGIN_NS_END
