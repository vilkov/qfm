#include "validstringdialog.h"
#include <QMessageBox>


ValidStringDialog::ValidStringDialog(const QString &title, const QString &label, const QString &regexp, QWidget *parent) :
	StringDialog(title, label, QString(), parent),
    m_regexp(regexp)
{}

void ValidStringDialog::accept()
{
	QString res = value();

	if (res.isEmpty())
		QMessageBox::information(this, windowTitle(), tr("You must enter the value!"));
	else
		if (m_regexp.exactMatch(res))
			QDialog::accept();
		else
			QMessageBox::information(this, windowTitle(), tr("You must enter the valid value!"));
}
