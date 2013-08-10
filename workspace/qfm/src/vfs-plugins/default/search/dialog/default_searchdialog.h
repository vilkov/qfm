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
#ifndef DEFAULT_SEARCHDIALOG_H_
#define DEFAULT_SEARCHDIALOG_H_

#include <QtGui/QLabel>
#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchDialog : public QDialog
{
public:
	SearchDialog(QWidget *parent = 0);

	QString pattern() const { return m_editor.text(); }

    virtual void accept();
    virtual void reject();

private:
	QVBoxLayout m_layout;
	QHBoxLayout m_hLayout;
	QLabel m_label;
	QLineEdit m_editor;
	QDialogButtonBox m_buttonBox;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_SEARCHDIALOG_H_ */
