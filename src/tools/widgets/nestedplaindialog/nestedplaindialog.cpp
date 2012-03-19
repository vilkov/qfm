#include "nestedplaindialog.h"
#include <QtCore/QEventLoop>
#include <QtGui/QMessageBox>


NestedPlainDialog::NestedPlainDialog(QWidget *parent) :
	QDialog(parent),
	m_loop(NULL),
	m_mainWidget(NULL),
	m_currentWidget(NULL),
	m_vlayout(this),
	m_splitter(Qt::Horizontal, this),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

	m_splitter.setChildrenCollapsible(false);

	m_vlayout.setMargin(3);
	m_vlayout.setSpacing(1);
	m_vlayout.addWidget(&m_splitter);
	m_vlayout.addWidget(&m_buttonBox);

	connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void NestedPlainDialog::accept()
{
	if (m_currentWidget)
		NestedDialog::accept(m_currentWidget);
	else
		QDialog::accept();
}

void NestedPlainDialog::reject()
{
	if (m_currentWidget)
		NestedDialog::reject(m_currentWidget);
	else
		QDialog::reject();
}

void NestedPlainDialog::accepted()
{
	Q_ASSERT(m_currentWidget);
	m_loop->exit(NestedWidget::Accepted);
}

void NestedPlainDialog::rejected()
{
	Q_ASSERT(m_currentWidget);
	m_loop->exit(NestedWidget::Rejected);
}

void NestedPlainDialog::warning(const QString &text)
{
	QMessageBox::warning(this, windowTitle(), text);
}

void NestedPlainDialog::warning(const QString &title, const QString &text)
{
	QMessageBox::warning(this, title, text);
}

void NestedPlainDialog::critical(const QString &text)
{
	QMessageBox::critical(this, windowTitle(), text);
}

void NestedPlainDialog::critical(const QString &title, const QString &text)
{
	QMessageBox::critical(this, title, text);
}

int NestedPlainDialog::open(NestedWidget *widget)
{
	int res = NestedWidget::Rejected;
	NestedWidget *currentWidget = m_currentWidget;
	QEventLoop *currentLoop = m_loop;
	QEventLoop loop;

	if (currentWidget)
		currentWidget->setReadOnly(true);
	else
		m_mainWidget->setReadOnly(true);

	m_loop = &loop;
	m_currentWidget = widget;
	m_splitter.addWidget(widget->centralWidget());
	widget->setFocus();

	res = m_loop->exec();

//	m_splitter.removeWidget(widget->centralWidget());
	m_currentWidget = currentWidget;
	m_loop = currentLoop;

	if (currentWidget)
	{
		currentWidget->setReadOnly(false);
		currentWidget->setFocus();
	}
	else
	{
		m_mainWidget->setReadOnly(false);
		m_mainWidget->setFocus();
	}

	return res;
}

void NestedPlainDialog::setCentralWidget(BaseNestedWidget *widget)
{
	Q_ASSERT(m_mainWidget == NULL);
	m_splitter.addWidget((m_mainWidget = widget)->centralWidget());
	m_mainWidget->setFocus();
}
