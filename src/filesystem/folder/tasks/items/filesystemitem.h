#ifndef FILESYSTEMITEM_H_
#define FILESYSTEMITEM_H_

#include <QtCore/QList>
#include "../../info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public Info
{
public:
	typedef FileSystemItem    * value_type;
	typedef QList<value_type>   List;
	typedef List::size_type     size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	FileSystemItem(const Info &info) :
		Info(info)
	{}

	virtual bool isList() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEM_H_ */
