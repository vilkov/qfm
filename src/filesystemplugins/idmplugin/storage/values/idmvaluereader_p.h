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
							val.append(tr("Property \"%1\" does not exists").arg(format.at(i).string()));
						else
						{
							const InternalList values = m_items.value(entity()->at(index).entity);

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

	void add(const IdmEntityValue::Holder &value)
	{
		m_items[value->entity()].add(value->id(), value);
		m_value.clear();
	}

	void add(const List &values)
	{
		InternalList &list = m_items[values.at(0)->entity()];

		for (List::size_type i = 0, size = values.size(); i < size; ++i)
			list.add(values.at(i)->id(), values.at(i));

		m_value.clear();
	}

	void take(const IdmEntityValue::Holder &value)
	{
		InternalList &list = m_items[value->entity()];
		list.remove(value->id());
		m_value.clear();
	}

	void remove(const IdmEntityValue::Holder &value)
	{
		InternalList &list = m_items[value->entity()];
		list.take(value->id());
		m_value.clear();
	}

	void remove(const List &values)
	{
		InternalList &list = m_items[values.at(0)->entity()];

		for (List::size_type i = 0, size = values.size(); i < size; ++i)
			list.take(values.at(i)->id());

		m_value.clear();
	}

	IdmEntityValue::Holder value(IdmEntity *property, id_type id)
	{
		InternalList &list = m_items[property];
		InternalList::size_type index = list.indexOf(id);

		if (index == InternalList::InvalidIndex)
			return IdmEntityValue::Holder();
		else
			return list.at(index);
	}

private:
	mutable QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMVALUEREADER_P_H_ */
