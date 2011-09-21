#ifndef IDMVALUEREADER_P_H_
#define IDMVALUEREADER_P_H_

#include <QtCore/QList>
#include "idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValueImp : public IdmEntityValue
{
public:
	IdmEntityValueImp(IdmEntity *entity, id_type id, const QVariant &value) :
		IdmEntityValue(entity, id),
		m_value(value)
	{}

	virtual QVariant value() const { return m_value; }

private:
	QVariant m_value;
};


class IdmEntityCompositeValueImp : public IdmEntityValue
{
public:
	typedef QList<IdmEntityValue*> List;
	typedef QMap<IdmEntity*, List> Map;

public:
	IdmEntityCompositeValueImp(IdmEntity *entity, id_type id) :
		IdmEntityValue(entity, id)
	{}
	virtual ~IdmEntityCompositeValueImp()
	{
		for (Map::iterator it = m_items.begin(), end = m_items.end(); it != end; ++it)
			qDeleteAll(it.value());
	}

	virtual QVariant value() const { return QString::fromLatin1("Composite value"); }

	void add(IdmEntityValue *value) { m_items[value->entity()].push_back(value); }

private:
	Map m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_P_H_ */
