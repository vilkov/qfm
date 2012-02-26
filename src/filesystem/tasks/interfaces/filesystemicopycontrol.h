#ifndef FILESYSTEMICOPYCONTROL_H_
#define FILESYSTEMICOPYCONTROL_H_

#include "../containers/filesystemscanedfiles.h"
#include "../../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class ICopyControl : public IFileContainer
{
public:
	virtual bool start(const ScanedFiles::Files &files, bool move) = 0;
	virtual void done(bool error) = 0;
	virtual void canceled() = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMICOPYCONTROL_H_ */
