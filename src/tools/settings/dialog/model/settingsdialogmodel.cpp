#include "settingsdialogmodel.h"


SETTINGS_NS_BEGIN

Model::Model(const Pages &pages, QObject *parent) :
	Tools::Models::Tree::Model(m_container, parent),
	m_container(pages)
{}

Page *Model::pageAt(const QModelIndex &index) const
{
	return static_cast<ItemPage *>(index.internalPointer())->page();
}

Model::ItemPage::ItemPage(Page *page) :
	Tools::Models::Tree::Item(NULL),
	m_page(page)
{
	Q_ASSERT(m_page);

	const Page::Pages &subpages = m_page->subpages();
	m_subpages.reserve(subpages.size());

	for (Page::Pages::size_type i = 0, size = subpages.size(); i < size; ++i)
		m_subpages.push_back(new ItemPage(subpages.at(i)));
}

Model::ItemPage::~ItemPage()
{
	qDeleteAll(m_subpages);
}

Model::ItemPage::Base *Model::ItemPage::at(size_type index) const
{
	return m_subpages.at(index);
}

Model::ItemPage::size_type Model::ItemPage::size() const
{
	return m_subpages.size();
}

Model::ItemPage::size_type Model::ItemPage::indexOf(Base *item) const
{
	return m_subpages.indexOf(static_cast<ItemPage *>(item));
}

QVariant Model::ItemPage::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_page->title();
	else
		return QVariant();
}

Model::Container::Container(const Pages &pages)
{
	m_items.reserve(pages.size());

	for (Pages::size_type i = 0, size = pages.size(); i < size; ++i)
		m_items.push_back(new ItemPage(pages.at(i)));
}

Model::Container::~Container()
{
	qDeleteAll(m_items);
}

Model::Container::size_type Model::Container::size() const
{
	return m_items.size();
}

Model::Container::Item *Model::Container::at(size_type index) const
{
	return m_items.at(index);
}

Model::Container::size_type Model::Container::indexOf(Item *item) const
{
	return m_items.indexOf(static_cast<ItemPage *>(item));
}

SETTINGS_NS_END
