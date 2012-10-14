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
#ifndef CONFIGOPTIONGROUP_H_
#define CONFIGOPTIONGROUP_H_

#include <QtGui/QWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include "configoption.h"
#include "configoptionvaluetemplate.h"


SETTINGS_NS_BEGIN

class ConfigOptionGroup : public ConfigOption
{
public:
	typedef QList<ConfigOption*>  value_type;
	typedef value_type::size_type size_type;

public:
	ConfigOptionGroup(const QString &title, ConfigOption *parent = 0) :
		ConfigOption(title, parent),
		m_container(0)
	{}
	virtual ~ConfigOptionGroup() { qDeleteAll(m_settings); }

	virtual Type type() const { return Group; }
	virtual bool isEnabled() const { return m_container->isEnabled(); }
	virtual void setEnabled(bool value) { m_container->setEnabled(value); }
	virtual QWidget *createGroup(const QFont &font)
	{
		PScopedPointer<QWidget> parent(m_container = new QWidget());
		QGridLayout *layout = new QGridLayout(parent.data());

		for (size_type i = 0, size = m_settings.size(); i < size; ++i)
			if (m_settings.at(i)->type() == ConfigOption::Value)
			{
				layout->addWidget(static_cast<ConfigOptionValue*>(m_settings.at(i))->createLabel(parent.data(), font), i, 0, 1, 1);
				layout->addWidget(static_cast<ConfigOptionValue*>(m_settings.at(i))->createEditor(parent.data()), i, 1, 1, 1);
			}
			else
				layout->addWidget(static_cast<ConfigOptionGroup*>(m_settings.at(i))->createChildGroup(parent.data(), font), i, 0, 1, 2);

		layout->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Expanding), m_settings.size(), 1, 2);

		return parent.take();
	}

	size_type size() const { return m_settings.size(); }
	ConfigOption *at(const size_type &index) const { return m_settings.at(index); }

	ConfigOptionGroup *add(const QString &title)
	{
		PScopedPointer<ConfigOptionGroup> group(new ConfigOptionGroup(title, this));
		m_settings.push_back(group.data());
		return group.take();
	}

	template <typename UserConfigOptionClass>
	ConfigOptionValue *add(const qint32 &id, const QString &title)
	{
		PScopedPointer<UserConfigOptionClass> setting(new UserConfigOptionClass(id, title, this));
		m_settings.push_back(setting.data());
		return setting.take();
	}

	template <typename Editor>
	ConfigOptionValue *add(const qint32 &id, const QString &title, const QVariant &defaultValue)
	{
		PScopedPointer<ConfigOptionValueTemplate<Editor> > setting(new ConfigOptionValueTemplate<Editor>(id, title, defaultValue, this));
		m_settings.push_back(setting.data());
		return setting.take();
	}

protected:
	QWidget *createChildGroup(QWidget *parentWidget, const QFont &font)
	{
		PScopedPointer<QGroupBox> parent(static_cast<QGroupBox*>(m_container = new QGroupBox(title(), parentWidget)));
		QGridLayout *layout = new QGridLayout(parent.data());

		for (size_type i = 0, size = m_settings.size(); i < size; ++i)
			if (m_settings.at(i)->type() == ConfigOption::Value)
			{
				layout->addWidget(static_cast<ConfigOptionValue*>(m_settings.at(i))->createLabel(parent.data(), font), i, 0, 1, 1);
				layout->addWidget(static_cast<ConfigOptionValue*>(m_settings.at(i))->createEditor(parent.data()), i, 1, 1, 1);
			}
			else
				layout->addWidget(static_cast<ConfigOptionGroup*>(m_settings.at(i))->createChildGroup(parent.data(), font), i, 0, 1, 2);

		return parent.take();
	}

private:
	QWidget *m_container;
	value_type m_settings;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONGROUP_H_ */
