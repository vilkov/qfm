#include "libunrarscanner.h"

#include <QtCore/QDebug>
#include <libunrar/rar.hpp>
#include <libunrar/dll.hpp>


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

Scanner::Scanner(const IFileContainer *container, IFileAccessor::Holder &file) :
	m_container(container),
	m_file(file.take())
{}

Scanner::~Scanner()
{}

void Scanner::enumerate(IEnumerator::Holder &enumerator) const
{

}

IFileInfo *Scanner::info(const QString &fileName, QString &error) const
{
	return NULL;
}

void Scanner::scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	QByteArray fileName(m_container->location().toUtf8());

	struct RAROpenArchiveDataEx archiveData;
	memset(&archiveData, 0, sizeof(struct RAROpenArchiveDataEx));

	archiveData.ArcName = fileName.data();
	archiveData.OpenMode = RAR_OM_LIST;

	if (void *archive = RAROpenArchiveEx(&archiveData))
	{
		struct RARHeaderDataEx fileInfo;
		int res;

		while ((res = RARReadHeaderEx(archive, &fileInfo)) == 0)
		{
			qDebug() << QString::fromUtf8(fileInfo.ArcName);
		}

		RARCloseArchive(archive);
	}
}

void Scanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{

}

LIBUNRAR_ARC_PLUGIN_NS_END
