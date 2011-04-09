#ifndef FILESYSTEMENTRY_H_
#define FILESYSTEMENTRY_H_

#include <QDir>
#include <QString>
#include "filesystemitem.h"
#include "../filesysteminfo.h"
#include "../filesystemchangeslist.h"


class FileSystemEntry : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemEntry)

public:
	FileSystemEntry(const FileSystemInfo &info, FileSystemItem *parent = 0);
	FileSystemEntry(const Change &change, FileSystemItem *parent = 0);

	virtual QVariant data(qint32 column, qint32 role) const;
	virtual void accept(FileSystemModelVisitor *visitor) const;

	QIcon icon() const { return m_info.icon(); }

	const QFileInfo &fileInfo() const { return m_info.fileInfo(); }
	QFileInfo &fileInfo() { return m_info.fileInfo(); }

	void update(const FileSystemInfo &info) { m_info = info; }

	const QVariant &fileSize() const { return m_fileSize; }
	void setFileSize(const QVariant &value) { m_fileSize = value; }

	bool isLocked() const { return m_locked; }
	void setLocked(bool value) { m_locked = value; }

	QString humanReadableSize(quint64 size) const;

private:
	bool m_locked;
	QVariant m_fileSize;
	FileSystemInfo m_info;
};

#endif /* FILESYSTEMENTRY_H_ */
