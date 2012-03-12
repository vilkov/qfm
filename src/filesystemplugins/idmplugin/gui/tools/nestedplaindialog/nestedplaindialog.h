#ifndef NESTEDPLAINDIALOG_H_
#define NESTEDPLAINDIALOG_H_

#include <QtGui/QFrame>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "widgets/mainnestedwidget.h"


class NestedPlainDialog : public QDialog
{
public:
	NestedPlainDialog(QWidget *parent = 0);

protected:
	void setCentralWidget(QWidget *widget);

private:
	MainNestedWidget *m_mainWidget;
	BaseNestedWidget *m_prevWidget;
	BaseNestedWidget *m_currentWidget;
	QVBoxLayout m_vlayout;
	QFrame m_frame;
	QHBoxLayout m_hlayout;
	QDialogButtonBox m_buttonBox;
};

#endif /* NESTEDPLAINDIALOG_H_ */
