#include "default_copycontrol.h"
#include "../../../application.h"

#include <vfs/tools/vfs_commontools.h>
#include <QtGui/QMessageBox>


DEFAULT_PLUGIN_NS_BEGIN

CopyControl::CopyControl(INode *node, const QByteArray &path) :
	m_container(path),
	m_node(node)
{}

bool CopyControl::isDefault() const
{
	return m_container.isDefault();
}

IFileInfo::size_type CopyControl::freeSpace() const
{
	return m_container.freeSpace();
}

ICopyControl *CopyControl::createControl(INodeView *view) const
{
	return m_container.createControl(view);
}

const Location &CopyControl::location() const
{
	return m_container.location();
}

Location CopyControl::location(const IFileInfo *info) const
{
	return m_container.location(info);
}

Location CopyControl::location(const QString &fileName) const
{
	return m_container.location(fileName);
}

bool CopyControl::contains(const QString &fileName) const
{
	return m_container.contains(fileName);
}

IFileInfo *CopyControl::info(const QString &fileName, QString &error) const
{
	return m_container.info(fileName, error);
}

bool CopyControl::remove(const IFileInfo *info, QString &error) const
{
	return m_container.remove(info, error);
}

bool CopyControl::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	return m_container.rename(info, fileName, error);
}

bool CopyControl::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	return m_container.move(source, info, error);
}

IFileContainer *CopyControl::open() const
{
	return m_container.open();
}

IFileContainer *CopyControl::open(const IFileInfo *info, QString &error) const
{
	return m_container.open(info, error);
}

IFileAccessor *CopyControl::open(const IFileInfo *info, int flags, QString &error) const
{
	return m_container.open(info, flags, error);
}

IFileContainer *CopyControl::create(const QString &fileName, QString &error) const
{
	return m_container.create(fileName, error);
}

IFileAccessor *CopyControl::create(const QString &fileName, int flags, QString &error) const
{
	return m_container.create(fileName, flags, error);
}

const IFileContainerScanner *CopyControl::scanner() const
{
	return m_container.scanner();
}

const IApplications *CopyControl::applications() const
{
	return m_container.applications();
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
								arg(location()).
								arg(Tools::humanReadableShortSize(fs)).
								arg(Tools::humanReadableShortSize(total)),
							QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void CopyControl::done(bool error)
{}

void CopyControl::canceled()
{}

DEFAULT_PLUGIN_NS_END
