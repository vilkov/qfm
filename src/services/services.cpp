#include "services.h"


static Services *selfInstance = 0;

Services::Services()
{
	selfInstance = this;
}

void Services::view(const FileSystem::Info &file)
{

}

void Services::edit(const FileSystem::Info &file)
{

}

void Services::open(const FileSystem::Info &file)
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
