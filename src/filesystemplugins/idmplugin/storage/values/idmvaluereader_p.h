#ifndef IDMVALUEREADER_P_H_
#define IDMVALUEREADER_P_H_

#include <QtCore/QList>
#include "idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValueImp : public IdmEntityValue
{
public:
	IdmEntityValueImp(IdmEntity *entity, id_type id, const QVariant &value) :
		IdmEntityValue(entity, id)
	{}

	virtual QVariant value() const { return m_value; }

private:
	QVariant m_value;
};


class IdmEntityCompositeValueImp : public IdmEntityValue
{
public:
	typedef QList<IdmEntityValue*> List;

public:
	IdmEntityCompositeValueImp(IdmEntity *entity, id_type id) :
		IdmEntityValue(entity, id)
	{}
	virtual ~IdmEntityCompositeValueImp()
	{
		qDeleteAll(m_items);
	}

	virtual QVariant value() const { return QString::fromLatin1("Composite value"); }

	void add(IdmEntityValue *value) { m_items.push_back(value); }

private:
	List m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_P_H_ */
