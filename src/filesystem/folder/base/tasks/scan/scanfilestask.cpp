#include "scanfilestask.h"


FILE_SYSTEM_NS_BEGIN

ScanFilesTask::ScanFilesTask(QObject *receiver, const Info &root, const QStringList &files) :
	ScanFilesBaseTask(receiver),
	m_root(root),
	m_files(files)
{}

void ScanFilesTask::run(const volatile bool &aborted)
{
	m_subnode.reset(scan(m_root.absoluteFilePath(), m_files, aborted));
}

FILE_SYSTEM_NS_END
