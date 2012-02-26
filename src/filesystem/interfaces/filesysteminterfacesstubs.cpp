#include "filesysteminode.h"
#include "filesystemiplugin.h"
#include "filesysteminodeview.h"
#include "filesystemifiletype.h"
#include "filesystemifileaccessor.h"
#include "filesystemifilecontainer.h"
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

IFileContainer::~IFileContainer()
{}

IFileOperations::~IFileOperations()
{}

IFileNavigation::~IFileNavigation()
{}

FILE_SYSTEM_NS_END
