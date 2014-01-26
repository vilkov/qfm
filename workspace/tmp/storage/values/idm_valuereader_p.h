/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IDM_VALUEREADER_P_H_
#define IDM_VALUEREADER_P_H_

#include <QtCore/QList>
#include <QtCore/QCoreApplication>
#include "idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class EntityValueImp : public EntityValue
{
public:
	EntityValueImp(Entity *entity, id_type id, const QVariant &value) :
		EntityValue(entity, id),
		m_value(value)
	{}

	virtual QVariant value() const { return m_value; }
	void setValue(const QVariant &value) { m_value = value; }

private:
	QVariant m_value;
};


class CompositeEntityValueImp : public CompositeEntityValue
{
	Q_DECLARE_TR_FUNCTIONS(CompositeEntityValueImp)

public:
	CompositeEntityValueImp(Entity *entity, id_type id) :
		CompositeEntityValue(entity, id)
	{}

	virtual QVariant value() const
	{
		if (m_value.isValid())
			return m_value;
		else
		{
			QString val;
			const ShortFormat &format = entity()->shortFormat();

			for (ShortFormat::size_type i = 0, size = format.size(); i < size; ++i)
				switch (format.at(i).type())
				{
					case ShortFormat::Token::Text:
					{
						val.append(format.at(i).string());
						break;
					}
					case ShortFormat::Token::Property:
					{
						Entity::size_type index = entity()->indexOf(format.at(i).string());

						if (index == Entity::InvalidIndex)
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

    virtual void resetValue()
    {
        m_value.clear();
    }

    void add(const EntityValue::Holder &value)
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

	void take(const EntityValue::Holder &value)
	{
		InternalList &list = m_items[value->entity()];
		list.remove(value->id());
		m_value.clear();
	}

	void remove(const EntityValue::Holder &value)
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

	EntityValue::Holder value(Entity *property, id_type id)
	{
		InternalList &list = m_items[property];
		InternalList::size_type index = list.indexOf(id);

		if (index == InternalList::InvalidIndex)
			return EntityValue::Holder();
		else
			return list.at(index);
	}

private:
	mutable QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUEREADER_P_H_ */
