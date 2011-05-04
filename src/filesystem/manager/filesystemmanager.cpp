#include "filesystemmanager.h"
#include <stlsoft/string/string_view.hpp>
#include <stlsoft/string/simple_string.hpp>
#include <stlsoft/string/string_tokeniser.hpp>


FileSystemManager::FileSystemManager()
{

}

FileSystemManager::~FileSystemManager()
{
	qDeleteAll(m_staticPlugins);
}

FileSystemTree *FileSystemManager::parse(const QString &absoluteFilePath) const
{
	typedef stlsoft::string_tokeniser<QString, QChar> tokeniser;
}
