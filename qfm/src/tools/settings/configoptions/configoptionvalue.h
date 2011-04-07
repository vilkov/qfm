#ifndef CONFIGOPTIONVALUE_H_
#define CONFIGOPTIONVALUE_H_

#include <QList>
#include <QFont>
#include <QWidget>
#include "configoption.h"


class ConfigOptionValue: public ConfigOption
{
public:
	ConfigOptionValue(const qint32 &id, const QString &title, const QVariant &defaultValue, ConfigOption *parent = 0) :
		ConfigOption(title, parent),
		m_id(id),
		m_defaultValue(defaultValue)
	{}

	virtual Type type() const { return Value; }
	virtual QWidget *createLabel(QWidget *parent, const QFont &font) = 0;
	virtual QWidget *createEditor(QWidget *parent) = 0;
	virtual QVariant editorValue() const = 0;
	virtual void setEditorValue(const QVariant &value) = 0;
	virtual void setLoadedEditorValue(const QVariant &value) = 0;

	const qint32 &id() const { return m_id; }
	QVariant defaultValue() const { return m_defaultValue; }

private:
	qint32 m_id;
	QVariant m_defaultValue;
};

#endif /* CONFIGOPTIONVALUE_H_ */
