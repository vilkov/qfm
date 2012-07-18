#ifndef VFS_ITEM_H_
#define VFS_ITEM_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtCore/QSharedData>
#include "../../vfs_ns.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


VFS_NS_BEGIN

class Item : public ::Tools::Models::Tree::Item, public QSharedData
{
	Q_DISABLE_COPY(Item)

public:
	class Holder : public QExplicitlySharedDataPointer<Item>
	{
	public:
	    inline Holder() :
	    	QExplicitlySharedDataPointer<Item>()
	    {}

	    inline explicit Holder(Item *data) :
			QExplicitlySharedDataPointer<Item>(data)
	    {}

	    inline Holder &operator=(Item *o) { QExplicitlySharedDataPointer<Item>::operator=(o); return *this; }

	    template <typename R> inline
	    R *as() const { return static_cast<R *>(data()); }
	};

public:
	Item(Base *parent);

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_reason; }
	const QIcon &lockIcon() const { return m_icon; }

	void lock(const QString &reason);
	void cancel(const QString &reason);
    void unlock() { m_locked = false; m_reason.clear(); m_icon = QIcon(); }

private:
	bool m_locked;
	QString m_reason;
	QIcon m_icon;
};

VFS_NS_END

#endif /* VFS_ITEM_H_ */
