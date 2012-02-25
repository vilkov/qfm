#ifndef NAVIGATIONHISTORY_H_
#define NAVIGATIONHISTORY_H_

#include "historyentry.h"


HISTORY_NS_BEGIN

class Navigation
{
public:
	Navigation();
	~Navigation();

	void insert(Entry *entry);

	Entry *backward();
	Entry *forvard();

private:
	inline void removeTail(Entry *entry);

private:
	Entry *m_current;
};

HISTORY_NS_END

#endif /* NAVIGATIONHISTORY_H_ */
