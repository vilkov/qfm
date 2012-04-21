#include "confirmstringdialog.h"
#include <QMessageBox>


ConfirmStringDialog::ConfirmStringDialog(const QString &title, const QString &label1, const QString &label2, QWidget *parent) :
	QDialog(parent),
	m_label1(this),
	m_label2(this),
	m_lineEdit1(this),
	m_lineEdit2(this),
	m_gridLayout(this),
	m_buttonBox(this)
{
	setWindowTitle(title);

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    m_label1.setFont(font);
    m_label1.setText(label1);
    m_label2.setFont(font);
    m_label2.setText(label2);

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    m_gridLayout.setSpacing(6);
    m_gridLayout.setContentsMargins(6, 6, 6, 6);
    m_gridLayout.addWidget(&m_label1, 0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit1, 0, 1, 1, 1);
    m_gridLayout.addWidget(&m_label2, 1, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit2, 1, 1, 1, 1);
    m_gridLayout.addWidget(&m_buttonBox, 2, 0, 1, 2);

    m_lineEdit1.setEchoMode(QLineEdit::Password);
    m_lineEdit2.setEchoMode(QLineEdit::Password);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ConfirmStringDialog::accept()
{
	if (m_lineEdit1.text().isEmpty())
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
	else
		if (m_lineEdit1.text() != m_lineEdit2.text())
			QMessageBox::information(this, windowTitle(), tr("Values do not match!"));
		else
			QDialog::accept();
}
