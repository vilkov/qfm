#include "idmcopycontrol.h"
#include "../gui/value/new/file/newfilevaluedialog.h"
#include "../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmCopyControl::IdmCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info, const Info &storage) :
	CopyControl(info),
	m_container(container),
	m_entity(entity),
	m_storage(QString(info - storage).append(QChar('/')))
{}

bool IdmCopyControl::start(const Snapshot::Files &files, bool move)
{
	if (m_container.transaction())
	{
		IdmEntityValue::Holder value(m_container.addValue(m_entity));

		if (value)
		{
			IdmEntity *path;
    		CompositeValueModel::Files possibleFiles;
    		CompositeValueModel::ValueList list;
    		list.reserve(files.size());

			for (IdmEntity::size_type i = 0, size = m_entity->size(); i < size; ++i)
		    	if ((path = m_entity->at(i).entity)->type() == Database::Path)
		    	{
		    		IdmEntityValue::Holder localValue;

					for (Snapshot::Files::size_type i = 0, size = files.size(); i < size; ++i)
						if (localValue = m_container.addValue(path, QString(m_storage).append(files.at(i)->fileName())))
						{
							list.push_back(localValue);
							possibleFiles[localValue->id()] = files.at(i);
						}
						else
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
							return false;
						}

					break;
		    	}

			if (m_container.addValue(value, list))
			{
				NewFileValueDialog dialog(m_container, value, possibleFiles, Application::mainWindow());

				if (dialog.exec() != NewFileValueDialog::Accepted)
					m_container.rollback();
				else
					if (m_container.commit())
						return true;
					else
					{
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}

				m_container.updateEditorGeometry(m_entity, dialog.geometry());
			}
			else
			{
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
	}
	else
		QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());

	return false;
}

void IdmCopyControl::done(bool error)
{

}

void IdmCopyControl::canceled()
{

}

IDM_PLUGIN_NS_END
