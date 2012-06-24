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

	layout->addWidget(m_editor = new QLabel(toString(m_newFont = OptionFont::font())), 1);
	m_editor->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	m_editor->setAlignment(Qt::AlignCenter);
	m_editor->setFont(m_newFont);

	layout->addWidget(new Button(&m_eventHandler, QString::fromLatin1("...")));

	return page.take();
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
