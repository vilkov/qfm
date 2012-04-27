#ifndef IDMMODEL_H_
#define IDMMODEL_H_

#include "items/idmitem.h"
#include "../../../tools/models/tree/treemodel.h"


IDM_PLUGIN_NS_BEGIN

class IdmModel : public ::Tools::Models::Tree::Model
{
public:
	IdmModel(QObject *parent = 0);

protected:
	class Container : public ::Tools::Models::Tree::Model::Container
	{
	public:
		typedef QList<IdmItem *> List;

	public:
		Container();
		virtual ~Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class IdmModel;
		friend class IdmRootNode;
		friend class IdmNodeQueryResults;
		List m_container;
	};

protected:
	Container m_container;
	Container::List &m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMMODEL_H_ */
