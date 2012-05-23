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

	/* IFileLocation */
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;

	/* IFileContainer */
	virtual bool isPhysical() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual bool contains(const QString &fileName) const;
	virtual bool remove(const QString &fileName, QString &error) const;
	virtual bool rename(const QString &oldName, const QString &newName, QString &error) const;
	virtual bool move(const IFileContainer *source, const QString &fileName, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileAccessor *open(const QString &fileName, int flags, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;

private:
	FileContainer(IFileContainer::Holder &container, IFileAccessor::Holder &file, const QString &fileName);

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container, IFileAccessor::Holder &file, const QString &fileName) :
			container(container.take()),
			m_archive(this->container.data(), file),
			fileName(fileName)
		{}

		IFileContainer::Holder container;
		Scanner m_archive;
		QString fileName;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
	QString m_path;
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARFILECONTAINER_H_ */
