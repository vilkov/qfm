#include "defaultcopycontrol.h"
#include "../../../filesystem/tools/filesystemcommontools.h"
#include "../../../application.h"

#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

CopyControl::CopyControl(INode *node, const QString &path) :
	m_node(node),
	m_container(path)
{}

QString CopyControl::location() const
{
	return m_container.location();
}

QString CopyControl::location(const QString &fileName) const
{
	return m_container.location(fileName);
}

bool CopyControl::isPhysical() const
{
	return m_container.isPhysical();
}

IFileInfo::size_type CopyControl::freeSpace() const
{
	return m_container.freeSpace();
}

ICopyControl *CopyControl::createControl(INodeView *view) const
{
	return new CopyControl(*this);
}

bool CopyControl::contains(const QString &fileName) const
{
	return m_container.contains(fileName);
}

bool CopyControl::remove(const QString &fileName, QString &error) const
{
	return m_container.remove(fileName, error);
}

bool CopyControl::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return m_container.rename(oldName, newName, error);
}

bool CopyControl::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	return m_container.move(source, fileName, error);
}

IFileContainer *CopyControl::open() const
{
	return m_container.open();
}

IFileAccessor *CopyControl::open(const QString &fileName, int flags, QString &error) const
{
	return m_container.open(fileName, flags, error);
}

IFileContainer *CopyControl::open(const QString &fileName, bool create, QString &error) const
{
	return m_container.open(fileName, create, error);
}

const IFileContainerScanner *CopyControl::scanner() const
{
	return m_container.scanner();
}

INode *CopyControl::node() const
{
	return m_node;
}

bool CopyControl::start(const Snapshot &files, bool move)
{
	IFileInfo::size_type fs;
	IFileInfo::size_type total;

	return ((total = files.totalSize()) <= (fs = freeSpace()) ||
		QMessageBox::question(
							Application::mainWindow(),
							tr("Insufficient space on device"),
							tr("Destination \"%1\" (%2) doesn't have enough free space (%3). Continue?").
								arg(m_container.location()).
								arg(Tools::humanReadableShortSize(fs)).
								arg(Tools::humanReadableShortSize(total)),
							QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void CopyControl::done(bool error)
{}

void CopyControl::canceled()
{}

DEFAULT_PLUGIN_NS_END
