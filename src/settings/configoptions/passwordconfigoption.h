#ifndef PASSWORDCONFIGOPTION_H_
#define PASSWORDCONFIGOPTION_H_

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class PasswordConfigOption: public ::Tools::Settings::ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(PasswordConfigOption)

public:
	PasswordConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

private:
	QLineEdit *m_editor;
};

#endif /* PASSWORDCONFIGOPTION_H_ */
