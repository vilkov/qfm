#ifndef PASSWORDCONFIGOPTION_H_
#define PASSWORDCONFIGOPTION_H_

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class PasswordConfigOption: public ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(PasswordConfigOption)

public:
	PasswordConfigOption(const qint32 &id, const QString &title, ConfigOption *parent = 0) :
		ConfigOptionValueDefault(id, title, QVariant(), parent),
		m_editor(0)
	{}

	virtual bool isEnabled() const { return m_editor->isEnabled(); }
	virtual void setEnabled(bool value) { m_editor->setEnabled(value); }
	virtual QWidget *createEditor(QWidget *parent)
	{
		m_editor = new QLineEdit(parent);
		static_cast<QLineEdit*>(m_editor)->setEchoMode(QLineEdit::Password);

	    return m_editor;
	}
	virtual QVariant editorValue() const { return m_editor->text(); }
	virtual void setEditorValue(const QVariant &value) { m_editor->setText(value.toString()); }
	virtual void setLoadedEditorValue(const QVariant &value) { m_editor->setText(value.toString()); }

private:
	QLineEdit *m_editor;
};

#endif /* PASSWORDCONFIGOPTION_H_ */
