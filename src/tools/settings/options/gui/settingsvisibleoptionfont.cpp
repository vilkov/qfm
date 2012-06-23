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

QLayout *VisibleOptioinFont::createEditor()
{
	typedef Events::MouseReleaseEventSource<Events::EventSourceBase<QPushButton> > Button;
	PScopedPointer<QHBoxLayout> res(new QHBoxLayout());

    res->setSpacing(3);
    res->setMargin(1);

    res->addWidget(new QLabel(m_label));

	res->addWidget(m_editor = new QLabel(toString(m_newFont = OptionFont::font())), 1);
	m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	m_editor->setAlignment(Qt::AlignCenter);
	m_editor->setFont(m_newFont);

	res->addWidget(new Button(&m_eventHandler, QString::fromLatin1("...")));

	return res.take();
}

bool VisibleOptioinFont::accept()
{
	setFont(m_newFont);
	return true;
}

void VisibleOptioinFont::reject()
{}

void VisibleOptioinFont::chooseFontEvent()
{
	if (chooseFont(m_newFont))
	{
		m_editor->setText(toString(m_newFont));
		m_editor->setFont(m_newFont);
	}
}

SETTINGS_NS_END
