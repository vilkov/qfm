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
#ifndef CHOOSEDIALOG_H_
#define CHOOSEDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QDialogButtonBox>


class ChooseDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QList<QPair<QString, void*> > List;

public:
    ChooseDialog(const QString &title, const List &values, QWidget *parent = 0);

    void *value() const;
    virtual void accept();

private:
    bool check();

private:
    typedef QList<QPair<void*, QRadioButton*> > Container;

private:
    Container m_values;
    QVBoxLayout m_layout;
    QDialogButtonBox m_buttonBox;
};

#endif /* CHOOSEDIALOG_H_ */
