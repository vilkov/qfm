#ifndef ARCARCHIVE_H_
#define ARCARCHIVE_H_

#include <QtCore/QString>
#include "../nodes/items/arcnodeitem.h"
#include "../../../filesystem/tasks/filesystembasetask.h"
#include "../../../filesystem/interfaces/filesystemifilecontrol.h"


ARC_PLUGIN_NS_BEGIN

class Archive
{
public:
	typedef BaseTask::Task::Flags      Flags;
	typedef QList<ArcNodeItem::Base *> Container;
	struct Contents
	{
		Contents() :
			extractedSize(0)
		{}

		Container items;
		qint64 extractedSize;
	};

	struct State
	{
		QString error;
	};

public:
	virtual ~Archive();

	static const Archive *archive(const QString &fileName, State **state);

	virtual State *beginRead(const QString &fileName) const = 0;
	virtual Contents readAll(State *state, const volatile Flags &aborted) const = 0;
	virtual void extract(State *state, const ArcNodeItem::Base *entry, const IFileControl *dest, const volatile Flags &aborted) const = 0;
	virtual void endRead(State *state) const = 0;
};

ARC_PLUGIN_NS_END

#endif /* ARCARCHIVE_H_ */
