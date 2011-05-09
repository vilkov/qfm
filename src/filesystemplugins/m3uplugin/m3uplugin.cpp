#include "m3uplugin.h"
#include "m3unode.h"
#include <QtCore/QBuffer>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>


M3uPlugin::M3uPlugin() :
	m_identity(QString::fromLatin1("#EXTM3U"))
{}

FileSystem::Node *M3uPlugin::node(const FileSystem::Info &info, File file, size_type size, FileSystem::Node *parent) const
{
	QByteArray data = QByteArray::fromRawData((const char*)file, size);
	QBuffer buffer(&data);

	if (buffer.open(QBuffer::ReadOnly))
	{
		QTextStream stream(&buffer);

		stream.setCodec(QTextCodec::codecForName("UTF-8"));

		if (stream.readLine() == m_identity)
			return new M3uNode(info, parent);
	}

	return 0;
}
