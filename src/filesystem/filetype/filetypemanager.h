#ifndef FILETYPEMANAGER_H_
#define FILETYPEMANAGER_H_

#ifndef Q_OS_WIN
#	include <sys/types.h>
#endif

#include <QtCore/QString>
#include "../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class FileTypeManager
{
public:
	class MimeType
	{
	public:
		MimeType();
		MimeType(const QString &type, const QString &subtype);

		const QString &type() const { return m_type; }
		const QString &subtype() const { return m_subtype; }

	private:
		QString m_type;
		QString m_subtype;
	};

public:
	FileTypeManager();
	~FileTypeManager();

	MimeType type(const QString &absoluteFilePath) const;
	Info info(const QString &absoluteFilePath) const;

private:
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

FILE_SYSTEM_NS_END

#endif /* FILETYPEMANAGER_H_ */
