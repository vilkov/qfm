#include "idmcopycontrol.h"
#include "../gui/value/new/file/newfilevaluedialog.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmCopyControl::IdmCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const IFileContainer *folder, IdmEntity *entity) :
	m_dest(dest.take()),
	m_container(container),
	m_entity(entity),
	m_storage(difference(folder->location(), m_container.container()->location()).append(QChar('/')))
{}

QString IdmCopyControl::location() const
{
	return m_dest->location();
}

QString IdmCopyControl::location(const QString &fileName) const
{
	return m_dest->location(fileName);
}

bool IdmCopyControl::isPhysical() const
{
	return m_dest->isPhysical();
}

IFileInfo::size_type IdmCopyControl::freeSpace() const
{
	return m_dest->freeSpace();
}

ICopyControl *IdmCopyControl::createControl(INodeView *view) const
{
	return NULL;
}

bool IdmCopyControl::contains(const QString &fileName) const
{
	return m_dest->contains(fileName);
}

bool IdmCopyControl::remove(const QString &fileName, QString &error) const
{
	return m_dest->remove(fileName, error);
}

bool IdmCopyControl::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return m_dest->rename(oldName, newName, error);
}

bool IdmCopyControl::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	return m_dest->move(source, fileName, error);
}

IFileContainer *IdmCopyControl::open() const
{
	return m_dest->open();
}

IFileAccessor *IdmCopyControl::open(const QString &fileName, int mode, QString &error) const
{
	return m_dest->open(fileName, mode, error);
}

IFileContainer *IdmCopyControl::open(const QString &fileName, bool create, QString &error) const
{
	return m_dest->open(fileName, create, error);
}

const IFileContainerScanner *IdmCopyControl::scanner() const
{
	return m_dest->scanner();
}

INode *IdmCopyControl::node() const
{
	return m_dest->node();
}

bool IdmCopyControl::start(const Snapshot::List &files, bool move)
{
	if (m_dest->start(files, move) && m_container.transaction())
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

					for (Snapshot::List::size_type i = 0, size = files.size(); i < size; ++i)
						if (localValue = m_container.addValue(path, QString(m_storage).append(files.at(i).second->info()->fileName())))
						{
							list.push_back(localValue);
							possibleFiles[localValue->id()] = files.at(i).second;
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
	m_dest->done(error);
}

void IdmCopyControl::canceled()
{
	m_dest->canceled();
}

QString IdmCopyControl::difference(const QString &path1, const QString &path2)
{
	Path current(path1);
	Path other(path2);

	for (Path::Iterator otherIt = other.begin(), currentIt = current.begin();
		!otherIt.atEnd() && !currentIt.atEnd();
		++otherIt)
	{
		if (*currentIt == *otherIt)
			currentIt = current.erase(currentIt);
		else
			break;
	}

	return current.toString();
}

IDM_PLUGIN_NS_END
