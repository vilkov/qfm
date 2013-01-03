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
#ifndef IDM_CONSTRAINTQUERYDIALOG_H_
#define IDM_CONSTRAINTQUERYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <tools/events/qt/imp/mouseeventhandler.h>
#include <tools/events/qt/imp/mouseeventsource.h>
#include <tools/events/qt/imp/keyboardeventhandler.h>
#include <tools/events/qt/imp/keyboardeventsource.h>
#include "../../../containeres/idm_container.h"
#include "../../../storage/constraints/value/idm_constraint.h"


using namespace ::VFS::Plugins::Idm;
using namespace ::Tools::Events;

class ConstraintQueryDialog : public QDialog
{
	Q_OBJECT

public:
	ConstraintQueryDialog(const IdmContainer &container, const Entity::Property &property, QWidget *parent = 0);

	Constraint::Holder takeConstraint(BaseConstraint *parent);
    virtual void accept();

private Q_SLOTS:
	void chooseValue();
    void updateValue(const QString &text);
    void updateValue(const EntityValue::Holder &value);
    void updateValue(int index);

private:
    void setEditFont(bool italic);

private:
    typedef KeyboardEventSource<
                EventSourceBase<
                    QLineEdit
                >
            > LineEdit;
    typedef KeyboardEventHandler<
                EventHandlerBase<
                    ConstraintQueryDialog
                >
            > LineEditHandler;

private:
	IdmContainer m_container;
	const Entity::Property &m_property;
	EntityValue::Holder m_value;
	LineEditHandler m_handler;
	QLabel m_label;
	QComboBox m_operator;
	LineEdit m_edit;
	QPushButton m_choose;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
};

#endif /* IDM_CONSTRAINTQUERYDIALOG_H_ */
