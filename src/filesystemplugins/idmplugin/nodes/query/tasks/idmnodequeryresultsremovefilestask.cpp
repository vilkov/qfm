#include "idmnodequeryresultsremovefilestask.h"
#include "../items/idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

RemoveFilesTask::RemoveFilesTask(QObject *receiver, const List &values, const List &files) :
	BaseTask(receiver),
	m_values(values),
	m_files(files)
{}

void RemoveFilesTask::run(const volatile bool &aborted)
{
	QueryResultPathValueItem *item;

	for (List::size_type i = 0, size = m_files.size(); i < size; ++i)
	{
		item = static_cast<QueryResultPathValueItem*>(m_files.at(i).second);
		item->info().refresh();

		if (item->info().exists())
		{
			item->info();
		}
	}
}

IDM_PLUGIN_NS_END
