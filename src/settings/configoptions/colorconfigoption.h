#ifndef COLORCONFIGOPTION_H_
#define COLORCONFIGOPTION_H_

#include <QtGui/QLabel>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvalue.h"


class ColorConfigOption: public ::Tools::Settings::ConfigOptionValue
{
	Q_DECLARE_TR_FUNCTIONS(ColorConfigOption)

public:
	ColorConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createLabel(QWidget *parent, const QFont &font);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *checkListenerSlot, const char *colorListenerSlot);

private:
	inline void setEditorValue(bool value);
	inline void setEditorValue(const QColor &value);

private:
	QCheckBox *m_label;
	QLabel *m_editor;
	QPushButton *m_button;
	QObject *m_listener;
	const char *m_checkListenerSlot;
	const char *m_colorListenerSlot;
};

#endif /* COLORCONFIGOPTION_H_ */
