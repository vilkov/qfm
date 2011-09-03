#include "stringdialog.h"
#include <QtGui/QMessageBox>


StringDialog::StringDialog(const QString &title, const QString &label, const QString &value, QWidget *parent) :
	QDialog(parent),
	m_label(this),
	m_lineEdit(value, this),
	m_gridLayout(this),
	m_buttonBox(this)
{
	setWindowTitle(title);

    QFont font;
    font.setBold(true);
    font.setWeight(75);
    m_label.setFont(font);
    m_label.setText(label);

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

    m_gridLayout.setSpacing(6);
    m_gridLayout.setContentsMargins(6, 6, 6, 6);
    m_gridLayout.addWidget(&m_label, 0, 0, 1, 1);
    m_gridLayout.addWidget(&m_lineEdit, 0, 1, 1, 1);
    m_gridLayout.addWidget(&m_buttonBox, 1, 0, 1, 2);

    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_lineEdit.selectAll();
}

void StringDialog::accept()
{
	if (check())
		QDialog::accept();
}

bool StringDialog::check()
{
	if (value().isEmpty())
	{
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
		return false;
	}
	else
		return true;
}
