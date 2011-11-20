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

	IdmEntityValue *next() const;
	bool eof() const { return m_afterLast; }
	bool bof() const { return m_beforeFirst; }

	static void addValue(IdmEntityValue *value, IdmEntityValue *property);
	static void takeValue(IdmEntityValue *value, IdmEntityValue *property);
	static void updateValue(IdmEntityValue *value, const QVariant &newValue);
	static void removeValue(IdmEntityValue *value, IdmEntityValue *property);
	static IdmCompositeEntityValue *createValue(IdmEntity *entity, IdmEntityValue::id_type id);
	static IdmEntityValue *createValue(IdmEntity *entity, IdmEntityValue::id_type id, const QVariant &value);

private:
	IdmEntityValue *doNext() const;
	IdmEntityValue *value(IdmEntity *entity, IdmEntity::id_type id, int column) const;
	void property(IdmEntityValue *value, IdmEntity *property, int &column) const;
	void skip(IdmEntity *property, int &column) const;

private:
	QString m_lastError;
	QueryContext m_context;
	mutable bool m_afterLast;
	mutable bool m_beforeFirst;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_H_ */
