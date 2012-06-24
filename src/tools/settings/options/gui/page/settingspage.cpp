#include "settingspage.h"
#include "../../../../pointers/pscopedpointer.h"

#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>


SETTINGS_NS_BEGIN

Page::Page(const QString &title, const QString &id, Option *parent) :
	Scope(id, parent),
	m_title(title)
{}

QWidget *Page::createEditor()
{
	PScopedPointer<QGroupBox> page(new QGroupBox(title()));
	QVBoxLayout *layout(new QVBoxLayout(page.data()));

	QGroupBox m_selectedPage;

	layout->setSpacing(1);
	layout->setMargin(1);

	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		layout->addWidget(m_guis.at(i)->createEditor());

	layout->addStretch(1);
	return page.take();
}

void Page::restoreDefault()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->restoreDefault();
}

bool Page::accept()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		if (!m_guis.at(i)->accept())
			return false;

	return true;
}

void Page::reject()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->reject();
}

SETTINGS_NS_END
