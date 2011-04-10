#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QAbstractItemModel>
#include "filestask.h"


class ScanFilesTask : public FilesTask
{
public:
	struct Params : public FilesTask::Params
	{
		quint64 size;
		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	struct EventParams : public FilesTask::EventParams
	{
		quint64 size;
		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesTask(Params *parameters);

	virtual void run(const volatile bool &stopedFlag);

protected:
	Params *parameters() const { return static_cast<Params*>(FilesTask::parameters()); }

private:
	void scan(FileSystemTree *tree, const volatile bool &stopedFlag);
};


class ScanFilesForSizeTask : public ScanFilesTask
{
public:
	ScanFilesForSizeTask(Params *parameters);

	virtual void run(const volatile bool &stopedFlag);
};


class ScanFilesForRemoveTask : public ScanFilesTask
{
public:
	ScanFilesForRemoveTask(Params *parameters);

	virtual void run(const volatile bool &stopedFlag);
};


class ScanFilesForCopyTask : public ScanFilesTask
{
public:
	struct Params : public ScanFilesTask::Params
	{
		QAbstractItemModel *destination;
	};
	struct EventParams : public ScanFilesTask::EventParams
	{
		QAbstractItemModel *destination;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesForCopyTask(Params *parameters);

	virtual void run(const volatile bool &stopedFlag);

protected:
	Params *parameters() const { return static_cast<Params*>(FilesTask::parameters()); }
};


class ScanFilesForMoveTask : public ScanFilesForCopyTask
{
public:
	ScanFilesForMoveTask(Params *parameters);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* SCANFILESTASK_H_ */
