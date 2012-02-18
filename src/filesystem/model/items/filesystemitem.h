#ifndef FILESYSTEMLISTITEM_ITEM_H_
#define FILESYSTEMLISTITEM_ITEM_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include "../../filesystem_ns.h"
#include "../../../tools/models/tree/items/treemodelitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public ::Tools::Models::Tree::Item
{
public:
	FileSystemItem(Base *parent);

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

#endif /* FILESYSTEMLISTITEM_ITEM_H_ */
