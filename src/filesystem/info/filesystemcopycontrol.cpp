#include "filesystemcopycontrol.h"
#include "../tools/filesystemcommontools.h"
#include "../../application.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

CopyControl::CopyControl(const Info &info) :
	m_container(info)
{}

bool CopyControl::isPhysical() const
{
	return m_container.isPhysical();
}

QString CopyControl::location() const
{
	return m_container.location();
}

QString CopyControl::location(const QString &fileName) const
{
	return m_container.location(fileName);
}

IFileInfo::size_type CopyControl::freeSpace() const
{
	return m_container.freeSpace();
}

bool CopyControl::contains(const QString &fileName) const
{
	return m_container.contains(fileName);
}

bool CopyControl::remove(const QString &fileName, QString &error) const
{
	return m_container.remove(fileName, error);
}

IFile *CopyControl::open(const QString &fileName) const
{
	return m_container.open(fileName);
}

IFileContainer *CopyControl::open(const QString &fileName, bool create, QString &error) const
{
	return m_container.open(fileName, create, error);
}

bool CopyControl::copy(const IFileInfo *source, QString &error) const
{
	return m_container.copy(source, error);
}

bool CopyControl::start(const ScanedFiles::Files &files, bool move)
{
	IFile::size_type fs;

	return (files.totalSize() <= (fs = freeSpace()) ||
		QMessageBox::question(
							Application::mainWindow(),
							tr("Insufficient space on device"),
							tr("Destination \"%1\" (%2) doesn't have enough free space (%3). Continue?").
								arg(info().absoluteFilePath()).
								arg(Tools::humanReadableShortSize(fs)).
								arg(Tools::humanReadableShortSize(files.totalSize())),
							QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void CopyControl::done(bool error)
{}

void CopyControl::canceled()
{}

FILE_SYSTEM_NS_END
