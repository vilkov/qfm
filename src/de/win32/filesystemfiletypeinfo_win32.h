#ifndef FILESYSTEMFILETYPEINFO_WIN32_H_
#define FILESYSTEMFILETYPEINFO_WIN32_H_

#include <QtCore/QString>


struct FileTypeInfo
{
	FileTypeInfo();

    QString extension;
    QIcon icon;
};

#endif /* FILESYSTEMFILETYPEINFO_WIN32_H_ */
