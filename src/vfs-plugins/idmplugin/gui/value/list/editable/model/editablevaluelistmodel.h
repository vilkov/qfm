#ifndef EDITABLEVALUELISTMODEL_H_
#define EDITABLEVALUELISTMODEL_H_

#include "../../../model/valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

class EditableValueListModel : public ValueListModel
{
public:
	EditableValueListModel(const IdmContainer &container, const Select &query, QObject *parent = 0);

	void add(const List &list);
	QModelIndex add(const IdmEntityValue::Holder &value);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* EDITABLEVALUELISTMODEL_H_ */
