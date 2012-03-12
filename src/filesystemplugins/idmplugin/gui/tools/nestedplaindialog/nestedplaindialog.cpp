#include "nestedplaindialog.h"


NestedPlainDialog::NestedPlainDialog(QWidget *parent) :
	QDialog(parent),
	m_mainWidget(NULL),
	m_prevWidget(NULL),
	m_currentWidget(NULL),
	m_vlayout(this),
	m_frame(this),
	m_hlayout(&m_frame),
	m_buttonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this)
{
	m_vlayout.setMargin(3);
	m_vlayout.setSpacing(1);
	m_vlayout.addWidget(&m_frame);
	m_vlayout.addWidget(&m_buttonBox);

	m_hlayout.setMargin(3);
	m_hlayout.setSpacing(1);
	m_hlayout.addWidget(m_mainWidget);
}

void NestedPlainDialog::setCentralWidget(QWidget *widget)
{

}
