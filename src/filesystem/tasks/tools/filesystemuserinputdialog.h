#ifndef FILESYSTEMUSERINPUTDIALOG_H_
#define FILESYSTEMUSERINPUTDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../filesystem_ns.h"


class UserInputDialog : public QDialog
{
	Q_OBJECT

public:
    enum Icon
    {
        Information,
        Warning,
        Critical,
        Question
    };

public:
	UserInputDialog(Icon icon, const QString &title, const QString &question, qint32 buttons, QWidget *parent = 0);

	int answer() const { return m_answer; }
	QString value() const { return m_lineEdit.text(); }

private Q_SLOTS:
	void buttonClicked(QAbstractButton *button);

private:
	int m_answer;
	QVBoxLayout m_layout;
	QHBoxLayout m_hLayout;
	QVBoxLayout m_vLayout;
	QLabel m_iconLabel;
	QLabel m_text;
	QLineEdit m_lineEdit;
	QDialogButtonBox m_buttonBox;
};

#endif /* FILESYSTEMUSERINPUTDIALOG_H_ */
