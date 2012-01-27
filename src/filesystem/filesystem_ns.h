#ifndef FILESYSTEM_NS_H_
#define FILESYSTEM_NS_H_

#include "../memory/tags/global_memory_tag.h"

#define FILE_SYSTEM_NS_BEGIN namespace FileSystem {
#define FILE_SYSTEM_NS_END }

#define FILE_SYSTEM_NS(Expression) \
		FILE_SYSTEM_NS_BEGIN       \
			Expression;            \
		FILE_SYSTEM_NS_END

#endif /* FILESYSTEM_NS_H_ */
