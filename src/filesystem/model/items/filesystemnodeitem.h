#ifndef FILESYSTEMNODEITEM_H_
#define FILESYSTEMNODEITEM_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include <QtCore/QSharedData>
#include "../../filesystem_ns.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


FILE_SYSTEM_NS_BEGIN

class NodeItem : public ::Tools::Models::Tree::Item, public QSharedData
{
	Q_DISABLE_COPY(NodeItem)

public:
	class Holder : public QExplicitlySharedDataPointer<NodeItem>
	{
	public:
	    inline Holder() :
	    	QExplicitlySharedDataPointer<NodeItem>()
	    {}

	    inline explicit Holder(NodeItem *data) :
			QExplicitlySharedDataPointer<NodeItem>(data)
	    {}

	    inline Holder &operator=(NodeItem *o) { QExplicitlySharedDataPointer<NodeItem>::operator=(o); return *this; }

	    template <typename R> inline
	    R *as() const { return static_cast<R *>(data()); }
	};

public:
	NodeItem(Base *parent);

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

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMNODEITEM_H_ */
