#ifndef IDMQUERYRESULTSCOPYCONTROL_H_
#define IDMQUERYRESULTSCOPYCONTROL_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "../interface/idminodequeryresults.h"
#include "../../../containeres/idmcontainer.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/interfaces/filesystemicopycontrol.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryResultsCopyControl : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const IdmEntityValue::Holder &value, const IdmEntity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index);

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

	virtual const IFileContainerScanner *scanner() const;

	/* ICopyControl */
	virtual INode *node() const;

	virtual bool start(const Snapshot::List &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	ICopyControl::Holder m_dest;
	IdmContainer m_container;
	IdmEntityValue::Holder m_value;
	IdmEntity::Property m_property;
	IQueryResultsUpdater *m_model;
	QModelIndex m_index;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSCOPYCONTROL_H_ */
