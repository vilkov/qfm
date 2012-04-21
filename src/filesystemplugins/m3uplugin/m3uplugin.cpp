#include "m3uplugin.h"
#include "m3unode.h"
#include <QtCore/QTextCodec>


M3U_PLUGIN_NS_BEGIN

Plugin::Plugin() :
	m_identity(QString::fromLatin1("#EXTM3U"))
{}

void Plugin::registered()
{

}

Node *Plugin::node(const IFileContainer *container, const IFileInfo *file, Node *parent) const
{
	return 0;
}

//Node *M3uPlugin::node(const IFileInfo *info, IFile *file, Node *parent) const
//{
//	if (QTextCodec *codec = QTextCodec::codecForName("UTF-8"))
//	{
//		uchar data[64] = {};
//
//		for (IFile::size_type i = 0, size = file->read(data, 64); size > m_identity.size(); ++i)
//			if (data[i] == '\n' || data[i] == '\r')
//			{
//				data[i] = 0;
//
//				if (m_identity == codec->toUnicode((const char*)data, i))
//					return new M3uNode(info->absoluteFilePath(), parent);
//				else
//					break;
//			}
//	}
//
//	return 0;
//}

M3U_PLUGIN_NS_END
