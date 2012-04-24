#ifndef FILESYSTEMFOLDERITEM_H_
#define FILESYSTEMFOLDERITEM_H_

#include "../base/items/filesystemfolderbaseitem.h"


FILE_SYSTEM_NS_BEGIN

class FolderItem : public FolderBaseItem
{
public:
	explicit FolderItem(const Info &info, Base *parent = 0);
	explicit FolderItem(const Info &info, Node *node, Base *parent = 0);

	void setNode(Node *node) { FolderBaseItem::setNode(node); }

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* FileSystemBaseItem */
	virtual bool isRootItem() const;

	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

public:
	const QVariant &totalSize() const { return m_totalSize; }
	void setTotalSize(quint64 value) { m_totalSize = value; }
	void clearTotalSize() { m_totalSize.clear(); }

	void updateProgress(quint64 value, quint64 timeElapsed) { progress(value, timeElapsed); }

private:
	QVariant m_totalSize;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERITEM_H_ */
