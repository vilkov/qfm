#ifndef CHECKCONFIGOPTION_H_
#define CHECKCONFIGOPTION_H_

#include <QList>
#include <QCheckBox>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class CheckConfigOption: public ConfigOptionValueDefault
{
public:
	CheckConfigOption(qint32 id, const QString &title, ConfigOption *parent);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *listenerSlot);
	void addOption(ConfigOption *option);
	void removeOption(ConfigOption *option);

private:
	inline void setEditorValue(bool value);

private:
	QCheckBox *m_editor;
	QObject *m_listener;
	const char *m_listenerSlot;
	QList<ConfigOption*> m_options;
};

#endif /* CHECKCONFIGOPTION_H_ */
