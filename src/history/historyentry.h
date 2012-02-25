#ifndef HISTORYENTRY_H_
#define HISTORYENTRY_H_

#include "history_ns.h"


HISTORY_NS_BEGIN

class Entry
{
public:
	virtual ~Entry();

private:
	friend class Navigation;
	Entry *m_head;
	Entry *m_prev;
	Entry *m_next;
};

HISTORY_NS_END

#endif /* HISTORYENTRY_H_ */
