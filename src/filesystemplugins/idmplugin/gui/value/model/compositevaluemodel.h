#ifndef COMPOSITEVALUEMODEL_H_
#define COMPOSITEVALUEMODEL_H_

#include <QtCore/QMap>
#include "../../../model/idmmodel.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/containers/filesystemsnapshot.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueModel : public IdmModel
{
public:
	typedef IdmCompositeEntityValue::List                    ValueList;
	typedef Container::size_type                             size_type;
	typedef QMap<Database::id_type, const WrappedNodeItem *> Files;

public:
	CompositeValueModel(const IdmEntityValue::Holder &value, QObject *parent = 0);
	CompositeValueModel(const IdmEntityValue::Holder &value, const Files &files, QObject *parent = 0);

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

	void add(const QModelIndex &index, const IdmEntityValue::Holder &value);
	void add(const QModelIndex &index, const ValueList &values);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEMODEL_H_ */
