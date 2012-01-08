#ifndef FILESYSTEMFILETYPEID_WIN32_H_
#define FILESYSTEMFILETYPEID_WIN32_H_

#include <QtCore/QString>


struct FileTypeId_Win32
{
	FileTypeId_Win32();

    QString extension;
    QIcon icon;
};

#endif /* FILESYSTEMFILETYPEID_WIN32_H_ */
