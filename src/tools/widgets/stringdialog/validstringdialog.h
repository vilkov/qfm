#ifndef VALIDSTRINGDIALOG_H_
#define VALIDSTRINGDIALOG_H_

#include <QRegExp>
#include "stringdialog.h"


class ValidStringDialog : public StringDialog
{
    Q_OBJECT

public:
    ValidStringDialog(const QString &title, const QString &label, const QString &regexp, QWidget *parent = 0);
    virtual void accept();

private:
    QRegExp m_regexp;
};

#endif /* VALIDSTRINGDIALOG_H_ */
