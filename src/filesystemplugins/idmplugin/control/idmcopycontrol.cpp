#include "idmcopycontrol.h"
#include "../gui/value/newvaluedialog.h"
#include "../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmCopyControl::IdmCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info) :
	CopyInfo(info),
	m_container(container),
	m_entity(entity)
{}

bool IdmCopyControl::start(const FileSystemList *files, bool move)
{
	if (m_container.transaction())
	{
		NewValueDialog dialog(m_container, m_entity, &Application::instance()->mainWindow());

		if (dialog.exec() == NewValueDialog::Accepted)
		{
//					IdmFileControl control(*entities.begin());
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

IDM_PLUGIN_NS_END
