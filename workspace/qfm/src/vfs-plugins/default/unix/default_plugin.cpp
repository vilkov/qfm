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
#include "../default_plugin.h"
#include "../nodes/default_node.h"
#include "../interfaces/default_filecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class LocalRootFolderNode;
static LocalRootFolderNode *rootNode = NULL;
static const QString root(QString::fromLatin1("/"));
static Plugin *s_instance;


class LocalRootFolderNode : public BaseNode
{
public:
	LocalRootFolderNode(IFileContainer::Holder &container) :
		BaseNode(container, NULL),
		m_path(QString::fromLatin1("/"))
	{}

	virtual ~LocalRootFolderNode()
	{
		rootNode = NULL;
	}

    /* INode */
	virtual QString title() const
	{
		return m_path;
	}

	virtual QString location() const
	{
		return m_path;
	}

	Node *viewChild(const Uri::iterator &path, QModelIndex &selected)
	{
		return Node::viewChild(path, selected);
	}

private:
	QString m_path;
};


Plugin::Plugin(::Tools::Settings::Option *parentOption) :
	m_settings(parentOption)
{
    Q_ASSERT(s_instance == NULL);
	Q_ASSERT(rootNode == NULL);
    s_instance = this;
}

Plugin::~Plugin()
{
	Q_ASSERT_X(rootNode == NULL, "Plugin::~Plugin", "Bad links counting!");
}

Plugin *Plugin::instance()
{
    Q_ASSERT(s_instance);
    return s_instance;
}

void Plugin::registered()
{}

::Tools::Settings::Page *Plugin::settings()
{
	return &m_settings;
}

const Location &Plugin::schema() const
{
    static const char schema[] = "file";
    static const Location res(location(QString::fromLatin1(schema), QByteArray::fromRawData(schema, qstrlen(schema))));
	return res;
}

::VFS::Node *Plugin::open(const Uri &path, QModelIndex &selected) const
{
    Uri::iterator i(path.begin());

	if ((*i) == root)
		if (rootNode)
			return rootNode->viewChild(++i, selected);
		else
		{
			IFileContainer::Holder container(new FileContainer(QByteArray()));
			rootNode = new LocalRootFolderNode(container);

			return rootNode->viewChild(++i, selected);
		}
	else
		return NULL;
}

void Plugin::container(const Uri &path, IFileContainer::Holder &container, IFileInfo::Holder &info, QString &error) const
{
    Uri::iterator i(path.begin());

    if ((*i) == root)
    {
        QString p;
        QString res;

        if (!(++i).atEnd())
            for (p = (*i); !(++i).atEnd(); p = (*i))
                res.append(QChar(L'/')).append(p);

        if (!p.isEmpty())
        {
            container = new FileContainer(res);
            info = container->info(p, error);
        }
    }
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeId type;
	type.mime = QByteArray("inode/directory");

	return FileTypeIdList() << type;
}

::VFS::Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, ::VFS::Node *parent) const
{
	IFileContainer::Holder folder(container->open(file, m_error));

	if (folder)
		return new Node(folder, parent);
	else
		return NULL;
}

DEFAULT_PLUGIN_NS_END
