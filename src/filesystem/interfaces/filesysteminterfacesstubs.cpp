#include "filesysteminode.h"
#include "filesystemiplugin.h"
#include "filesysteminodeview.h"
#include "filesystemifileinfo.h"
#include "filesystemifiletype.h"
#include "filesystemifilecontainer.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"
#include "filesystemifilecontainerscanner.h"
#include "filesystemisettings.h"


FILE_SYSTEM_NS_BEGIN

INode::~INode()
{}

IPlugin::~IPlugin()
{}

INodeView::~INodeView()
{}

IFileInfo::~IFileInfo()
{}

IFileType::~IFileType()
{}

IFileContainer::~IFileContainer()
{}

IFileContainer::Filter::~Filter()
{}

IFileOperations::~IFileOperations()
{}

IFileNavigation::~IFileNavigation()
{}

IFileContainerScanner::~IFileContainerScanner()
{}

IFileContainerScanner::IEnumerator::~IEnumerator()
{}

ISettings::~ISettings()
{}

FILE_SYSTEM_NS_END
