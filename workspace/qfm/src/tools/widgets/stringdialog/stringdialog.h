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

protected:
    virtual bool check();

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QGridLayout m_gridLayout;
    QDialogButtonBox m_buttonBox;
};

#endif /* STRINGDIALOG_H_ */
