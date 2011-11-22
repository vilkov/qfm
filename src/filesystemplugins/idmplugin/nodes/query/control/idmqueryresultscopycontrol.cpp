#include "idmqueryresultscopycontrol.h"
#include "../../../gui/value/new/file/newfilevaluedialog.h"
#include "../../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsCopyControl::IdmQueryResultsCopyControl(const IdmContainer &container, IdmEntity *entity, const IdmEntity::Property &property, const Info &info) :
	CopyInfo(info),
	m_container(container),
	m_entity(entity),
	m_property(property)
{}

bool IdmQueryResultsCopyControl::start(const InfoListItem *files, bool move)
{
	if (m_container.transaction())
	{
		PScopedPointer<IdmCompositeEntityValue> value(m_container.addValue(m_entity));

		if (value)
		{
			NewFileValueDialog dialog(m_container, value.data(), toStringList(files), &Application::instance()->mainWindow());

			if (dialog.exec() != NewFileValueDialog::Accepted)
				m_container.rollback();
			else
				if (!m_container.commit())
				{
					m_container.rollback();
					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
				}
		}
		else
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
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
