#ifndef IDMVALUEREADER_H_
#define IDMVALUEREADER_H_

#include "../queries/idmselectquery.h"
#include "../../containeres/idmcontainer.h"
#include "idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmValueReader
{
public:
	IdmValueReader(const IdmContainer &container, const Select &query);

	bool isValid() const { return m_context.isValid(); }
	IdmEntity *entity() const { return m_context.entity(); }
	const QString &lastError() const { return m_lastError; }

	IdmEntityValue::Holder next() const;
	bool eof() const { return m_afterLast; }
	bool bof() const { return m_beforeFirst; }

	static void addValue(const IdmEntityValue::Holder &value, const IdmEntityValue::Holder &property);
	static void addValue(const IdmEntityValue::Holder &value, const IdmCompositeEntityValue::List &values);
	static void takeValue(const IdmEntityValue::Holder &value, const IdmEntityValue::Holder &property);
	static void updateValue(const IdmEntityValue::Holder &value, const QVariant &newValue);
	static void removeValue(const IdmEntityValue::Holder &value, const IdmEntityValue::Holder &property);
	static void removeValue(const IdmEntityValue::Holder &value, const IdmCompositeEntityValue::List &values);
	static IdmEntityValue::Holder createValue(IdmEntity *entity, IdmEntityValue::id_type id);
	static IdmEntityValue::Holder createValue(IdmEntity *entity, IdmEntityValue::id_type id, const QVariant &value);

private:
	IdmEntityValue::Holder doNext() const;
	IdmEntityValue::Holder value(IdmEntity *entity, IdmEntity::id_type id, int column) const;
	void property(const IdmEntityValue::Holder &value, IdmEntity *property, int &column) const;
	void skip(IdmEntity *property, int &column) const;

private:
	QString m_lastError;
	QueryContext m_context;
	mutable bool m_afterLast;
	mutable bool m_beforeFirst;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_H_ */
