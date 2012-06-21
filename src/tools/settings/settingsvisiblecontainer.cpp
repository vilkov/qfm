#include "settingsvisiblecontainer.h"
#include "dialog/settingsdialog.h"


SETTINGS_NS_BEGIN

VisibleContainer::VisibleContainer(const QString &storage) :
	Container(storage)
{}

void VisibleContainer::manage(Page *option)
{
	Container::manage(option);
	m_pages.push_back(option);
}

void VisibleContainer::exec(const QString &title, DialogSettings &settings, QWidget *parent)
{
	Q_ASSERT(!m_pages.isEmpty());
	Dialog(title, settings, m_pages, parent).exec();
}

SETTINGS_NS_END
