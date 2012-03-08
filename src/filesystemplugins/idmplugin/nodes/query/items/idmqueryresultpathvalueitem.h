#ifndef IDMQUERYRESULTPATHVALUEITEM_H_
#define IDMQUERYRESULTPATHVALUEITEM_H_

#include <QtCore/QCoreApplication>
#include "idmqueryresultvalueitem.h"
#include "../../../../../filesystem/filesystemnode.h"
#include "../../../../../filesystem/info/filesysteminfo.h"
#include "../../../../../filesystem/interfaces/filesystemifilecontainer.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathValueItem : public QueryResultValueItem, public IFileInfo
{
	Q_DECLARE_TR_FUNCTIONS(QueryResultPathValueItem)

public:
	QueryResultPathValueItem(const IFileContainer *container, const IdmEntityValue::Holder &value, Base *parent);

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual IFileInfo::size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void update(const Info &info) { m_info = info; }

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

private:
	Info m_info;
	Node *m_node;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHVALUEITEM_H_ */
