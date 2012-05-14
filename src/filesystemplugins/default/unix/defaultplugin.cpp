#include "../defaultplugin.h"
#include "../defaultfoldernode.h"
#include "../interfaces/defaultfilecontainer.h"


DEFAULT_PLUGIN_NS_BEGIN

class LocalRootFolderNode;
static LocalRootFolderNode *rootNode = NULL;


class LocalRootFolderNode : public FolderNodeBase
{
public:
	LocalRootFolderNode(IFileContainer::Holder &container) :
		FolderNodeBase(container, NULL),
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

	/* IFileLocation */
	virtual QString location() const
	{
		return m_path;
	}

	Node *viewChild(const Path::Iterator &path, QModelIndex &selected)
	{
		return Node::viewChild(path, selected);
	}

private:
	QString m_path;
};


Plugin::Plugin() :
	m_settings()
{
	Q_ASSERT(rootNode == NULL);

	IFileContainer::Holder container(new Plugins::Default::FileContainer(QString()));
	rootNode = new LocalRootFolderNode(container);
}

Plugin::~Plugin()
{
	Q_ASSERT_X(rootNode == NULL, "Plugin::~Plugin", "Bad links counting!");
}

void Plugin::registered()
{}

const ::Tools::Settings::Tab *Plugin::settings() const
{
	return &m_settings;
}

QString Plugin::shema() const
{
	return QString::fromLatin1("file");
}

Node *Plugin::open(const Path::Iterator &path, QModelIndex &selected) const
{
	if ((*path) == QLatin1String("/"))
		return rootNode->viewChild(++path, selected);
	else
		return NULL;
}

Plugin::FileTypeIdList Plugin::fileTypes() const
{
	FileTypeId type;
	type.mime = QString::fromLatin1("inode/directory");

	return FileTypeIdList() << type;
}

Node *Plugin::open(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	IFileContainer::Holder folder(container->open(file->fileName(), false, m_error));

	if (folder)
		return new FolderNode(folder, parent);
	else
		return NULL;
}

DEFAULT_PLUGIN_NS_END
