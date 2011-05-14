#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QDir>
#include <QDirIterator>
#include <QScopedPointer>
#include "filestask.h"
#include "../../events/filesystemmodelevents.h"


FILE_SYSTEM_NS_BEGIN

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
		Node *subnode;
	};

public:
	ScanFilesTask(Params *params) :
		parent_class(params)
	{
		Q_ASSERT(params->source.node);
		Q_ASSERT(params->source.entry);
	}


	virtual void run(const volatile bool &stopedFlag)
	{
		QScopedPointer<ModelEvents::FolderNode> subnode(new ModelEvents::FolderNode(parameters()->source.entry->absoluteFilePath()));
		scan(subnode.data(), stopedFlag);
		parameters()->subnode = subnode.take();
	}

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

private:
	void scan(ModelEvents::FolderNode *node, const volatile bool &stopedFlag)
	{
		QFileInfo info;
		QDirIterator dirIt(node->absoluteFilePath(), QDir::AllEntries | QDir::System | QDir::Hidden | QDir::NoDotAndDotDot);

		while (!stopedFlag && !parent_class::isControllerDead() && dirIt.hasNext())
		{
			dirIt.next();
			node->add(getInfo(info = dirIt.fileInfo()));

			if (!info.isSymLink())
				if (info.isDir())
				{
					QScopedPointer<ModelEvents::FolderNode> subtree(new ModelEvents::FolderNode(info.absoluteFilePath(), node));
					scan(subtree.data(), stopedFlag);
					node->setSubnode(subtree.take());
				}
				else
					parameters()->size += info.size();
		}
	}
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
