#ifndef STRINGDIALOG_H_
#define STRINGDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>


class StringDialog : public QDialog
{
    Q_OBJECT

public:
    StringDialog(const QString &title, const QString &label, const QString &value = QString(), QWidget *parent = 0);

    QString value() const { return m_lineEdit.text().simplified(); }
    virtual void accept();

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};

#endif /* STRINGDIALOG_H_ */
