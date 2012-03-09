#include "editablevaluelistmodel.h"


IDM_PLUGIN_NS_BEGIN

EditableValueListModel::EditableValueListModel(const IdmContainer &container, const Select &query, QObject *parent) :
	ValueListModel(container, query, parent)
{}

void EditableValueListModel::add(const List &list)
{
	ValueListModel::add(list);
}

QModelIndex EditableValueListModel::add(const IdmEntityValue::Holder &value)
{
	return ValueListModel::add(value);
}

void EditableValueListModel::remove(const QModelIndex &index)
{
	ValueListModel::remove(index);
}

IDM_PLUGIN_NS_END
