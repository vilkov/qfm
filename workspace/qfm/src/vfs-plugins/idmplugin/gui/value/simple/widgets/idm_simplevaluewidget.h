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
#ifndef SIMPLEVALUEWIDGET_H_
#define SIMPLEVALUEWIDGET_H_

#include <tools/widgets/valuedialog/valuedialogmetafunctions.h>
#include <tools/widgets/nestedplaindialog/widgets/nestedwidget.h>


template <typename T>
class MainSimpleValueWidget : public BaseNestedWidget
{
public:
    MainSimpleValueWidget(NestedDialog *parent, const T &value = T());

    virtual QWidget *centralWidget();
    virtual void setReadOnly(bool value);
    virtual void setFocus();

    virtual void accept();
    T value() const;

private:
    typedef typename Editor<T>::type Editor;

private:
    Editor m_edit;
};


template <typename T>
MainSimpleValueWidget<T>::MainSimpleValueWidget(NestedDialog *parent, const T &value) :
    BaseNestedWidget(parent),
    m_edit()
{
    EditorValue<T>::setValue(&m_edit, value);
    m_edit.selectAll();
}

template <typename T>
QWidget *MainSimpleValueWidget<T>::centralWidget()
{
    return &m_edit;
}

template <typename T>
void MainSimpleValueWidget<T>::setReadOnly(bool value)
{
    m_edit.setEnabled(!value);
}

template <typename T>
void MainSimpleValueWidget<T>::setFocus()
{
    m_edit.setFocus();
}

template <typename T>
void MainSimpleValueWidget<T>::accept()
{
    if (value() == T())
        warning(tr("You must enter the value!"));
    else
        BaseNestedWidget::accept();
}

template <typename T>
T MainSimpleValueWidget<T>::value() const
{
    return EditorValue<T>::value(&m_edit);
}


template <typename T>
class SimpleValueWidget : public NestedWidget
{
public:
    SimpleValueWidget(NestedDialog *parent, const QString &title, const T &value = T());

    virtual void setFocus();

    virtual void accept();
    T value() const;

private:
    typedef typename Editor<T>::type Editor;

private:
    Editor m_edit;
};


template <typename T>
SimpleValueWidget<T>::SimpleValueWidget(NestedDialog *parent, const QString &title, const T &value) :
    NestedWidget(parent, title),
    m_edit()
{
    EditorValue<T>::setValue(&m_edit, value);
    m_edit.selectAll();

    setCentralWidget(&m_edit);
}

template <typename T>
void SimpleValueWidget<T>::setFocus()
{
    m_edit.setFocus();
}

template <typename T>
void SimpleValueWidget<T>::accept()
{
    if (value() == T())
        warning(tr("You must enter the value!"));
    else
        NestedWidget::accept();
}

template <typename T>
T SimpleValueWidget<T>::value() const
{
    return EditorValue<T>::value(&m_edit);
}

#endif /* SIMPLEVALUEWIDGET_H_ */
