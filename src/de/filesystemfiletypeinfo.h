#ifndef FILESYSTEMFILETYPEINFO_H_
#define FILESYSTEMFILETYPEINFO_H_

#if !defined(DESKTOP_ENVIRONMENT_IS_WINDOWS)
#	include "unix/filesystemfiletypeinfo_unix.h"
#else
#	include "win32/filesystemfiletypeinfo_win32.h"
#endif

#endif /* FILESYSTEMFILETYPEINFO_H_ */
