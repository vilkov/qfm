#ifndef M3UDELEGATE_H_
#define M3UDELEGATE_H_

#include "../../filesystem/filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class M3uDelegate : public Delegate
{
	Q_DISABLE_COPY(M3uDelegate)

public:
	M3uDelegate(QObject *parent = 0)
	{}
};

FILE_SYSTEM_NS_END

#endif /* M3UDELEGATE_H_ */
