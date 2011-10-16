#ifndef M3UCONTAINER_H_
#define M3UCONTAINER_H_

#include <QtCore/QList>
#include "../items/m3uitem.h"
#include "../../../tools/models/tree/treemodelcontainer.h"


M3U_PLUGIN_NS_BEGIN

class M3uContainer : public ::Tools::Models::Tree::Container
{
public:
	typedef QList<M3uItem*> Container;

public:
	M3uContainer();
	virtual ~M3uContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

	const Container &container() const { return m_container; }
	Container &container() { return m_container; }

private:
	Container m_container;
};

M3U_PLUGIN_NS_END

#endif /* M3UCONTAINER_H_ */
