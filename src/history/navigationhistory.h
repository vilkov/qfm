#ifndef NAVIGATIONHISTORY_H_
#define NAVIGATIONHISTORY_H_

#include "historyentry.h"


HISTORY_NS_BEGIN

class Navigation
{
public:
	Navigation();
	~Navigation();

	void init(Entry *entry);
	void save(Entry *entry);
	Entry *backward();
	Entry *forward();

private:
	Entry *m_current;
};

HISTORY_NS_END

#endif /* NAVIGATIONHISTORY_H_ */
