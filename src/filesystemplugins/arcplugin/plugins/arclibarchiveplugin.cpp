#include "arclibarchiveplugin.h"
#include "../nodes/items/arcnodeentryitem.h"
#include "../nodes/items/arcnodedirentryitem.h"
#include "../../../filesystem/info/filesystemfilestree.h"
#include <string.h>
#include <archive.h>
#include <archive_entry.h>
#include <QtCore/QMap>


ARC_PLUGIN_NS_BEGIN

struct LibArchiveState : public LibArchivePlugin::State
{
	LibArchiveState() :
		a(NULL),
		callback(0)
	{}

    struct archive *a;
    LibArchivePlugin::Callback *callback;
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
	Q_ASSERT(s && s->error.isEmpty());
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

    if (res != ARCHIVE_EOF && !aborted)
		state->error = QString::fromUtf8(archive_error_string(state->a));

    return contents;
}

void LibArchivePlugin::extract(State *s, const ArcNodeItem *entry, const IFileControl *dest, Callback *callback, const volatile Flags &aborted) const
{
	Q_ASSERT(s && s->error.isEmpty());
	Q_ASSERT(callback);
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
	state->callback = callback;
	bool tryAgain;

	state->callback->progressInit(entry);

	if (entry->isDir())
		extractEntry(state, dest, static_cast<const ArcNodeListItem *>(entry), tryAgain = false, aborted);
	else
		extractFile(state, dest, static_cast<const ArcNodeItem *>(entry), tryAgain = false, aborted);

	state->callback->progresscomplete();
}

void LibArchivePlugin::extractAll(State *s, const IFileControl *dest, Callback *callback, const volatile Flags &aborted) const
{
	Q_ASSERT(s && s->error.isEmpty());
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
	const IFileControl *control;
    struct archive_entry *e;
    bool tryAgain = false;
	FilesTree tree(dest);
    int res;

    state->callback = callback;

    while (!aborted && (res = archive_read_next_header(state->a, &e)) == ARCHIVE_OK)
    	do
			if (control = tree.open(const_cast<char *>(archive_entry_pathname(e)), true, state->error))
				if (control->isFile())
					if (tryAgain || state->callback->overwriteAll())
						doExtractFile(state, control, tryAgain = false, aborted);
					else
						state->callback->askForOverwrite(
								tr("File \"%1\" already exists in \"%3\". Overwrite it?").
									arg(control->fileName()).
									arg(control->absolutePath()),
								tryAgain = false,
								aborted);
				else
					doExtractFile(state, control, tryAgain = false, aborted);
			else
				state->callback->askForSkipIfNotCopy(
						tr("Failed to open/create directory \"%1\". Skip it?").arg(state->error),
						tryAgain = false,
						aborted);
    	while (tryAgain && !aborted);

    if (res != ARCHIVE_EOF && !aborted)
		state->error = QString::fromUtf8(archive_error_string(state->a));
}

void LibArchivePlugin::endRead(State *s) const
{
	Q_ASSERT(s);
	LibArchiveState *state = static_cast<LibArchiveState *>(s);

	if (state->a)
	{
		archive_read_close(state->a);
		archive_read_finish(state->a);
	}

	delete state;
}

void LibArchivePlugin::extractEntry(State *s, const IFileControl *destination, const ArcNodeListItem *e, volatile bool &tryAgain, const volatile Flags &aborted) const
{
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
	const ArcNodeDirEntryItem *entry = static_cast<const ArcNodeDirEntryItem *>(e);

	do
		if (destination->contains(entry->fileName()))
		{
			PScopedPointer<IFileControl> dest;

			if (dest = destination->openFolder(entry->fileName(), false, state->error))
				for (ArcNodeListItem::size_type i = 0;
						i < entry->size() && !aborted;
						++i)
					if (static_cast<const ArcNodeItem *>(entry->at(i))->isDir())
						extractEntry(state, dest.data(), static_cast<const ArcNodeListItem *>(entry->at(i)), tryAgain = false, aborted);
					else
						extractFile(state, dest.data(), static_cast<const ArcNodeItem *>(entry->at(i)), tryAgain = false, aborted);
			else
				if (state->callback->skipAllIfNotCopy())
					break;
				else
					state->callback->askForSkipIfNotCopy(
							tr("Failed to open directory \"%1\". Skip it?").
								arg(destination->absoluteFilePath(entry->fileName())),
							tryAgain = false,
							aborted);
		}
		else
		{
			PScopedPointer<IFileControl> dest;

			if (dest = destination->openFolder(entry->fileName(), true, state->error))
				for (ArcNodeListItem::size_type i = 0;
						i < entry->size() && !aborted;
						++i)
					if (static_cast<const ArcNodeItem *>(entry->at(i))->isDir())
						extractEntry(state, dest.data(), static_cast<const ArcNodeListItem *>(entry->at(i)), tryAgain = false, aborted);
					else
						extractFile(state, dest.data(), static_cast<const ArcNodeItem *>(entry->at(i)), tryAgain = false, aborted);
			else
				if (state->callback->skipAllIfNotCopy())
					break;
				else
					state->callback->askForSkipIfNotCopy(
							tr("Failed to create directory \"%1\". Skip it?").
								arg(destination->absoluteFilePath(entry->fileName())),
							tryAgain = false,
							aborted);
		}
	while (tryAgain && !aborted);
}

void LibArchivePlugin::extractFile(State *s, const IFileControl *destination, const ArcNodeItem *e, volatile bool &tryAgain, const volatile Flags &aborted) const
{
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
	const ArcNodeEntryItem *entry = static_cast<const ArcNodeEntryItem *>(e);

	do
		if (destination->contains(entry->fileName()))
			if (tryAgain || state->callback->overwriteAll())
				doExtractFile(state, destination, entry, tryAgain = false, aborted);
			else
				state->callback->askForOverwrite(
						tr("File \"%1\" already exists in \"%3\". Overwrite it?").
							arg(entry->fileName()).
							arg(destination->absoluteFilePath()),
						tryAgain = false,
						aborted);
		else
			doExtractFile(state, destination, entry, tryAgain = false, aborted);
	while (tryAgain && !aborted);
}

void LibArchivePlugin::doExtractFile(State *s, const IFileControl *destination, const ArcNodeItem *entry, volatile bool &tryAgain, const volatile Flags &aborted) const
{
    int res;
    struct archive_entry *e;
	QByteArray entryPathUtf8;
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
    QString entryPath = static_cast<const ArcNodeEntryItem *>(entry)->fileName();

	for (ArcNodeDirEntryItem *parent = static_cast<const ArcNodeDirEntryItem *>(entry->parent()); parent; parent = static_cast<const ArcNodeDirEntryItem *>(parent->parent()))
		entryPath.prepend(QChar('/')).prepend(static_cast<const ArcNodeDirEntryItem *>(parent)->fileName());

	entryPathUtf8 = entryPath.toUtf8();

	while ((res = archive_read_next_header(state->a, &e)) == ARCHIVE_OK && !aborted)
		if (strcmp(entryPathUtf8, archive_entry_pathname(e)) == 0)
		{
			PScopedPointer<IFileControl> destEntry;

			do
				if (destEntry = destination->openFile(static_cast<const ArcNodeEntryItem *>(entry)->fileName(), state->error))
				{
					PScopedPointer<IFile> m_destFile;

					if (m_destFile = destEntry->file(IFile::ReadWrite, state->error))
					{
						int size;
						IFile::value_type *buffer = state->callback->buffer();
						IFile::size_type bufferSize = state->callback->bufferSize();

						for (;!aborted;)
						{
							size = archive_read_data(state->a, buffer, bufferSize);

							if (size < 0)
							{
								state->error = QString::fromUtf8(archive_error_string(state->a));
								break;
							}

							if (size == 0)
								break;

							if (m_destFile->write(buffer, size) == (IFile::size_type)size)
								state->callback->progressUpdate(size);
							else
							{
								state->callback->askForSkipIfNotCopy(
										tr("Failed to write to file \"%1\" (%2). Skip it?").
											arg(destEntry->absoluteFilePath()).
											arg(state->error = m_destFile->lastError()),
										tryAgain = false,
										aborted);

								break;
							}
						}
					}
					else
						if (state->callback->skipAllIfNotCopy() || tryAgain)
							break;
						else
							state->callback->askForSkipIfNotCopy(
									tr("Failed to create file \"%1\" (%2). Skip it?").
										arg(destination->absoluteFilePath(static_cast<const ArcNodeEntryItem *>(entry)->fileName())).
										arg(state->error),
									tryAgain = false,
									aborted);
				}
				else
					if (state->callback->skipAllIfNotCopy() || tryAgain)
						break;
					else
						state->callback->askForSkipIfNotCopy(
								tr("Failed to create file \"%1\" (%2). Skip it?").
									arg(destination->absoluteFilePath(static_cast<const ArcNodeEntryItem *>(entry)->fileName())).
									arg(state->error),
								tryAgain = false,
								aborted);
			while (tryAgain && !aborted);

			break;
		}
		else
			archive_read_data_skip(state->a);

	if (res != ARCHIVE_EOF && res != ARCHIVE_OK)
		state->error = QString::fromUtf8(archive_error_string(state->a));
}

void LibArchivePlugin::doExtractFile(State *s, const IFileControl *control, volatile bool &tryAgain, const volatile Flags &aborted) const
{
	LibArchiveState *state = static_cast<LibArchiveState *>(s);
	PScopedPointer<IFile> file;

	do
		if (file = control->file(IFile::ReadWrite, state->error))
		{
			int size;
			IFile::value_type *buffer = state->callback->buffer();
			IFile::size_type bufferSize = state->callback->bufferSize();

			for (;!aborted;)
			{
				size = archive_read_data(state->a, buffer, bufferSize);

				if (size < 0)
				{
					state->error = QString::fromUtf8(archive_error_string(state->a));
					break;
				}

				if (size == 0)
					break;

				if (file->write(buffer, size) != (IFile::size_type)size)
					break;
			}
		}
		else
			if (state->callback->skipAllIfNotCopy() || tryAgain)
				break;
			else
				state->callback->askForSkipIfNotCopy(
						tr("Failed to create file \"%1\" (%2). Skip it?").
							arg(control->absoluteFilePath()).
							arg(state->error),
						tryAgain = false,
						aborted);
	while (tryAgain && !aborted);
}

ARC_PLUGIN_NS_END
