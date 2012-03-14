#ifndef NESTEDPLAINDIALOG_H_
#define NESTEDPLAINDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QSplitter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "widgets/nesteddialog.h"


class QEventLoop;

class NestedPlainDialog : public QDialog, public NestedDialog
{
public:
	NestedPlainDialog(QWidget *parent = 0);

	virtual void accept();
	virtual void reject();

protected:
	/* NestedDialog */
	virtual void accepted();
	virtual void rejected();

	void setCentralWidget(QWidget *widget);
	int open(NestedWidget *widget);

private:
	QEventLoop *m_loop;
	QWidget *m_mainWidget;
	NestedWidget *m_currentWidget;

private:
	QVBoxLayout m_vlayout;
	QSplitter m_splitter;
	QDialogButtonBox m_buttonBox;
};

#endif /* NESTEDPLAINDIALOG_H_ */
