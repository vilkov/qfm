#include "idmcontainer.h"
#include "../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmContainer::IdmContainer(const Info &storage) :
	m_data(new Data(storage))
{
	m_data->menuActions.push_back(new QAction(tr("Create"), 0));
	m_data->menuActions.last()->setData(Create);

	m_data->menuActions.push_back(new QAction(tr("Query"), 0));
	m_data->menuActions.last()->setData(Query);

	m_data->menuActions.push_back(new QAction(tr("List"), 0));
	m_data->menuActions.last()->setData(List);

	m_data->entityTypes[Database::Int]       = IdmEntityTypeDescription(tr("Int"),       tr("Integer type"));
	m_data->entityTypes[Database::String]    = IdmEntityTypeDescription(tr("String"),    tr("String type (max length is 256 characters)"));
	m_data->entityTypes[Database::Date]      = IdmEntityTypeDescription(tr("Date"),      tr("Date type"));
	m_data->entityTypes[Database::Time]      = IdmEntityTypeDescription(tr("Time"),      tr("Time type"));
	m_data->entityTypes[Database::DateTime]  = IdmEntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
	m_data->entityTypes[Database::Memo]      = IdmEntityTypeDescription(tr("Memo"),      tr("Memo type"));
	m_data->entityTypes[Database::Composite] = IdmEntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
	m_data->entityTypes[Database::Rating]    = IdmEntityTypeDescription(tr("Rating"),    tr("Rating type (shown as stars)"));
	m_data->entityTypes[Database::Path]      = IdmEntityTypeDescription(tr("Path"),      tr("Path to file or folder"));
}

IDM_PLUGIN_NS_END
