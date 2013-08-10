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
#ifndef SETTINGSDIALOGMODEL_H_
#define SETTINGSDIALOGMODEL_H_

#include "../../options/gui/page/settingspage.h"
#include "../../../models/tree/treemodel.h"


SETTINGS_NS_BEGIN

class Model : public Tools::Models::Tree::Model
{
public:
	typedef QList<Page *> Pages;

public:
	Model(const Pages &pages, QObject *parent = 0);

	Page *pageAt(const QModelIndex &index) const;

private:
	class ItemPage : public Tools::Models::Tree::Item
	{
	public:
		typedef QList<ItemPage *> Container;

	public:
		ItemPage(Page *page, Base *parent = NULL);
		virtual ~ItemPage();

		virtual Base *at(size_type index) const;
		virtual size_type size() const;
		virtual size_type indexOf(Base *item) const;

		Page *page() const { return m_page; }

		virtual QVariant data(qint32 column, qint32 role) const;

	private:
		Page *m_page;
		Container m_subpages;
	};

	class Container : public Tools::Models::Tree::Model::Container
	{
	public:
		Container(const Pages &pages);
		virtual ~Container();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		typedef QList<Item *> List;
		List m_items;
	};

private:
	Container m_container;
};

SETTINGS_NS_END

#endif /* SETTINGSDIALOGMODEL_H_ */
