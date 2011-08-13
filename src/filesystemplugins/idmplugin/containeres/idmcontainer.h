#ifndef IDMCONTAINER_H_
#define IDMCONTAINER_H_

#include <QtCore/QCoreApplication>
#include "menu/idmmenu.h"
#include "../storage/idmstorage.h"


FILE_SYSTEM_NS_BEGIN

class IdmContainer
{
	Q_DECLARE_TR_FUNCTIONS(IdmContainer)

public:
	typedef IdmMenu::value_type   value_type;
	typedef value_type::size_type size_type;

	enum MenuId
	{
		Create,
		Remove
	};

public:
	IdmContainer(const QFileInfo &storage);

	operator const IdmItem * () const { return &m_menu; }
	operator IdmItem * () { return &m_menu; }

	bool isValid() const { return m_storage.isValid(); }
	const QString &lastError() const { return m_storage.lastError(); }

	IdmItem *at(size_type index) const { return m_menu.at(index); }
	size_type size() const { return m_menu.size(); }
	size_type indexOf(IdmItem *item) const { return m_menu.indexOf(item); }

private:
	IdmMenu m_menu;
	IdmStorage m_storage;
};

FILE_SYSTEM_NS_END

#endif /* IDMCONTAINER_H_ */
