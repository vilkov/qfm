#include "idmqueryresultscopycontrol.h"
#include "../../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsCopyControl::IdmQueryResultsCopyControl(const IdmContainer &container, IdmCompositeEntityValue *value, const IdmEntity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index, const Info &info) :
	CopyInfo(info),
	m_container(container),
	m_value(value),
	m_property(property),
	m_model(model),
	m_index(index)
{}

bool IdmQueryResultsCopyControl::start(const InfoListItem *files, bool move)
{
	if (m_container.transaction())
	{
		IdmEntityValue *localValue;
		IdmCompositeEntityValue::List list;
		list.reserve(files->size());

		for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
			if (localValue = m_container.addValue(m_property.entity, absoluteFilePath(files->at(i)->fileName())))
				list.push_back(localValue);
			else
			{
				m_container.rollback();
				qDeleteAll(list);
				return false;
			}

		if (m_container.addValue(m_value, list))
			if (m_container.commit())
				m_model->add(m_index, list);
			else
			{
				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
		else
		{
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
			m_container.rollback();
			qDeleteAll(list);
		}
	}
	else
		QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());

	return false;
}

void IdmQueryResultsCopyControl::done(bool error)
{

}

void IdmQueryResultsCopyControl::canceled()
{

}

QStringList IdmQueryResultsCopyControl::toStringList(const InfoListItem *files) const
{
	QStringList res;
	res.reserve(files->size());

	for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
		res.push_back(absoluteFilePath(files->at(i)->fileName()));

	return res;
}

IDM_PLUGIN_NS_END
