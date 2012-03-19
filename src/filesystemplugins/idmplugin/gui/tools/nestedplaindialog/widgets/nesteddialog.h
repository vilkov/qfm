#ifndef NESTEDDIALOG_H_
#define NESTEDDIALOG_H_

#include "nestedwidget.h"


class BaseNestedDialog
{
public:
	virtual ~BaseNestedDialog();

protected:
	void accept(NestedWidget *widget) { widget->accept(); }
	void reject(NestedWidget *widget) { widget->reject(); }
};


class NestedDialog : public BaseNestedDialog
{
protected:
	friend class BaseNestedWidget;
	virtual void accepted() = 0;
	virtual void rejected() = 0;

	virtual int open(NestedWidget *widget) = 0;
	virtual void critical(const QString &text) = 0;
	virtual void critical(const QString &title, const QString &text) = 0;
};

#endif /* NESTEDDIALOG_H_ */
