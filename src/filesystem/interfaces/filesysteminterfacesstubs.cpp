#include "filesysteminode.h"
#include "filesystemiplugin.h"
#include "filesysteminodeview.h"
#include "filesystemifiletype.h"
#include "filesystemifileaccessor.h"
#include "filesystemifilelocation.h"
#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

INode::~INode()
{}

IPlugin::~IPlugin()
{}

INodeView::~INodeView()
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

FILE_SYSTEM_NS_END
