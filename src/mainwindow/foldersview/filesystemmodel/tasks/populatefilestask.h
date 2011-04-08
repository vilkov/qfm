#ifndef POPULATEFILESTASK_H_
#define POPULATEFILESTASK_H_

#include "filestask.h"


class PopulateFilesTask : public FilesTask
{
public:
	PopulateFilesTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver);

	FileSystemEntry *entry() const { return m_entry; }
	FileSystemTree *subtree() const { return m_subtree; }

	virtual void run(const volatile bool &stopedFlag);

private:
	void populate(FileSystemTree *tree, const volatile bool &stopedFlag);

private:
	FileSystemEntry *m_entry;
	FileSystemTree *m_subtree;
};


class PopulateFilesForRemoveTask : public PopulateFilesTask
{
public:
	PopulateFilesForRemoveTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* POPULATEFILESTASK_H_ */
