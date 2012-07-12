#include "libunrarfilecontainer.h"
#include "../../../default/interfaces/defaultfileinfo.h"
#include "../../../default/interfaces/defaultfilecontainer.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

IFileContainer *FileContainer::create(const IFileContainer *container, const IFileInfo *file, QString &error)
{
	if (container->isDefault())
	{
		IFileContainer::Holder localContainer(container->open());

		if (localContainer)
			return new FileContainer(localContainer, QByteArray(localContainer.as<Default::BaseFileContainer>()->m_path).append('/').append(static_cast<const Default::Info *>(file)->rawFileName()), file->fileName());
	}

	return NULL;
}

QString FileContainer::extractDirectoryName(const IFileInfo *file)
{
	QString fileName = file->fileName();
	return fileName.mid(0, fileName.indexOf(QChar('.')));
}

QString FileContainer::extractArchivedFileName(const IFileInfo *file)
{
	QString fileName = file->fileName();
	return fileName.mid(fileName.lastIndexOf(QChar('/')) + 1);
}

bool FileContainer::isDefault() const
{
	return false;
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	return m_data->container->freeSpace();
}

ICopyControl *FileContainer::createControl(INodeView *view) const
{
	return NULL;
}

QString FileContainer::location() const
{
	return m_path;
}

bool FileContainer::contains(const QString &fileName) const
{
	return false;
}

IFileInfo *FileContainer::info(const QString &fileName, QString &error) const
{
	return NULL;
}

bool FileContainer::remove(const IFileInfo *info, QString &error) const
{
	return false;
}

bool FileContainer::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	return false;
}

bool FileContainer::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	return false;
}

IFileContainer *FileContainer::open() const
{
	return new FileContainer(*this);
}

IFileContainer *FileContainer::create(const QString &fileName, QString &error) const
{
	return NULL;
}

IFileAccessor *FileContainer::create(const QString &fileName, int flags, QString &error) const
{
	return NULL;
}

IFileContainer *FileContainer::open(const IFileInfo *info, QString &error) const
{
	return NULL;
}

IFileAccessor *FileContainer::open(const IFileInfo *info, int flags, QString &error) const
{
	return NULL;
}

IFileContainer *FileContainer::filter(Filter::Holder &filter, QString &error) const
{
	return NULL;
}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_data->m_scanner;
}

FileContainer::FileContainer(IFileContainer::Holder &container, const QByteArray &filePath, const QString &fileName) :
	m_data(new Data(container, filePath)),
	m_path(m_data->container->location().append(QChar('/')).append(fileName))
{}

LIBUNRAR_ARC_PLUGIN_NS_END
