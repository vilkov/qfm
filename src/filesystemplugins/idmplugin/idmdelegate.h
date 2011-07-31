#ifndef IDMDELEGATE_H_
#define IDMDELEGATE_H_

#include "../../filesystem/filesystemdelegate.h"


FILE_SYSTEM_NS_BEGIN

class IdmDelegate : public Delegate
{
	Q_DISABLE_COPY(IdmDelegate)

public:
	IdmDelegate(QObject *parent = 0)
	{}
};

FILE_SYSTEM_NS_END

#endif /* IDMDELEGATE_H_ */
