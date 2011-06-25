#ifndef SCANFILESTASKS_H_
#define SCANFILESTASKS_H_

#include "scanfilestask.h"


FILE_SYSTEM_NS_BEGIN

/********************************************************************************************************/
class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	typedef ScanFilesTask::Params              Params;
	typedef ModelEvents::ScanFilesForSizeEvent Event;

public:
	ScanFilesForSizeTask(QObject *listener, const Info &node, const QStringList &entries);

	virtual void run(const volatile bool &stopedFlag);
	inline Params *parameters() const { return static_cast<Params*>(ScanFilesTask::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	typedef ScanFilesTask::Params                Params;
	typedef ModelEvents::ScanFilesForRemoveEvent Event;

public:
	ScanFilesForRemoveTask(QObject *listener, const Info &node, const QStringList &entries);

	virtual void run(const volatile bool &stopedFlag);
	inline Params *parameters() const { return static_cast<Params*>(ScanFilesTask::parameters()); }
};


/********************************************************************************************************/
class ScanFilesWithDestinationTask : public ScanFilesTask
{
public:
	struct Params : public ScanFilesTask::Params
	{
		Params(QObject *listener, const Info &node, const QStringList &entries, IFileControl *destination) :
			ScanFilesTask::Params(listener, node, entries),
			destination(destination)
		{}

		IFileControl *destination;
	};

public:
	ScanFilesWithDestinationTask(Params *params);

	Params *parameters() const { return static_cast<Params*>(ScanFilesTask::parameters()); }
};


/********************************************************************************************************/
class ScanFilesForCopyTask : public ScanFilesWithDestinationTask
{
public:
	struct Params : public ScanFilesWithDestinationTask::Params
	{
		Params(QObject *listener, const Info &node, const QStringList &entries, IFileControl *destination, bool move) :
			ScanFilesWithDestinationTask::Params(listener, node, entries, destination),
			move(move)
		{}

		bool move;
	};
	typedef ModelEvents::ScanFilesForCopyEvent Event;

public:
	ScanFilesForCopyTask(QObject *listener, const Info &node, const QStringList &entries, IFileControl *destination, bool move);

	Params *parameters() const { return static_cast<Params*>(ScanFilesWithDestinationTask::parameters()); }
	virtual void run(const volatile bool &stopedFlag);
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASKS_H_ */
