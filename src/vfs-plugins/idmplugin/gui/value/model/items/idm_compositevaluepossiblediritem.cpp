#include "idm_compositevaluepossiblediritem.h"
#include "idm_compositevaluepossiblefileitem.h"
#include "../../../../../../application.h"

#include <vfs/model/vfs_proxymodel.h>


IDM_PLUGIN_NS_BEGIN

class CompositeValueFakePossibleDirItem : public CompositeValuePossibleDirItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValueFakePossibleDirItem(const SnapshotItem *source, IdmItem *parent = 0) :
		CompositeValuePossibleDirItem(EntityValue::Holder(), source, parent)
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		switch (role)
		{
			case Qt::EditRole:
			case Qt::DisplayRole:
				return source()->info()->fileName().as<QString>();
			case Qt::DecorationRole:
				return source()->info()->fileType()->icon();
			case Qt::TextAlignmentRole:
				return Qt::AlignLeft;
			case Qt::ToolTipRole:
				return source()->info()->fileType()->name();
		}

		return QVariant();
	}
};


class CompositeValueFakePossibleFileItem : public CompositeValuePossibleFileItem
{
public:
	CompositeValueFakePossibleFileItem(const SnapshotItem *source, IdmItem *parent = 0) :
		CompositeValuePossibleFileItem(EntityValue::Holder(), source, parent)
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		switch (role)
		{
			case Qt::EditRole:
			case Qt::DisplayRole:
				return source()->info()->fileName().as<QString>();
			case Qt::DecorationRole:
				return source()->info()->fileType()->icon();
			case Qt::TextAlignmentRole:
				return Qt::AlignLeft;
			case Qt::ToolTipRole:
				return source()->info()->fileType()->name();
		}

		return QVariant();
	}
};

static bool lessThan(CompositeValuePossibleDirItem::Container::value_type v1, CompositeValuePossibleDirItem::Container::value_type v2)
{
	if (static_cast<CompositeValuePathItem *>(v1)->isFile())
		if (static_cast<CompositeValuePathItem *>(v2)->isFile())
			return ProxyModel::compareFileNames(static_cast<CompositeValuePathItem *>(v1)->fileName(), static_cast<CompositeValuePathItem *>(v2)->fileName());
		else
			return true;
	else
		if (static_cast<CompositeValuePathItem *>(v2)->isFile())
			return false;
		else
			return ProxyModel::compareFileNames(static_cast<CompositeValuePathItem *>(v1)->fileName(), static_cast<CompositeValuePathItem *>(v2)->fileName());
}

CompositeValuePossibleDirItem::CompositeValuePossibleDirItem(const EntityValue::Holder &value, const SnapshotItem *source, IdmItem *parent) :
	CompositeValuePathItem(value, parent),
	m_source(source)
{
	const SnapshotItem *file;

	for (SnapshotItem::const_iterator i = source->begin(), end = source->end(); i != end; ++i)
	{
		file = (*i);

		if (file->info()->isFile())
			add(new CompositeValueFakePossibleFileItem(file, this));
		else
			add(new CompositeValueFakePossibleDirItem(file, this));
	}

	qSort(m_items.begin(), m_items.end(), lessThan);
}

CompositeValuePossibleDirItem::~CompositeValuePossibleDirItem()
{
	qDeleteAll(m_items);
}

CompositeValuePossibleDirItem::Base *CompositeValuePossibleDirItem::at(size_type index) const
{
	return m_items.at(index);
}

CompositeValuePossibleDirItem::size_type CompositeValuePossibleDirItem::size() const
{
	return m_items.size();
}

CompositeValuePossibleDirItem::size_type CompositeValuePossibleDirItem::indexOf(Base *item) const
{
	return m_items.indexOf(item);
}

QVariant CompositeValuePossibleDirItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::EditRole:
		case Qt::DisplayRole:
			return m_value->value();
		case Qt::DecorationRole:
			return m_source->info()->fileType()->icon();
		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;
		case Qt::ToolTipRole:
			return m_source->info()->fileType()->name();
	}

	return QVariant();
}

QString CompositeValuePossibleDirItem::fileName() const
{
	return m_source->info()->fileName();
}

bool CompositeValuePossibleDirItem::isFile() const
{
	return false;
}

void CompositeValuePossibleDirItem::open() const
{}

IDM_PLUGIN_NS_END
