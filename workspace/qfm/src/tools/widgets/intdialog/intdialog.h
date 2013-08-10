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
