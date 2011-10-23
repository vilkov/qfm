#include "idmcopycontrol.h"
#include "../gui/value/new/file/newfilevaluedialog.h"
#include "../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmCopyControl::IdmCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info) :
	CopyInfo(info),
	m_container(container),
	m_entity(entity)
{}

bool IdmCopyControl::start(const InfoListItem *files, bool move)
{
	if (m_container.transaction())
	{
		NewFileValueDialog dialog(m_container, m_entity, toStringList(files), &Application::instance()->mainWindow());

		if (dialog.exec() == NewFileValueDialog::Accepted)
		{
			PScopedPointer<IdmEntityValue> value(dialog.value());

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

	return false;
}

void IdmCopyControl::done(bool error)
{

}

void IdmCopyControl::canceled()
{

}

QStringList IdmCopyControl::toStringList(const InfoListItem *files) const
{
	QStringList res;
	res.reserve(files->size());

	for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
		res.push_back(absoluteFilePath(files->at(i)->fileName()));

	return res;
}

IDM_PLUGIN_NS_END
