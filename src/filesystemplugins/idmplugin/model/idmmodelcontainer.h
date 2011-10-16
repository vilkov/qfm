#ifndef IDMMODELCONTAINER_H_
#define IDMMODELCONTAINER_H_

#include <QtCore/QList>
#include "items/idmitem.h"
#include "../../../tools/models/tree/treemodelcontainer.h"


IDM_PLUGIN_NS_BEGIN

class IdmModelContainer : public ::Tools::Models::Tree::Container
{
public:
	typedef QList<IdmItem*> Container;

public:
	IdmModelContainer();
	virtual ~IdmModelContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

	const Container &container() const { return m_container; }
	Container &container() { return m_container; }

private:
	friend class IdmModel;
	Container m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMMODELCONTAINER_H_ */
