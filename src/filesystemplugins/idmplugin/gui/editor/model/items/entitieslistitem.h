#ifndef ARTIFACTSVIRTUALITEM_H_
#define ARTIFACTSVIRTUALITEM_H_

#include <QVariant>
#include <QList>
#include <QString>
#include "artifactsitem.h"
#include "artifactscolumn.h"


class ArtifactsVirtualItem : public ArtifactsItem
{
	Q_DISABLE_COPY(ArtifactsVirtualItem)

public:
	ArtifactsVirtualItem(const qint32 &id, const QList<ArtifactsColumn> &columns, ModelItem *parent = 0);

	virtual int columnCount() const { return m_columns.size(); }
	virtual QVariant data(int column, int role) const;
	virtual Qt::ItemFlags flags(int column) const;
	virtual bool isVirtual() const { return true; }
	const ArtifactsColumn &column(int column) const { return m_columns[column]; }

private:
	QString columnData(const int &column) const;

private:
	QList<ArtifactsColumn> m_columns;
};

#endif /* ARTIFACTSVIRTUALITEM_H_ */
