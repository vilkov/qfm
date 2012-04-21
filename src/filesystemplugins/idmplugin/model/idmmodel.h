#ifndef IDMMODEL_H_
#define IDMMODEL_H_

#include "idmmodelcontainer.h"
#include "../../../tools/models/tree/treemodel.h"


IDM_PLUGIN_NS_BEGIN

class IdmModel : public ::Tools::Models::Tree::Model
{
public:
	typedef IdmModelContainer::Container Container;

public:
	IdmModel(QObject *parent = 0);

protected:
	IdmModelContainer m_container;
	Container &m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMMODEL_H_ */
