#ifndef M3UDELEGATE_H_
#define M3UDELEGATE_H_

#include "../../filesystem/filesystemdelegate.h"


class M3uDelegate : public FileSystem::Delegate
{
	Q_DISABLE_COPY(M3uDelegate)

public:
	M3uDelegate(QObject *parent = 0)
	{}
};

#endif /* M3UDELEGATE_H_ */
