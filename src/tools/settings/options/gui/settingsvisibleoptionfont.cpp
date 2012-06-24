#include "settingsvisibleoptionfont.h"
#include "../../../pointers/pscopedpointer.h"
#include "../../../events/imp/mouseeventsource.h"

#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>


SETTINGS_NS_BEGIN

VisibleOptioinFont::VisibleOptioinFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue) :
	OptionFont(id, parent, defaultValue),
	m_eventHandler(this),
	m_label(label)
{
	m_eventHandler.registerMouseReleaseEventHandler(&VisibleOptioinFont::chooseFontEvent);
}

QWidget *VisibleOptioinFont::createEditor()
{
	typedef Events::MouseReleaseEventSource<Events::EventSourceBase<QPushButton> > Button;
	PScopedPointer<QWidget> page(new QWidget());
	QHBoxLayout *layout(new QHBoxLayout(page.data()));

	layout->setSpacing(3);
	layout->setMargin(1);

	layout->addWidget(new QLabel(m_label));

	layout->addWidget(m_editor = new QLabel(toString(m_currentFont = OptionFont::font())), 1);
	m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	m_editor->setAlignment(Qt::AlignCenter);
	m_editor->setFont(m_currentFont);

	layout->addWidget(new Button(&m_eventHandler, QString::fromLatin1("...")));

	return page.take();
}

void VisibleOptioinFont::restoreDefault()
{
	m_editor->setText(toString(m_currentFont = OptionFont::defaultValue()));
	m_editor->setFont(m_currentFont);
}

bool VisibleOptioinFont::accept()
{
	if (m_currentFont != font())
	{
		setFont(m_currentFont);
		applyFont(m_currentFont);
	}

	return true;
}

void VisibleOptioinFont::reject()
{}

void VisibleOptioinFont::chooseFontEvent()
{
	if (chooseFont(m_currentFont))
	{
		m_editor->setText(toString(m_currentFont));
		m_editor->setFont(m_currentFont);
	}
}

SETTINGS_NS_END
