#include "filesystemfoldernodeinfo.h"
#include "filesystemfoldernodefile.h"
#include "../../../application.h"
#include <QtCore/QFSFileEngine>


FILE_SYSTEM_NS_BEGIN

#ifndef Q_OS_WIN
QString Info::root = QString::fromLatin1("/");
#endif


Info::Info() :
	m_info()
{}

Info::Info(const QString &filePath) :
	m_info(filePath)
{
#ifdef Q_OS_WIN
	m_permissions = m_info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

Info::Info(const QFileInfo &info) :
	m_info(info)
{
#ifdef Q_OS_WIN
	m_permissions = m_info.permissions();
#else
	translatePermissions(getuid(), getgid());
#endif
}

#ifndef Q_OS_WIN
Info::Info(const QString &filePath, uint userId, uint groupId) :
	m_info(filePath)
{
	translatePermissions(userId, groupId);
}

Info::Info(const QFileInfo &info, uint userId, uint groupId) :
	m_info(info)
{
	translatePermissions(userId, groupId);
}
#endif

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
	QString str = m_info.isDir() ? absoluteFilePath() : absolutePath();

	if (fileName == root)
		return fileName;
	else
		return str.append(QChar('/')).append(fileName);
#endif
}

bool Info::exists(IFileInfo *info) const
{
	return QDir(m_info.absoluteFilePath()).exists(info->fileName());
}

void Info::refresh()
{
	m_displayType = QString();
	m_icon = QIcon();
	m_info.refresh();
}

IFile *Info::open(IFile::OpenMode mode, QString &error) const
{
	QFile::OpenMode openMode;
	QScopedPointer<FileSystem::File> file(new FileSystem::File(absoluteFilePath()));

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

void Info::close(IFile *file) const
{
	delete file;
}

IFileControl *Info::open(const QString &fileName, QString &error) const
{
	return new Info(absoluteFilePath(fileName));
}

IFileControl *Info::create(IFileInfo *info, QString &error) const
{
	if (info->isFile())
		return new Info(absoluteFilePath(info->fileName()));
	else
		if (m_info.absoluteDir().mkdir(info->fileName()))
			return new Info(absoluteFilePath(info->fileName()));
		else
			error = QString::fromLatin1("Failed to create directory \"%1\".").arg(absoluteFilePath(info->fileName()));

	return 0;
}

IFileControl *Info::create(const QString &fileName, FileType type, QString &error) const
{
	if (type == File)
		return new Info(absoluteFilePath(fileName));
	else
		if (m_info.absoluteDir().mkdir(fileName))
			return new Info(absoluteFilePath(fileName));
		else
			error = QString::fromLatin1("Failed to create directory \"%1\".").arg(absoluteFilePath(fileName));

	return 0;
}

void Info::close(IFileControl *info) const
{
	delete info;
}

const QIcon &Info::icon() const
{
	if (m_icon.isNull())
		m_icon = Application::instance()->iconProvider().icon(m_info);

	return m_icon;
}

const QString &Info::displayType() const
{
	if (m_displayType.isEmpty())
		m_displayType = Application::instance()->iconProvider().type(m_info);

	return m_displayType;
}

bool Info::isCaseSensitive() const
{
	return QFSFileEngine(absoluteFilePath()).caseSensitive();
}

#ifndef Q_OS_WIN
void Info::translatePermissions(uint userId, uint groupId)
{
	QFile::Permissions perm = m_info.permissions();
	QFile::Permissions p = perm;
	p &= ~(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser);

	if (perm & QFile::ReadOther || (m_info.ownerId() == userId  && perm & QFile::ReadOwner) ||
                                   (m_info.groupId() == groupId && perm & QFile::ReadGroup))
		p |= QFile::ReadUser;

	if (perm & QFile::WriteOther || (m_info.ownerId() == userId  && perm & QFile::WriteOwner) ||
                                    (m_info.groupId() == groupId && perm & QFile::WriteGroup))
		p |= QFile::WriteUser;

	if (perm & QFile::ExeOther || (m_info.ownerId() == userId  && perm & QFile::ExeOwner) ||
                                  (m_info.groupId() == groupId && perm & QFile::ExeGroup))
		p |= QFile::ExeUser;

	m_permissions = p;
}
#endif

FILE_SYSTEM_NS_END
