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
		ItemPage(Page *page);
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
