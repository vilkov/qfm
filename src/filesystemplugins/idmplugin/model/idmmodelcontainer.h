#ifndef IDMMODELCONTAINER_H_
#define IDMMODELCONTAINER_H_

#include <QtCore/QList>
#include "items/idmitem.h"
#include "../../../filesystem/model/filesystemmodelcontainer.h"


IDM_PLUGIN_NS_BEGIN

class IdmModelContainer : public ModelContainer
{
public:
	typedef QList<IdmItem*> Container;

public:
	IdmModelContainer();
	virtual ~IdmModelContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

private:
	friend class IdmModel;
	friend class IdmNodeBase;
	friend class IdmNodeQueryResults;
	Container m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMMODELCONTAINER_H_ */
