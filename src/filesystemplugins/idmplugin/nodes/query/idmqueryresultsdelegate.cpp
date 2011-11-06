#include "idmqueryresultsdelegate.h"
#include "items/idmqueryresultvalueitem.h"
#include "../../../../tools/widgets/valuedialog/valuedialogmetafunctions.h"


IDM_PLUGIN_NS_BEGIN

IdmQueryResultsDelegate::IdmQueryResultsDelegate(const IdmContainer &container, QObject *parent) :
	IdmDelegate(parent),
    m_container(container)
{}

QWidget *IdmQueryResultsDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	switch (static_cast<QueryResultValueItem*>(index.internalPointer())->value()->entity()->type())
	{
		case Database::Int:
			return new Editor<typename EntityValueType<Database::Int>::type>::type(parent);

		case Database::String:
			return new Editor<typename EntityValueType<Database::String>::type>::type(parent);

		case Database::Date:
			return new Editor<typename EntityValueType<Database::Date>::type>::type(parent);

		case Database::Time:
			return new Editor<typename EntityValueType<Database::Time>::type>::type(parent);

		case Database::DateTime:
			return new Editor<typename EntityValueType<Database::DateTime>::type>::type(parent);

		case Database::Memo:
			return new Editor<typename EntityValueType<Database::Memo>::type>::type(parent);

		case Database::Rating:
			return new Editor<typename EntityValueType<Database::Rating>::type>::type(parent);

		default:
			break;
	}

	return 0;
}

void IdmQueryResultsDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	switch (static_cast<QueryResultValueItem*>(index.internalPointer())->value()->entity()->type())
	{
		case Database::Int:
			EditorValue<typename EntityValueType<Database::Int>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::String:
			EditorValue<typename EntityValueType<Database::String>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::Date:
			EditorValue<typename EntityValueType<Database::Date>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::Time:
			EditorValue<typename EntityValueType<Database::Time>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::DateTime:
			EditorValue<typename EntityValueType<Database::DateTime>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::Memo:
			EditorValue<typename EntityValueType<Database::Memo>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		case Database::Rating:
			EditorValue<typename EntityValueType<Database::Rating>::type>::setValue(editor, index.data(Qt::DisplayRole));
			break;

		default:
			break;
	}
}

void IdmQueryResultsDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}

IDM_PLUGIN_NS_END
