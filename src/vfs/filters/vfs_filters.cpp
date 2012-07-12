#include "vfs_filters.h"


VFS_NS_BEGIN

Filters::~Filters()
{
	qDeleteAll(m_filters);
}

bool Filters::match(const IFileInfo *info) const
{
	for (Container::const_iterator i = m_filters.constBegin(), end = m_filters.constEnd(); i != end; ++i)
		if (!(*i)->match(info))
			return false;

	return true;
}

bool FileNameFilter::match(const IFileInfo *info) const
{
	return m_pattern.exactMatch(info->fileName());
}

bool ModificationTimeFilterRange::match(const IFileInfo *info) const
{
	return DateFilterRange::match(info->lastModified());
}

VFS_NS_END
