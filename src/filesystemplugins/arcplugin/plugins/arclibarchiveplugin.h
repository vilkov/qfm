#ifndef ARCLIBARCHIVEPLUGIN_H_
#define ARCLIBARCHIVEPLUGIN_H_

#include <QtCore/QCoreApplication>
#include "../archive/arcarchive.h"


ARC_PLUGIN_NS_BEGIN

class LibArchivePlugin : public Archive
{
	Q_DECLARE_TR_FUNCTIONS(LibArchivePlugin)

public:
	LibArchivePlugin();

	virtual State *beginRead(const QString &fileName) const;
	virtual Contents readAll(State *state, const volatile Flags &aborted) const;
	virtual void extract(State *state, const ArcNodeItem *entry, const IFileContainer *dest, Callback *callback, const volatile Flags &aborted) const;
	virtual void extractAll(State *state, const IFileContainer *dest, Callback *callback, const volatile Flags &aborted) const;
	virtual void endRead(State *state) const;

private:
	void extractEntry(State *state, const IFileContainer *destination, const ArcNodeListItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) const;
	void extractFile(State *state, const IFileContainer *destination, const ArcNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) const;
	void doExtractFile(State *state, const IFileContainer *destination, const ArcNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) const;
	void doExtractFile(State *state, const IFileContainer *destination, const QString &fileName, volatile bool &tryAgain, const volatile Flags &aborted) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCLIBARCHIVEPLUGIN_H_ */
