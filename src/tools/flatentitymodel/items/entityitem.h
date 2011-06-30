#ifndef ENTITYITEM_H_
#define ENTITYITEM_H_

#include <QList>
#include <QVariant>


class EntityItem
{
public:
    typedef QList<EntityItem*>    value_type;
	typedef value_type::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	virtual ~EntityItem() {}

	virtual EntityItem *parent() const { return 0; }
	virtual EntityItem *child(size_type index) const { return 0; }
	virtual size_type size() const { return 0; }
	virtual size_type indexOf(EntityItem *item) const { return InvalidIndex; }
	virtual QVariant data(qint32 column, qint32 role) const = 0;
};

#endif /* ENTITYITEM_H_ */
