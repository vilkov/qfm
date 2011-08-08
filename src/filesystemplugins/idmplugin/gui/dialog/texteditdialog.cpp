#include "texteditdialog.h"


TextEditDialog::TextEditDialog(const QString &text, QWidget *parent):
	QDialog(parent)
{
	ui.setupUi(this);
	ui.plainTextEdit->document()->setPlainText(text);
}
