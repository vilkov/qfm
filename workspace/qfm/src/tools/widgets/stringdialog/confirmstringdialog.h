/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CONFIRMSTRINGDIALOG_H_
#define CONFIRMSTRINGDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGridLayout>
#include <QtGui/QDialogButtonBox>


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
