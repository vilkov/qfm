#include "arcarchive.h"
#include "../arcplugin.h"


ARC_PLUGIN_NS_BEGIN

Archive::~Archive()
{}

const Archive *Archive::archive(const QString &fileName, State **state)
{
	for (const Archive **arc = Plugin::archivers(); *arc; ++arc)
		if (((*state) = (*arc)->beginRead(fileName))->error.isEmpty())
			return (*arc);
		else
			(*arc)->endRead((*state));

	return 0;
}

ARC_PLUGIN_NS_END
