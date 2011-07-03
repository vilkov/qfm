#ifndef COLORCONFIGOPTION_H_
#define COLORCONFIGOPTION_H_

#include <QLabel>
#include <QColor>
#include <QPalette>
#include <QCheckBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QCoreApplication>
#include "../../tools/settings/configoptions/configoptionvalue.h"


class ColorConfigOption: public ConfigOptionValue
{
	Q_DECLARE_TR_FUNCTIONS(ColorConfigOption)

public:
	ColorConfigOption(const qint32 &id, const QString &title, ConfigOption *parent = 0) :
		ConfigOptionValue(id, title, QVariant(), parent),
		m_editor(0),
		m_listener(0)
	{}

	virtual bool isEnabled() const { return m_label->isChecked(); }
	virtual void setEnabled(bool value) { m_label->setEnabled(value); setEditorValue(value); }
	virtual QWidget *createLabel(QWidget *parent, const QFont &font)
	{
		m_label = new QCheckBox(title(), parent);
		m_label->setFont(font);

		return m_label;
	}
	virtual QWidget *createEditor(QWidget *parent)
	{
		PScopedPointer<QWidget> widget(new QWidget(parent));
		QHBoxLayout *layout = new QHBoxLayout(widget.data());

	    layout->setSpacing(1);
	    layout->setContentsMargins(1, 1, 1, 1);
	    layout->addWidget(m_editor = new QLabel(), 1);
	    m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	    m_editor->setAlignment(Qt::AlignCenter);
	    layout->addWidget(m_button = new QPushButton(tr("...")));
	    m_listener->connect(m_button, SIGNAL(clicked()), m_colorListenerSlot);
	    m_listener->connect(m_label, SIGNAL(stateChanged(int)), m_checkListenerSlot);

	    return widget.take();
	}
	virtual QVariant editorValue() const { return QVariantList() << (m_label->checkState() == Qt::Checked) << m_editor->text(); }
	virtual void setEditorValue(const QVariant &value)
	{
		QVariantList list = value.toList();

		if (list.isEmpty())
			setEditorValue(false);
		else
		{
			if (list.at(0).isValid())
				setEditorValue(list.at(0).toBool());

			if (list.at(1).isValid())
				setEditorValue(qVariantValue<QColor>(list.at(1)));
		}
	}
	virtual void setLoadedEditorValue(const QVariant &value)
	{
		QVariantList list = value.toList();

		if (list.isEmpty())
		{
			m_label->setChecked(false);
			setEditorValue(false);
			setEditorValue(QColor());
		}
		else
		{
			bool val = list.at(0).toBool();
			m_label->setChecked(val);
			setEditorValue(val);
			setEditorValue(qVariantValue<QColor>(list.at(1)));
		}
	}

	void setListener(QObject *listener, const char *checkListenerSlot, const char *colorListenerSlot) { m_listener = listener; m_checkListenerSlot = checkListenerSlot; m_colorListenerSlot = colorListenerSlot; }

private:
	inline void setEditorValue(bool value)
	{
		m_editor->setEnabled(value);
		m_button->setEnabled(value);
	}
	inline void setEditorValue(const QColor &value)
	{
		m_editor->setText(value.name());
		m_editor->setPalette(QPalette(value));
		m_editor->setAutoFillBackground(true);
	}

private:
	QCheckBox *m_label;
	QLabel *m_editor;
	QPushButton *m_button;
	QObject *m_listener;
	const char *m_checkListenerSlot;
	const char *m_colorListenerSlot;
};

#endif /* COLORCONFIGOPTION_H_ */
