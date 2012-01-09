#include "arcarchive.h"
#include <string.h>
#include <archive.h>
#include <archive_entry.h>
#include <QtCore/QMap>


ARC_PLUGIN_NS_BEGIN

Archive::Contents::Contents() :
	extractedSize(0)
{}

Archive::Contents Archive::read(const QString &fileName)
{
	Contents contents;

	if (struct archive *a = archive_read_new())
	{
		if (archive_read_support_compression_all(a) == ARCHIVE_OK &&
			archive_read_support_format_all(a) == ARCHIVE_OK &&
			archive_read_open_filename(a, fileName.toUtf8(), 10240) == ARCHIVE_OK)
		{
		    QMap<QString, ArchiveEntry *> parents;
		    ArchiveEntry::size_type index;
		    struct archive_entry *e;
		    ArchiveEntry *entry;
		    ArchiveEntry *parent;
		    QString fileName;
		    const char *path;
		    char *sep;
		    int res;

		    while ((res = archive_read_next_header(a, &e)) == ARCHIVE_OK)
		    {
		    	contents.extractedSize += archive_entry_size(e);

		    	path = archive_entry_pathname(e);

		    	if ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
		    	{
		    		(*sep) = 0;
		    		ArchiveEntry *&p = parents[fileName = QString::fromUtf8(path)];

		    		if (p == NULL)
		    		{
		    			p = parent = new ArchiveEntry(fileName);
		    			contents.items.push_back(parent);
		    		}
		    		else
		    			parent = p;

		    		path = (++sep);

		    		while ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
		    		{
			    		(*sep) = 0;

			    		if ((index = parent->indexOf(fileName = QString::fromUtf8(path))) == ArchiveEntry::InvalidIndex)
			    		{
			    			parent->add(entry = new ArchiveEntry(fileName, parent));
			    			parent = entry;
			    		}
			    		else
			    			parent = static_cast<ArchiveEntry *>(parent->at(index));

			    		path = (++sep);
		    		}

		    		if (!(fileName = QString::fromUtf8(path)).isEmpty())
		    			parent->add(new ArchiveEntry(fileName, parent));
		    	}
		    	else
		    		contents.items.push_back(new ArchiveEntry(QString::fromUtf8(path)));

		        archive_read_data_skip(a);
		    }
		}

		archive_read_close(a);
		archive_read_finish(a);
	}

	return contents;
}

ARC_PLUGIN_NS_END
