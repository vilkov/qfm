#ifndef ARCLIBARCHIVEPLUGIN_H_
#define ARCLIBARCHIVEPLUGIN_H_

#include "../archive/arcarchive.h"
#include <QtCore/QCoreApplication>


ARC_PLUGIN_NS_BEGIN

class LibArchivePlugin : public Archive
{
	Q_DECLARE_TR_FUNCTIONS(LibArchivePlugin)

public:
	LibArchivePlugin();

	virtual State *beginRead(const QString &fileName) const;
	virtual Contents readAll(State *state, const volatile Flags &aborted) const;
	virtual void extract(State *state, const ArcNodeItem::Base *entry, const IFileControl *dest, IFile::value_type *buffer, IFile::size_type bufferSize, const volatile Flags &aborted) const;
	virtual void endRead(State *state) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCLIBARCHIVEPLUGIN_H_ */
