#include "filesystemscanedfiles.h"


FILE_SYSTEM_NS_BEGIN

ScanedFiles::ScanedFiles() :
	m_data()
{}

ScanedFiles::ScanedFiles(List::size_type reserver) :
	m_data(new Data())
{
	m_data->list.reserve(reserver);
}

ScanedFiles::Data::Data() :
	totalSize(0)
{}

ScanedFiles::Data::~Data()
{
	for (List::size_type i = 0, size = list.size(); i < size; ++i)
		delete list.at(i).second;
}

FILE_SYSTEM_NS_END
