#ifndef EMAILCONFIGOPTION_H_
#define EMAILCONFIGOPTION_H_

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class EmailConfigOption: public ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(EmailConfigOption)

public:
	EmailConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *listenerSlot);

private:
	QLineEdit *m_editor;
	QPushButton *m_button;
	QObject *m_listener;
	const char *m_listenerSlot;
};

#endif /* EMAILCONFIGOPTION_H_ */
