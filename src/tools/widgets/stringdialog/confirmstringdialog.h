#ifndef CONFIRMSTRINGDIALOG_H_
#define CONFIRMSTRINGDIALOG_H_

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QDialogButtonBox>


class ConfirmStringDialog : public QDialog
{
    Q_OBJECT

public:
    ConfirmStringDialog(const QString &title, const QString &label1, const QString &label2, QWidget *parent = 0);

    QString value() const { return m_lineEdit1.text(); }
    virtual void accept();

private:
    QLabel m_label1;
    QLabel m_label2;
    QLineEdit m_lineEdit1;
    QLineEdit m_lineEdit2;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};

#endif /* CONFIRMSTRINGDIALOG_H_ */
