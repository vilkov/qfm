#ifndef COMPOSITEVALUEMODEL_H_
#define COMPOSITEVALUEMODEL_H_

#include "../../../model/idmmodel.h"
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueModel : public IdmModel
{
public:
	typedef IdmCompositeEntityValue::List ValueList;
	typedef Container::size_type          size_type;

public:
	CompositeValueModel(IdmEntity *entity, QObject *parent = 0);

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

	void add(const QModelIndex &index, IdmEntityValue *value);
	void add(const QModelIndex &index, const ValueList &values);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEMODEL_H_ */
