#ifndef FILESYSTEMMODELVISITOR_H_
#define FILESYSTEMMODELVISITOR_H_

#include "../items/filesystementry.h"


class FileSystemModelVisitor
{
public:
	virtual void visit(const FileSystemEntry *entry) = 0;
};

#endif /* FILESYSTEMMODELVISITOR_H_ */
