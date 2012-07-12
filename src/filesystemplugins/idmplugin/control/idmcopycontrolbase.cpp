#include "idmcopycontrolbase.h"
#include "../../../filesystem/tools/filesystempath.h"


IDM_PLUGIN_NS_BEGIN

IdmCopyControlBase::IdmCopyControlBase(ICopyControl::Holder &dest, const IdmContainer &container) :
	m_dest(dest.take()),
	m_container(container),
	m_storage(difference(m_dest->location(), m_container.container()->location()).append(QChar('/')))
{}

bool IdmCopyControlBase::isDefault() const
{
	return m_dest->isDefault();
}

IFileInfo::size_type IdmCopyControlBase::freeSpace() const
{
	return m_dest->freeSpace();
}

ICopyControl *IdmCopyControlBase::createControl(INodeView *view) const
{
	return NULL;
}

QString IdmCopyControlBase::location() const
{
	return m_dest->location();
}

bool IdmCopyControlBase::contains(const QString &fileName) const
{
	return m_dest->contains(fileName);
}

IFileInfo *IdmCopyControlBase::info(const QString &fileName, QString &error) const
{
	return m_dest->info(fileName, error);
}

bool IdmCopyControlBase::remove(const IFileInfo *info, QString &error) const
{
	return m_dest->remove(info, error);
}

bool IdmCopyControlBase::rename(const IFileInfo *oldInfo, const QString &newName, QString &error) const
{
	return m_dest->rename(oldInfo, newName, error);
}

bool IdmCopyControlBase::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	return m_dest->move(source, info, error);
}

IFileContainer *IdmCopyControlBase::open() const
{
	return m_dest->open();
}

IFileContainer *IdmCopyControlBase::open(const IFileInfo *info, QString &error) const
{
	return m_dest->open(info, error);
}

IFileAccessor *IdmCopyControlBase::open(const IFileInfo *info, int flags, QString &error) const
{
	return m_dest->open(info, flags, error);
}

IFileContainer *IdmCopyControlBase::create(const QString &fileName, QString &error) const
{
	return m_dest->create(fileName, error);
}

IFileAccessor *IdmCopyControlBase::create(const QString &fileName, int flags, QString &error) const
{
	return m_dest->create(fileName, flags, error);
}

IFileContainer *IdmCopyControlBase::filter(Filter::Holder &filter, QString &error) const
{
	return NULL;
}

const IFileContainerScanner *IdmCopyControlBase::scanner() const
{
	return m_dest->scanner();
}

INode *IdmCopyControlBase::node() const
{
	return m_dest->node();
}

bool IdmCopyControlBase::start(const Snapshot &files, bool move)
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
