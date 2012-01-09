#include "arcarchiveentry.h"


ARC_PLUGIN_NS_BEGIN

ArchiveEntry::ArchiveEntry(const QString &fileName, Base *parent) :
	FileSystemListItem(parent),
	m_fileName(fileName)
{}

QVariant ArchiveEntry::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_fileName;
	else
		return QVariant();
}

ARC_PLUGIN_NS_END
