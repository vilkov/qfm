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
#ifndef PASSWORDCONFIGOPTION_H_
#define PASSWORDCONFIGOPTION_H_

#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include <tools/settings/configoptions/configoptionvaluedefault.h>


class PasswordConfigOption: public ::Tools::Settings::ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(PasswordConfigOption)

public:
	PasswordConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

private:
	QLineEdit *m_editor;
};

#endif /* PASSWORDCONFIGOPTION_H_ */
