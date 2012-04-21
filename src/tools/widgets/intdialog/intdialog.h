#ifndef INTDIALOG_H_
#define INTDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>


class IntDialog : public QDialog
{
    Q_OBJECT

public:
    IntDialog(const QString &title, const QString &label, int value = 0, QWidget *parent = 0);

    int value() const { return m_lineEdit.value(); }
    virtual void accept();

private:
    QLabel m_label;
    QSpinBox m_lineEdit;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};

#endif /* INTDIALOG_H_ */
