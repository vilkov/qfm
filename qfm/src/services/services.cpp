#include "services.h"


static Services *selfInstance = 0;

Services::Services()
{
	selfInstance = this;
}

void Services::view(const QString &filePath)
{

}

void Services::edit(const QString &filePath)
{

}

void Services::open(const QString &filePath)
{

}

Services *Services::instance()
{
	Q_ASSERT(selfInstance != 0);
	return selfInstance;
}

void Services::registerService(View *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Edit *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Open *service)
{
	Q_ASSERT(selfInstance != 0);
}
