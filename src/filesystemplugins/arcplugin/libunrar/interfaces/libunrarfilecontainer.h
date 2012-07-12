#ifndef LIBUNRARFILECONTAINER_H_
#define LIBUNRARFILECONTAINER_H_

#include <QtCore/QSharedData>
#include "libunrarscanner.h"
#include "../../../../filesystem/interfaces/filesystemifilecontainer.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer
{
public:
	static IFileContainer *create(const IFileContainer *container, const IFileInfo *file, QString &error);
	static QString extractDirectoryName(const IFileInfo *file);
	static QString extractArchivedFileName(const IFileInfo *file);

	/* IFileContainer */
	virtual bool isDefault() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual QString location() const;
	virtual bool contains(const QString &fileName) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;

	virtual bool remove(const IFileInfo *info, QString &error) const;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const;
	virtual IFileContainer *create(const QString &fileName, QString &error) const;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const;

	virtual IFileContainer *filter(Filter::Holder &filter, QString &error) const;
	virtual const IFileContainerScanner *scanner() const;

private:
	FileContainer(IFileContainer::Holder &container, const QByteArray &filePath, const QString &fileName);

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container, const QByteArray &filePath) :
			container(container.take()),
			m_scanner(this->container.data(), filePath)
		{}

		IFileContainer::Holder container;
		Scanner m_scanner;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
	QString m_path;
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARFILECONTAINER_H_ */
