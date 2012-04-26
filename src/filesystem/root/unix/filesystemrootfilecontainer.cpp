#include "filesystemrootfilecontainer.h"
#include "../../../filesystemplugins/default/interfaces/defaultfilecontainer.h"


FILE_SYSTEM_NS_BEGIN

RootFileContainer::RootFileContainer() :
	m_path(QString::fromLatin1("/"))
{}

QString RootFileContainer::location() const
{
	return QString();
}

QString RootFileContainer::location(const QString &fileName) const
{
	return QString();
}

bool RootFileContainer::isPhysical() const
{
	return false;
}

IFileInfo::size_type RootFileContainer::freeSpace() const
{
	return 0;
}

ICopyControl *RootFileContainer::createControl(INodeView *view) const
{
	return NULL;
}

bool RootFileContainer::contains(const QString &fileName) const
{
	return fileName == m_path;
}

bool RootFileContainer::remove(const QString &fileName, QString &error) const
{
	return false;
}

bool RootFileContainer::rename(const QString &oldName, const QString &newName, QString &error) const
{
	return false;
}

bool RootFileContainer::move(const IFileContainer *source, const QString &fileName, QString &error) const
{
	return false;
}

IFileContainer *RootFileContainer::open() const
{
	return NULL;
}

IFileAccessor *RootFileContainer::open(const QString &fileName, int mode, QString &error) const
{
	return NULL;
}

IFileContainer *RootFileContainer::open(const QString &fileName, bool create, QString &error) const
{
	if (fileName == m_path)
		return new Plugins::Default::FileContainer(m_path);
	else
		return NULL;
}

const IFileContainerScanner *RootFileContainer::scanner() const
{
	return this;
}

virtual void RootFileContainer::enumerate(IEnumerator::Holder &enumerator) const
{

}

IFileInfo *RootFileContainer::info(const QString &fileName, QString &error) const
{
	return NULL;
}

void RootFileContainer::scan(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const
{

}

void RootFileContainer::refresh(Snapshot &snapshot, const volatile BaseTask::Flags &aborted) const
{

}

bool RootFileContainer::isDir() const
{
	return true;
}

bool RootFileContainer::isFile() const
{
	return false;
}

bool isLink() const
{
	return false;
}

RootFileContainer::size_type RootFileContainer::fileSize() const
{
	return 0;
}

QString RootFileContainer::fileName() const
{
	return m_path;
}

const IFileType *RootFileContainer::fileType() const
{
	return this;
}

QDateTime RootFileContainer::lastModified() const
{
	return QDateTime();
}

int RootFileContainer::permissions() const
{
	return Read;
}

FileTypeId RootFileContainer::id() const
{
	return FileTypeId();
}

QIcon RootFileContainer::icon() const
{
	return QIcon();
}

QString RootFileContainer::name() const
{
	return QString();
}

QString RootFileContainer::description() const
{
	return QString();
}

FILE_SYSTEM_NS_END
