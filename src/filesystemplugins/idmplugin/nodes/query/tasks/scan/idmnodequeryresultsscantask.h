#ifndef IDMNODEQUERYRESULTSSCANTASK_H_
#define IDMNODEQUERYRESULTSSCANTASK_H_

#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include "../../items/idmqueryresultvalueitem.h"
#include "../../../../../../filesystem/tasks/scan/scanfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	typedef QList<QPair<QModelIndex, QueryResultValueItem*> > List;

public:
	ScanFilesTask(TasksNode *receiver, const List &files);

	virtual void run(const volatile bool &aborted);

private:
	List m_files;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSSCANTASK_H_ */
