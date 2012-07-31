#include "../defaultplugin.h"
#include "../nodes/defaultnode.h"
#include "../interfaces/defaultfilecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class LocalRootFolderNode;
static LocalRootFolderNode *rootNode = NULL;


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

	Node *viewChild(const Uri::Iterator &path, QModelIndex &selected)
	{
		return Node::viewChild(path, selected);
	}

private:
	QString m_path;
};


Plugin::Plugin(::Tools::Settings::Option *parentOption) :
	m_settings(parentOption)
{
	Q_ASSERT(rootNode == NULL);
}

Plugin::~Plugin()
{
	Q_ASSERT_X(rootNode == NULL, "Plugin::~Plugin", "Bad links counting!");
}

void Plugin::registered()
{}

::Tools::Settings::Page *Plugin::settings()
{
	return &m_settings;
}

QString Plugin::shema() const
{
	return QString::fromLatin1("file");
}

::VFS::Node *Plugin::open(const Uri::Iterator &path, QModelIndex &selected) const
{
	if ((*path) == QLatin1String("/"))
		if (rootNode)
			return rootNode->viewChild(++path, selected);
		else
		{
			IFileContainer::Holder container(new FileContainer(QByteArray()));
			rootNode = new LocalRootFolderNode(container);

			return rootNode->viewChild(++path, selected);
		}
	else
		return NULL;
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeId type;
	type.mime = QString::fromLatin1("inode/directory");

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
