#include "../filesysteminfo.h"
#include "../filesystemfile.h"
#include "../../tools/filesystemcommontools.h"
#include "../../../application.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>


FILE_SYSTEM_NS_BEGIN

static QString root = QString::fromLatin1("/");


static QString normalizeFilePath(const QString &filePath, bool &isRoot)
{
	QString res = filePath;

	if (res.startsWith(QChar('/')))
	{
		if (res.size() == 1)
			isRoot = true;

		return res;
	}
	else
	{
		char buff[PATH_MAX] = {};

		if (char *name = getcwd(buff, PATH_MAX))
			res.prepend(QString::fromUtf8(name));
	}

	if (res.endsWith(QChar('/')))
		res.chop(1);

	return res;
}


Info::Info() :
	m_isRoot(false)
{}

Info::Info(const QString &filePath) :
	m_isRoot(false),
	m_filePath(normalizeFilePath(filePath, m_isRoot)),
	m_fileName(filePath.mid(filePath.lastIndexOf(QChar('/')) + 1))
{
	refresh();
}

bool Info::isDir() const
{
	return m_info.isDir;
}

bool Info::isFile() const
{
	return m_info.isFile;
}

bool Info::isLink() const
{
	return m_info.isLink;
}

bool Info::exists() const
{
	struct stat st;
	return stat(m_filePath.toUtf8().constData(), &st) != ENOENT;
}

qint64 Info::fileSize() const
{
	return m_info.size;
}

QString Info::fileName() const
{
	return m_fileName;
}

QString Info::absolutePath() const
{
	return m_filePath.mid(0, m_filePath.lastIndexOf(QChar('/')));
}

QString Info::absoluteFilePath() const
{
	return m_filePath;
}

QString Info::absoluteFilePath(const QString &fileName) const
{
#ifdef Q_OS_WIN
	QString str = m_info.isDir() ? absoluteFilePath() : absolutePath();

	if (str.isEmpty())
		return fileName;
	else
		if (str.endsWith(QChar('/')))
			return str.append(fileName);
		else
			return str.append(QChar('/')).append(fileName);
#else
	QString str = m_info.isDir ? absoluteFilePath() : absolutePath();

	if (fileName == root)
		return fileName;
	else
		return str.append(QChar('/')).append(fileName);
#endif
}

QDateTime Info::lastModified() const
{
	return m_info.lastModified;
}

int Info::permissions() const
{
	return m_info.permissions;
}

void Info::refresh()
{
	m_info = Application::instance()->desktopEnvironment().info(m_filePath);
}

IFile::size_type Info::freeSpace() const
{
	return Tools::freeSpace(m_info.isDir ? absoluteFilePath().toUtf8() : absolutePath().toUtf8());
}

bool Info::contains(IFileControl *info) const
{
	return QDir(absoluteFilePath()).exists(info->fileName());
}

bool Info::rename(const QString &newFileName, QString &error) const
{
	QFile file(absoluteFilePath());

	if (file.rename(absoluteFilePath(newFileName)))
		return true;
	else
	{
		error = file.errorString();
		return false;
	}
}

IFile *Info::open(IFile::OpenMode mode, QString &error) const
{
	QFile::OpenMode openMode;
	PScopedPointer<FileSystem::File> file(new FileSystem::File(absoluteFilePath()));

	switch (mode)
	{
		case IFile::ReadOnly:
			openMode = QFile::ReadOnly;
			break;
		case IFile::WriteOnly:
			openMode = QFile::WriteOnly | QFile::Truncate;
			break;
	}

	if (file->open(openMode))
		return file.take();
	else
		error = file->lastError();

	return 0;
}

IFileControl *Info::open(IFileControl *info, QString &error) const
{
	return new Info(absoluteFilePath(info->fileName()));
}

IFileControl *Info::create(IFileControl *info, QString &error) const
{
	if (info->isFile())
		return new Info(absoluteFilePath(info->fileName()));
	else
		if (QDir(absoluteFilePath()).mkdir(info->fileName()))
			return new Info(absoluteFilePath(info->fileName()));
		else
			error = QString::fromLatin1("Failed to create directory \"%1\".").arg(absoluteFilePath(info->fileName()));

	return 0;
}

IFileControl *Info::create(const QString &name, FileType type, QString &error) const
{
	if (type == File)
		return new Info(absoluteFilePath(name));
	else
		if (QDir(absoluteFilePath()).mkdir(name))
			return new Info(absoluteFilePath(name));
		else
			error = QString::fromLatin1("Failed to create directory \"%1\".").arg(absoluteFilePath(name));

	return 0;
}

const QIcon &Info::icon() const
{
	return m_info.icon;
}

const QString &Info::displayType() const
{
	return m_info.type;
}

FILE_SYSTEM_NS_END
