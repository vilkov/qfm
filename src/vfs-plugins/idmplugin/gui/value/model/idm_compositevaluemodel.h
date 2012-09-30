#ifndef IDM_COMPOSITEVALUEMODEL_H_
#define IDM_COMPOSITEVALUEMODEL_H_

#include <QtCore/QMap>
#include <vfs/containers/vfs_snapshot.h>
#include "../../../model/idm_model.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueModel : public Model
{
public:
	typedef CompositeEntityValue::List                    ValueList;
	typedef Container::size_type                             size_type;
	typedef QMap<Database::id_type, const SnapshotItem *> Files;

public:
	CompositeValueModel(const EntityValue::Holder &value, QObject *parent = 0);
	CompositeValueModel(const EntityValue::Holder &value, const Files &files, QObject *parent = 0);

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

	void add(const QModelIndex &index, const EntityValue::Holder &value);
	void add(const QModelIndex &index, const ValueList &values);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEMODEL_H_ */
