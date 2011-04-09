#include "filesystemtree.h"


FileSystemTree::FileSystemTree(const QString &directory, FileSystemItem *parent) :
	FileSystemItem(parent),
	m_fileInfo(directory),
	m_parentEntry(0)
{
	if (!m_fileInfo.isRoot())
		m_childs.push_back(new FileSystemRoot(this));
}

FileSystemTree::FileSystemTree(const QFileInfo &info, FileSystemItem *parent) :
	FileSystemItem(parent),
	m_fileInfo(info),
	m_parentEntry(0)
{
	if (!m_fileInfo.isRoot())
		m_childs.push_back(new FileSystemRoot(this));
}

FileSystemTree::~FileSystemTree()
{
	for (value_type::size_type i = 0, size = m_childs.size(); i < size; ++i)
	{
		delete m_childs.at(i).entry;
		delete m_childs.at(i).subtree;
	}
}

FileSystemTree::size_type FileSystemTree::indexOf(FileSystemItem *item) const
{
	for (value_type::size_type i = 0, size = m_childs.size(); i < size; ++i)
		if (m_childs.at(i).entry == item)
			return static_cast<size_type>(i);

	return InvalidIndex;
}

FileSystemItem *FileSystemTree::subtree(FileSystemItem *entry) const
{
	Q_ASSERT(indexOf(entry) != InvalidIndex);
	return m_childs.at(indexOf(entry)).subtree;
}

void FileSystemTree::setSubtree(FileSystemItem *entry, FileSystemItem *tree)
{
	Q_ASSERT(indexOf(entry) != InvalidIndex);
	FileSystemPair &pair = m_childs[indexOf(entry)];
	delete pair.subtree;
	pair.subtree = tree;
}

void FileSystemTree::setSubtree(FileSystemTree *tree)
{
	Q_ASSERT(!m_childs.isEmpty());
	FileSystemPair &pair = m_childs.last();
	delete pair.subtree;
	pair.subtree = tree;
	tree->setParent(this);
}

ChangesList FileSystemTree::makeChangeSet() const
{
	ChangesList list;

	if (m_fileInfo.isRoot())
	{
		list.reserve(m_childs.size());

		for (size_type i = 0, size = m_childs.size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, m_childs.at(i).entry));
	}
	else
	{
		list.reserve(m_childs.size() - 1);

		for (size_type i = 1, size = m_childs.size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, m_childs.at(i).entry));
	}

	return list;
}
