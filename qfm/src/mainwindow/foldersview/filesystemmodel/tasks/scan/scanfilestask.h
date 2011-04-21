#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>
#include "filestask.h"
#include "../../items/filesystemtree.h"
#include "../../events/filesystemmodelevents.h"


/********************************************************************************************************/
template<typename BaseClass>
class ScanFilesTask : public TemplateFilesTask<BaseClass>
{
public:
	typedef TemplateFilesTask<BaseClass> parent_class;

public:
	struct Params : public parent_class::Params
	{
		typename parent_class::Params::Snapshot source;
		quint64 size;
		FileSystemTree *subtree;
	};
	struct EventParams : public parent_class::EventParams
	{
		typename parent_class::EventParams::Snapshot snapshot;
		quint64 size;
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	ScanFilesTask(Params *params) :
		parent_class(params)
	{
		Q_ASSERT(params->source.entry);
		Q_ASSERT(params->source.object);
		Q_ASSERT(params->source.fileSystemTree);
	}


	virtual void run(const volatile bool &stopedFlag)
	{
		QScopedPointer<FileSystemTree> subtree(new FileSystemTree(parameters()->source.entry->fileInfo().absoluteFilePath()));
		scan(subtree.data(), stopedFlag);
		parameters()->subtree = subtree.take();
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

private:
	void scan(FileSystemTree *tree, const volatile bool &stopedFlag)
	{
		QFileInfo info;
		QDirIterator dirIt(tree->fileInfo().absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && !parent_class::isControllerDead() && dirIt.hasNext())
		{
			dirIt.next();
			tree->add<FileSystemEntry>(getInfo(info = dirIt.fileInfo()));

			if (!info.isSymLink())
				if (info.isDir())
				{
					QScopedPointer<FileSystemTree> subtree(new FileSystemTree(info.absoluteFilePath(), tree));
					scan(subtree.data(), stopedFlag);
					tree->setSubtree(subtree.take());
				}
				else
					parameters()->size += info.size();
		}
	}
};

#endif /* SCANFILESTASK_H_ */
