#ifndef LIBARCHIVEUNPACKINTOSUBDIRACTIONTASK_H_
#define LIBARCHIVEUNPACKINTOSUBDIRACTIONTASK_H_

#include <QtCore/QCoreApplication>
#include <vfs/tools/vfs_filestree.h>
#include <vfs/tasks/vfs_performactiontask.h>
#include "../../libarchive_ns.h"
#include "../../../../../tools/taskspool/tryier.h"
#include "../../../../../tools/taskspool/questioner.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirActionTask : public PerformActionTask
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirActionTask)

public:
	UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files);

protected:
	virtual void process(const volatile Flags &aborted, QString &error);

private:
	typedef Tools::TasksPool::Tryier<UnPackIntoSubdirActionTask>     Tryier;
	typedef Tools::TasksPool::Questioner<UnPackIntoSubdirActionTask> Questioner;

	class OpenArchive
	{
	public:
		OpenArchive(const IFileContainer *container, const IFileInfo *file, IFileContainer::Holder &result) :
			m_container(container),
			m_file(file),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainer *m_container;
		const IFileInfo *m_file;
		IFileContainer::Holder &m_result;
	};

	class CreateDestination
	{
	public:
		CreateDestination(const IFileContainer *container, const IFileInfo *file, IFileContainer::Holder &result) :
			m_container(container),
			m_file(file),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainer *m_container;
		const IFileInfo *m_file;
		IFileContainer::Holder &m_result;
	};

	class OpenDestination
	{
	public:
		OpenDestination(FilesTree &tree, const IFileInfo *file, const IFileContainer *&result) :
			m_tree(tree),
			m_file(file),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		FilesTree &m_tree;
		const IFileInfo *m_file;
		const IFileContainer *&m_result;
	};

	class OpenArchiveFile
	{
	public:
		OpenArchiveFile(const IFileContainerScanner::IEnumerator *enumerator, IFileAccessor::Holder &result) :
			m_enumerator(enumerator),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainerScanner::IEnumerator *m_enumerator;
		IFileAccessor::Holder &m_result;
	};

	class OverwriteFile
	{
		Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirActionTask::OverwriteFile)

	public:
		OverwriteFile(const IFileContainer *container, const QString &fileName) :
			m_container(container),
			m_fileName(fileName)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainer *m_container;
		const QString &m_fileName;
	};

	class CreateFile
	{
	public:
		CreateFile(const IFileContainer *container, const QString &fileName, IFileAccessor::Holder &result) :
			m_container(container),
			m_fileName(fileName),
			m_result(result)
		{}

		inline bool operator()(QString &error) const;

	private:
		const IFileContainer *m_container;
		QString m_fileName;
		IFileAccessor::Holder &m_result;
	};

	bool askForOverwrite(const QString &error, Questioner::Tristate &flag, const volatile Flags &aborted);
	bool askForSkipIfNotCopy(const QString &error, bool &flag, const volatile Flags &aborted);

private:
	enum { FileReadWriteGranularity = 1 * 1024 * 1024 };

private:
	const IFileContainer *m_container;
	IFileAccessor::value_type m_buffer[FileReadWriteGranularity];
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVEUNPACKINTOSUBDIRACTIONTASK_H_ */