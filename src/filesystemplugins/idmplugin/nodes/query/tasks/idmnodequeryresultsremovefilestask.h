#ifndef IDMNODEQUERYRESULTSREMOVETASK_H_
#define IDMNODEQUERYRESULTSREMOVETASK_H_

#include <QtCore/QList>
#include <QtCore/QModelIndex>
#include "idmnodequeryresultsevent.h"
#include "../items/idmqueryresultvalueitem.h"
#include "../../../../../filesystem/tasks/filesystembasetask.h"


IDM_PLUGIN_NS_BEGIN

class RemoveFilesTask : public BaseTask
{
public:
	typedef QList<QPair<QModelIndex, QueryResultValueItem*> > List;

public:
	RemoveFilesTask(QObject *receiver, const List &values, const List &files);

	virtual void run(const volatile bool &aborted);

private:
	List m_values;
	List m_files;
	List m_filed;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSREMOVETASK_H_ */
