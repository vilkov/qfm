#ifndef VFS_NS_H_
#define VFS_NS_H_


#define VFS_NS_BEGIN namespace VFS {
#define VFS_NS_END }

#define VFS_NS(Expression) \
		VFS_NS_BEGIN       \
			Expression;    \
		VFS_NS_END

#endif /* VFS_NS_H_ */
