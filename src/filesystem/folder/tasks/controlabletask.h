#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QSharedData>
#include "../../filesystem_ns.h"
#include "../events/filesystemmodelevents.h"


FILE_SYSTEM_NS_BEGIN

class ControlableTask : public ModelEvents::BaseTask
{
public:
	struct Params : public QSharedData, public ModelEvents::BaseTask::Params
	{};

public:
	ControlableTask(Params *parameters) :
		m_params(parameters),
		m_controllerDead(false)
	{
		Q_ASSERT(m_params);
	}


protected:
	inline const volatile bool &isControllerDead() const { return m_controllerDead; }

protected:
	typedef QExplicitlySharedDataPointer<Params> ParamsPointer;
	inline Params *parameters() const { return m_params.data(); }
	const ParamsPointer &params() const { return m_params; }

protected:
	class FolderNode : public FileSystem::FolderNode
	{
		Q_DISABLE_COPY(FolderNode)

	public:
		FolderNode(const Info &info, Node *parent = 0) :
			FileSystem::FolderNode(info, parent)
		{}

		void add(const Info &info) { items().add(new FolderNodeEntry(info)); }
		void setSubnode(FileSystem::FolderNode *subnode)
		{
			FileSystem::FolderNode::Values::size_type index = items().indexOf(subnode->fileName());

			if (index != FileSystem::FolderNode::Values::InvalidIndex)
				items()[index].node = subnode;
		}
	};

private:
    ParamsPointer m_params;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* CONTROLABLETASK_H_ */
