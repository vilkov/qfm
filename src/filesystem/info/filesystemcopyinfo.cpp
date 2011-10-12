#include "filesystemcopyinfo.h"
#include "../tools/filesystemcommontools.h"
#include "../../application.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

CopyInfo::CopyInfo(const Info &info) :
	m_info(info)
{}

bool CopyInfo::isDir() const
{
	return m_info.isDir();
}

bool CopyInfo::isFile() const
{
	return m_info.isFile();
}

bool CopyInfo::exists() const
{
	return m_info.exists();
}

QString CopyInfo::fileName() const
{
	return m_info.fileName();
}

QString CopyInfo::absolutePath() const
{
	return m_info.absolutePath();
}

QString CopyInfo::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString CopyInfo::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime CopyInfo::lastModified() const
{
	return m_info.lastModified();
}

void CopyInfo::refresh()
{
	m_info.refresh();
}

IFile::size_type CopyInfo::freeSpace() const
{
	return m_info.freeSpace();
}

bool CopyInfo::contains(IFileControl *info) const
{
	return m_info.contains(info);
}

bool CopyInfo::rename(const QString &newFileName, QString &error) const
{
	return m_info.rename(newFileName, error);
}

IFile *CopyInfo::open(IFile::OpenMode mode, QString &error) const
{
	return m_info.open(mode, error);
}

IFileControl *CopyInfo::open(IFileControl *info, QString &error) const
{
	return m_info.open(info, error);
}

IFileControl *CopyInfo::create(IFileControl *info, QString &error) const
{
	return m_info.create(info, error);
}

IFileControl *CopyInfo::create(const QString &name, FileType type, QString &error) const
{
	return m_info.create(name, type, error);
}

bool CopyInfo::physicalCopyIsNecessary() const
{
	return true;
}

bool CopyInfo::start(const InfoListItem *files, bool move)
{
	IFile::size_type fs;

	return (files->totalSize() <= (fs = freeSpace()) ||
		QMessageBox::question(
							&Application::instance()->mainWindow(),
							tr("Insufficient space on device"),
							tr("Destination \"%1\" (%2) doesn't have enough free space (%3). Continue?").
								arg(absoluteFilePath()).
								arg(Tools::humanReadableShortSize(fs)).
								arg(Tools::humanReadableShortSize(files->totalSize())),
							QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void CopyInfo::done(bool error)
{

}

void CopyInfo::canceled()
{

}

FILE_SYSTEM_NS_END
