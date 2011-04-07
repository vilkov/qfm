#ifndef CONFIGOPTIONGROUP_H_
#define CONFIGOPTIONGROUP_H_

#include <QWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <QSpacerItem>
#include <QScopedPointer>
#include "configoption.h"
#include "configoptionvaluetemplate.h"


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
		QScopedPointer<QWidget> parent(m_container = new QWidget());
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
		QScopedPointer<ConfigOptionGroup> group(new ConfigOptionGroup(title, this));
		m_settings.push_back(group.data());
		return group.take();
	}

	template <typename UserConfigOptionClass>
	ConfigOptionValue *add(const qint32 &id, const QString &title)
	{
		QScopedPointer<UserConfigOptionClass> setting(new UserConfigOptionClass(id, title, this));
		m_settings.push_back(setting.data());
		return setting.take();
	}

	template <typename Editor>
	ConfigOptionValue *add(const qint32 &id, const QString &title, const QVariant &defaultValue)
	{
		QScopedPointer<ConfigOptionValueTemplate<Editor> > setting(new ConfigOptionValueTemplate<Editor>(id, title, defaultValue, this));
		m_settings.push_back(setting.data());
		return setting.take();
	}

protected:
	QWidget *createChildGroup(QWidget *parentWidget, const QFont &font)
	{
		QScopedPointer<QGroupBox> parent(static_cast<QGroupBox*>(m_container = new QGroupBox(title(), parentWidget)));
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

#endif /* CONFIGOPTIONGROUP_H_ */
