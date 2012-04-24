#include "filesysteminode.h"
#include "filesystemiplugin.h"
#include "filesysteminodeview.h"
#include "filesystemifileinfo.h"
#include "filesystemifiletype.h"
#include "filesystemifileaccessor.h"
#include "filesystemifilelocation.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"
#include "filesystemifilecontainerscanner.h"


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

IFileAccessor::~IFileAccessor()
{}

IFileLocation::~IFileLocation()
{}

IFileOperations::~IFileOperations()
{}

IFileNavigation::~IFileNavigation()
{}

IFileContainerScanner::~IFileContainerScanner()
{}

IFileContainerScanner::Enumerator::~Enumerator()
{}

FILE_SYSTEM_NS_END
