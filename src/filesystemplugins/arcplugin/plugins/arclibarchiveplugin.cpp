#include "arclibarchiveplugin.h"
#include "../nodes/items/arcnodeentryitem.h"
#include "../nodes/items/arcnodedirentryitem.h"
#include "../../../tools/pointers/pscopedpointer.h"
#include <string.h>
#include <archive.h>
#include <archive_entry.h>
#include <QtCore/QMap>


ARC_PLUGIN_NS_BEGIN

struct LibArchiveState : public LibArchivePlugin::State
{
	LibArchiveState() :
		a(NULL)
	{}

    struct archive *a;
};


LibArchivePlugin::LibArchivePlugin()
{}

LibArchivePlugin::State *LibArchivePlugin::beginRead(const QString &fileName) const
{
	PScopedPointer<LibArchiveState> state(new LibArchiveState());

	if (state->a = archive_read_new())
	{
		if (archive_read_support_compression_all(state->a) != ARCHIVE_OK ||
			archive_read_support_format_all(state->a) != ARCHIVE_OK ||
			archive_read_open_filename(state->a, fileName.toUtf8(), 10240) != ARCHIVE_OK)
		{
			state->error = QString::fromUtf8(archive_error_string(state->a));
		}
	}
	else
		state->error = tr("Initialization of libarchive has failed");

	return state.take();
}

LibArchivePlugin::Contents LibArchivePlugin::readAll(State *s, const volatile Flags &aborted) const
{
	Q_ASSERT(s->error.isEmpty());
	Contents contents;
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
    QMap<QString, ArcNodeDirEntryItem *> parents;
    ArcNodeDirEntryItem::size_type index;
    struct archive_entry *e;
    ArcNodeDirEntryItem *entry;
    ArcNodeDirEntryItem *parent;
    QString fileName;
    const char *path;
    char *sep;
    int res;

    while ((res = archive_read_next_header(state->a, &e)) == ARCHIVE_OK && !aborted)
    {
    	contents.extractedSize += archive_entry_size(e);

    	path = archive_entry_pathname(e);

    	if ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
    	{
    		(*sep) = 0;
    		ArcNodeDirEntryItem *&p = parents[fileName = QString::fromUtf8(path)];

    		if (p == NULL)
    		{
    			p = parent = new ArcNodeDirEntryItem(fileName, QDateTime::fromTime_t(archive_entry_mtime(e)));
    			contents.items.push_back(parent);
    		}
    		else
    			parent = p;

    		path = (++sep);

    		while ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
    		{
	    		(*sep) = 0;

	    		if ((index = parent->indexOf(fileName = QString::fromUtf8(path))) == ArcNodeDirEntryItem::InvalidIndex)
	    		{
	    			parent->add(entry = new ArcNodeDirEntryItem(fileName, QDateTime::fromTime_t(archive_entry_mtime(e)), parent));
	    			parent = entry;
	    		}
	    		else
	    			parent = static_cast<ArcNodeDirEntryItem *>(parent->at(index));

	    		path = (++sep);
    		}

    		if (!(fileName = QString::fromUtf8(path)).isEmpty())
    			parent->add(new ArcNodeEntryItem(fileName, archive_entry_size(e), QDateTime::fromTime_t(archive_entry_mtime(e)), parent));
    	}
    	else
    		contents.items.push_back(new ArcNodeEntryItem(QString::fromUtf8(path), archive_entry_size(e), QDateTime::fromTime_t(archive_entry_mtime(e))));

        archive_read_data_skip(state->a);
    }

    if (res != ARCHIVE_EOF)
		state->error = QString::fromUtf8(archive_error_string(state->a));

    return contents;
}

void LibArchivePlugin::extract(State *s, const ArcNodeItem::Base *entry, const IFileControl *dest, IFile::value_type *buffer, IFile::size_type bufferSize, const volatile Flags &aborted) const
{
	Q_ASSERT(s->error.isEmpty());
	LibArchiveState *state = static_cast<LibArchiveState *>(s);

	if (entry->isList())
	{

	}
	else
	{
		PScopedPointer<IFileControl> destFile;
		QString entryPath = static_cast<const ArcNodeEntryItem *>(entry)->fileName();

		if (destFile = dest->openFile(entryPath, state->error))
		{
			PScopedPointer<IFile> file;

			if (file = destFile->file(IFile::ReadWrite, state->error))
			{
			    int res;
			    struct archive_entry *e;
				QByteArray entryPathUtf8;

				for (ArcNodeDirEntryItem *parent = static_cast<const ArcNodeDirEntryItem *>(entry->parent()); parent; parent = static_cast<const ArcNodeDirEntryItem *>(parent->parent()))
					entryPath.prepend(QChar('/')).prepend(static_cast<const ArcNodeDirEntryItem *>(parent)->fileName());

				entryPathUtf8 = entryPath.toUtf8();

			    while ((res = archive_read_next_header(state->a, &e)) == ARCHIVE_OK && !aborted)
			    	if (strcmp(entryPathUtf8, archive_entry_pathname(e)) == 0)
					{
			    		int size;

						for (;;)
						{
							size = archive_read_data(state->a, buffer, bufferSize);

							if (size < 0)
							{
								state->error = QString::fromUtf8(archive_error_string(state->a));
								break;
							}

							if (size == 0)
								break;

							file->write(buffer, size);
						}

						break;
					}
			    	else
			    		archive_read_data_skip(state->a);

			    if (res != ARCHIVE_EOF && res != ARCHIVE_OK)
					state->error = QString::fromUtf8(archive_error_string(state->a));
			}
		}
	}
}

void LibArchivePlugin::endRead(State *s) const
{
	LibArchiveState *state = static_cast<LibArchiveState *>(s);

	if (state->a)
	{
		archive_read_close(state->a);
		archive_read_finish(state->a);
	}

	delete state;
}

ARC_PLUGIN_NS_END
