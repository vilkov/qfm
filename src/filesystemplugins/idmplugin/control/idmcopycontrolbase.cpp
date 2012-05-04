#include "idmcopycontrolbase.h"
#include "../../../filesystem/tools/filesystempath.h"


IDM_PLUGIN_NS_BEGIN

IdmCopyControlBase::IdmCopyControlBase(ICopyControl::Holder &dest, const IdmContainer &container) :
	m_dest(dest.take()),
	m_container(container),
	m_storage(difference(m_dest->location(), m_container.container()->location()).append(QChar('/')))
{}

QString IdmCopyControlBase::location() const
{
	return m_dest->location();
}

QString IdmCopyControlBase::location(const QString &fileName) const
{
	return m_dest->location(fileName);
}

bool IdmCopyControlBase::isPhysical() const
{
	return m_dest->isPhysical();
}

IFileInfo::size_type IdmCopyControlBase::freeSpace() const
{
	return m_dest->freeSpace();
}

ICopyControl *IdmCopyControlBase::createControl(INodeView *view) const
{
	return NULL;
}

bool IdmCopyControlBase::contains(const QString &fileName) const
{
	return m_dest->contains(fileName);
}

bool IdmCopyControlBase::remove(const QString &fileName, QString &error) const
{
	return m_dest->remove(fileName, error);
}

bool IdmCopyControlBase::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return m_dest->rename(oldName, newName, error);
}

bool IdmCopyControlBase::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	return m_dest->move(source, fileName, error);
}

IFileContainer *IdmCopyControlBase::open() const
{
	return m_dest->open();
}

IFileAccessor *IdmCopyControlBase::open(const QString &fileName, int mode, QString &error) const
{
	return m_dest->open(fileName, mode, error);
}

IFileContainer *IdmCopyControlBase::open(const QString &fileName, bool create, QString &error) const
{
	return m_dest->open(fileName, create, error);
}

const IFileContainerScanner *IdmCopyControlBase::scanner() const
{
	return m_dest->scanner();
}

INode *IdmCopyControlBase::node() const
{
	return m_dest->node();
}

bool IdmCopyControlBase::start(const Snapshot::List &files, bool move)
{
	return false;
}

void IdmCopyControlBase::done(bool error)
{
	m_dest->done(error);
}

void IdmCopyControlBase::canceled()
{
	m_dest->canceled();
}

QString IdmCopyControlBase::difference(const QString &path1, const QString &path2)
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
