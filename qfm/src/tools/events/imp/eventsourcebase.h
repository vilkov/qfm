#ifndef EVENTSOURCEBASE_H_
#define EVENTSOURCEBASE_H_

#include <QtGui/QWidget>
#include "../eventhandler.h"
#include "../../metatemplates.h"


template <typename BaseClass>
class EventSourceBase : public BaseClass
{
public:
	EventSourceBase(EventHandler *eventHandler, QWidget *parent = 0) :
		BaseClass(parent),
	    m_eventHandler(eventHandler)
	{
		Q_ASSERT(m_eventHandler != 0);
	}

protected:
    inline EventHandler *eventHandler() const { return m_eventHandler; }

private:
    EventHandler *m_eventHandler;
};

#endif /* EVENTSOURCEBASE_H_ */
