#include "../folderfileaccessor.h"

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define UNDEFINED 0


DEFAULT_PLUGIN_NS_BEGIN

static const int open_flags[FileAccesor::Truncate + 1] =
{
		UNDEFINED, O_RDONLY,  O_RDWR,    UNDEFINED,
		O_APPEND,  UNDEFINED, UNDEFINED, UNDEFINED,
		O_CREAT,   UNDEFINED, UNDEFINED, UNDEFINED,
		UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED,
		O_TRUNC
};


FileAccesor::FileAccesor(const QString &absoluteFilePath, int mode) :
	m_file(::open(absoluteFilePath.toUtf8(),
				  open_flags[mode & ReadOnly]  |
				  open_flags[mode & ReadWrite] |
				  open_flags[mode & Open]      |
				  open_flags[mode & Create]    |
				  open_flags[mode & Truncate]))
{}

FileAccesor::~FileAccesor()
{
	if (m_file != -1)
		::close(m_file);
}

bool FileAccesor::isValid() const
{
	return m_file != -1;
}

const QString &FileAccesor::lastError() const
{
	return m_lastError = QString::fromUtf8(::strerror(errno));
}

FileAccesor::size_type FileAccesor::size()
{
	struct stat st;

	if (::fstat(m_file, &st) == 0)
		return st.st_size;
	else
		return 0;
}

bool FileAccesor::seek(size_type offset)
{
	return ((off_t)offset) == ::lseek(m_file, offset, SEEK_SET);
}

FileAccesor::size_type FileAccesor::read(value_type *data, size_type size)
{
	return ::read(m_file, data, size);
}

FileAccesor::size_type FileAccesor::write(const value_type *data, size_type size)
{
	return ::write(m_file, data, size);
}

DEFAULT_PLUGIN_NS_END
