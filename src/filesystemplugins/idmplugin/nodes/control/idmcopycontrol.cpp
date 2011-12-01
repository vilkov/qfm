#include "idmcopycontrol.h"
#include "../../gui/value/new/file/newfilevaluedialog.h"
#include "../../../../application.h"
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
		PScopedPointer<IdmCompositeEntityValue> value(m_container.addValue(m_entity));

		if (value)
		{
			IdmEntity *path;
    		CompositeValueModel::ValueList list;
    		list.reserve(files->size());

			for (IdmEntity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		    	if ((path = m_entity->at(i).entity)->type() == Database::Path)
		    	{
		    		IdmEntityValue *localValue;

					for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
						if (localValue = m_container.addValue(path, absoluteFilePath(files->at(i)->fileName())))
							list.push_back(localValue);
						else
						{
							m_container.rollback();
							qDeleteAll(list);
							return false;
						}

					break;
		    	}

			if (m_container.addValue(value.data(), list))
			{
				NewFileValueDialog dialog(m_container, value.data(), &Application::instance()->mainWindow());

				if (dialog.exec() != NewFileValueDialog::Accepted)
					m_container.rollback();
				else
					if (m_container.commit())
						return true;
					else
					{
						QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}
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
