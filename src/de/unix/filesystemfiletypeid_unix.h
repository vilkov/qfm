#ifndef FILESYSTEMFILETYPEID_UNIX_H_
#define FILESYSTEMFILETYPEID_UNIX_H_

#include <QtCore/QString>
#include <QtGui/QIcon>


struct FileTypeId_Unix
{
	FileTypeId_Unix();

	bool operator==(const FileTypeId_Unix &other) const { return mime == other.mime; }
	bool operator<(const FileTypeId_Unix &other) const { return mime < other.mime; }

    QString mime;
};

#endif /* FILESYSTEMFILETYPEID_UNIX_H_ */
