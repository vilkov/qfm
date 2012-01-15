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

	class Callback
	{
	public:
		virtual ~Callback();

		virtual IFile::value_type *buffer() const = 0;
		virtual IFile::size_type bufferSize() const = 0;

		virtual void progressInit(const ArcNodeItem::Base *item) = 0;
		virtual void progressUpdate(quint64 progressIncrement) = 0;
		virtual void progresscomplete() = 0;

		virtual bool overwriteAll() const = 0;
		virtual bool skipAllIfNotCopy() const = 0;
		virtual void askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted) = 0;
		virtual void askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted) = 0;
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
	virtual void extract(State *state, const ArcNodeItem::Base *entry, const IFileControl *dest, Callback *callback, const volatile Flags &aborted) const = 0;
	virtual void endRead(State *state) const = 0;
};

ARC_PLUGIN_NS_END

#endif /* ARCARCHIVE_H_ */
