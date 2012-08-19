#include "settingsgroupbox.h"
#include "../../../../pointers/pscopedpointer.h"

#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>


SETTINGS_NS_BEGIN

GroupBox::GroupBox(const QString &title, const QString &id, Option *parent) :
	Scope(id, parent),
	m_title(title)
{}

QWidget *GroupBox::createEditor()
{
	PScopedPointer<QGroupBox> page(new QGroupBox(title()));
	QVBoxLayout *layout(new QVBoxLayout(page.data()));

	QGroupBox m_selectedPage;

	layout->setSpacing(5);
	layout->setMargin(5);

	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		layout->addWidget(m_guis.at(i)->createEditor());

	layout->addStretch(1);
	return page.take();
}

void GroupBox::restoreDefault()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->restoreDefault();
}

bool GroupBox::accept()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		if (!m_guis.at(i)->accept())
			return false;

	return true;
}

void GroupBox::reject()
{
	for (Container::size_type i = 0, size = m_guis.size(); i < size; ++i)
		m_guis.at(i)->reject();
}

SETTINGS_NS_END
