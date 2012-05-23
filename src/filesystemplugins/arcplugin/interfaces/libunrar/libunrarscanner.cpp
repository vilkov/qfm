#include "libunrarscanner.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

Scanner::Scanner(const IFileContainer *container, IFileAccessor::Holder &file)
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

}

void Scanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{

}

LIBUNRAR_ARC_PLUGIN_NS_END
