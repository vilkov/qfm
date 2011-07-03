#ifndef EMAILCONFIGOPTION_H_
#define EMAILCONFIGOPTION_H_

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class EmailConfigOption: public ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(EmailConfigOption)

public:
	EmailConfigOption(const qint32 &id, const QString &title, ConfigOption *parent = 0) :
		ConfigOptionValueDefault(id, title, QString::fromLatin1("your.email@domain.com"), parent),
		m_editor(0),
		m_listener(0)
	{}

	virtual bool isEnabled() const { return m_editor->isEnabled(); }
	virtual void setEnabled(bool value) { m_editor->setEnabled(value); m_button->setEnabled(value); }
	virtual QWidget *createEditor(QWidget *parent)
	{
		PScopedPointer<QWidget> widget(new QWidget(parent));
		QHBoxLayout *layout = new QHBoxLayout(widget.data());

	    layout->setSpacing(1);
	    layout->setContentsMargins(1, 1, 1, 1);
	    layout->addWidget(m_editor = new QLineEdit());
	    layout->addWidget(m_button = new QPushButton(tr("Test")));
	    m_listener->connect(m_button, SIGNAL(clicked()), m_listenerSlot);

	    return widget.take();
	}
	virtual QVariant editorValue() const { return m_editor->text(); }
	virtual void setEditorValue(const QVariant &value) { m_editor->setText(value.toString()); }
	virtual void setLoadedEditorValue(const QVariant &value) { m_editor->setText(value.toString()); }

	void setListener(QObject *listener, const char *listenerSlot) { m_listener = listener; m_listenerSlot = listenerSlot; }

private:
	QLineEdit *m_editor;
	QPushButton *m_button;
	QObject *m_listener;
	const char *m_listenerSlot;
};

#endif /* EMAILCONFIGOPTION_H_ */
