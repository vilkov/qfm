#ifndef FILESYSTEMFILETYPEINFO_UNIX_H_
#define FILESYSTEMFILETYPEINFO_UNIX_H_

#include <QtCore/QString>
#include <QtGui/QIcon>


struct FileTypeInfo
{
	FileTypeInfo();

    QIcon icon;
    QString mimeType;
    QString descritpion;
};

#endif /* FILESYSTEMFILETYPEINFO_UNIX_H_ */
