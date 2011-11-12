#ifndef IDMVALUEREADER_P_H_
#define IDMVALUEREADER_P_H_

#include <QtCore/QList>
#include <QtCore/QCoreApplication>
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
	void setValue(const QVariant &value) { m_value = value; }

private:
	QVariant m_value;
};


class IdmEntityCompositeValueImp : public IdmCompositeEntityValue
{
	Q_DECLARE_TR_FUNCTIONS(IdmEntityCompositeValueImp)

public:
	IdmEntityCompositeValueImp(IdmEntity *entity, id_type id) :
		IdmCompositeEntityValue(entity, id)
	{}

	virtual QVariant value() const
	{
		if (m_value.isValid())
			return m_value;
		else
		{
			QString val;
			const IdmShortFormat &format = entity()->shortFormat();

			for (IdmShortFormat::size_type i = 0, size = format.size(); i < size; ++i)
				switch (format.at(i).type())
				{
					case IdmShortFormat::Token::Text:
					{
						val.append(format.at(i).string());
						break;
					}
					case IdmShortFormat::Token::Property:
					{
						IdmEntity::size_type index = entity()->indexOf(format.at(i).string());

						if (index == IdmEntity::InvalidIndex)
							val.append(tr("Property \"%1\" not exists").arg(format.at(i).string()));
						else
						{
							const List values = m_items.value(entity()->at(index).entity);

							if (values.isEmpty())
								val.append(tr("Property \"%1\" is empty").arg(format.at(i).string()));
							else
								val.append(values.at(0)->value().toString());
						}

						break;
					}
				}

			return m_value = val;
		}
	}

	void add(IdmEntityValue *value)
	{
		m_items[value->entity()].push_back(value);
		m_value.clear();
	}

	void remove(IdmEntityValue *value)
	{
		List &list = m_items[value->entity()];
		delete list.takeAt(list.indexOf(value));
		m_value.clear();
	}

private:
	mutable QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_P_H_ */
