#ifndef BASENESTEDWIDGET_H_
#define BASENESTEDWIDGET_H_

#include <QtGui/QWidget>


class BaseNestedWidget : public QWidget
{
public:
	BaseNestedWidget();

	virtual void setReadOnly(bool value) = 0;
};

#endif /* BASENESTEDWIDGET_H_ */
