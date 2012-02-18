#ifndef ARCUNPACKINTOSUBDIRACTION_H_
#define ARCUNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"
#include "../archive/arcarchive.h"


ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirAction : public Action, public Archive::Callback
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;

	/* Archive::Callback */
	virtual IFile::value_type *buffer() const;
	virtual IFile::size_type bufferSize() const;

	virtual void progressInit(const FileSystemItem *item);
	virtual void progressUpdate(quint64 progressIncrement);
	virtual void progresscomplete();

	virtual bool overwriteAll() const;
	virtual bool skipAllIfNotCopy() const;
	virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);
	virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted);

private:
	QString folderName(const QString &fileName) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKINTOSUBDIRACTION_H_ */
