#include "settingsvisiblecontainer.h"


SETTINGS_NS_BEGIN

VisibleContainer::VisibleContainer(const QString &storage) :
	Container(storage)
{}

void VisibleContainer::manage(Page *option)
{
	Container::manage(option);
	m_guis.push_back(option);
}

SETTINGS_NS_END
