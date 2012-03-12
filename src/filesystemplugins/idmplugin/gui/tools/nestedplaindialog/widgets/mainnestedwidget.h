#ifndef MAINNESTEDWIDGET_H_
#define MAINNESTEDWIDGET_H_

#include "basenestedwidget.h"


class MainNestedWidget : public BaseNestedWidget
{
public:
	MainNestedWidget();

	virtual QString title() const = 0;
};

#endif /* MAINNESTEDWIDGET_H_ */
