#ifndef IDMSELECTQUERY_H_
#define IDMSELECTQUERY_H_

#include <QtCore/QMap>
#include "idmquery.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	enum Operators
	{
		Less,
		Greater,
		Equal,
		Like
	};
	struct Condition
	{
		Condition()
		{}
		Condition(Operators op, const QVariant &value) :
			op(op),
			value(value)
		{}

		Operators op;
		QVariant value;
	};
	typedef QMap<IdmEntity*, Condition> Map;

public:
	Select(IdmEntity *entity);

	virtual QByteArray compile() const;

private:
	Map m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDMSELECTQUERY_H_ */
