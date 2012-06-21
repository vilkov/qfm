#include "settingspage.h"
#include "../../../../pointers/pscopedpointer.h"

#include <QtGui/QVBoxLayout>


SETTINGS_NS_BEGIN

Page::Page(const QString &title, const QString &id, Option *parent) :
	Scope(id, parent),
	m_title(title)
{}

QLayout *Page::createEditor()
{
	PScopedPointer<QVBoxLayout> res(new QVBoxLayout());

	res->setSpacing(1);
	res->setMargin(1);

	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		res->addLayout(m_guis.at(i)->createEditor());

	res->addStretch(1);
	return res.take();
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
