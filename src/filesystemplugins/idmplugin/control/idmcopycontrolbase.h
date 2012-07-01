#ifndef IDMCOPYCONTROLBASE_H_
#define IDMCOPYCONTROLBASE_H_

#include "../containeres/idmcontainer.h"
#include "../../../filesystem/interfaces/filesystemicopycontrol.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControlBase : public ICopyControl
{
public:
	IdmCopyControlBase(ICopyControl::Holder &dest, const IdmContainer &container);

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
	virtual IFileAccessor *open(const QString &fileName, int mode, QString &error) const;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const;
	virtual IFileContainer *filter(Filter::Holder &filter, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;

	/* ICopyControl */
	virtual INode *node() const;

	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

protected:
	static QString difference(const QString &path1, const QString &path2);

protected:
	ICopyControl::Holder m_dest;
	IdmContainer m_container;
	QString m_storage;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROLBASE_H_ */
