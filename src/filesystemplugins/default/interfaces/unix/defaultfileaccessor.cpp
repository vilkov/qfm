#include "../defaultfileaccessor.h"

#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


DEFAULT_PLUGIN_NS_BEGIN

FileAccesor::FileAccesor(const QString &absoluteFilePath, int flags) :
	m_file(::open(absoluteFilePath.toUtf8(),
				  open_flags[flags & ReadOnly]  |
				  open_flags[flags & ReadWrite] |
				  open_flags[flags & Open]      |
				  open_flags[flags & Create]    |
				  open_flags[flags & Truncate]))
{
	Q_ASSERT(open_flags[ReadOnly] == O_RDONLY);
	Q_ASSERT(open_flags[ReadWrite] == O_RDWR);
	Q_ASSERT(open_flags[Open] == (O_APPEND | O_EXCL));
	Q_ASSERT(open_flags[Create] == O_CREAT);
	Q_ASSERT(open_flags[Truncate] == O_TRUNC);

	Q_ASSERT(permissions_flags[UserRead] == S_IRUSR);
	Q_ASSERT(permissions_flags[UserWrite] == S_IWUSR);
	Q_ASSERT(permissions_flags[UserExec] == S_IXUSR);
	Q_ASSERT(permissions_flags[GroupRead] == S_IRGRP);
	Q_ASSERT(permissions_flags[GroupWrite] == S_IWGRP);
	Q_ASSERT(permissions_flags[GroupExec] == S_IXGRP);
	Q_ASSERT(permissions_flags[OthersRead] == S_IROTH);
	Q_ASSERT(permissions_flags[OthersWrite] == S_IWOTH);
	Q_ASSERT(permissions_flags[OthersExec] == S_IXOTH);

	Q_ASSERT(flags_permissions[S_IRUSR] == UserRead);
	Q_ASSERT(flags_permissions[S_IWUSR] == UserWrite);
	Q_ASSERT(flags_permissions[S_IXUSR] == UserExec);
	Q_ASSERT(flags_permissions[S_IRGRP] == GroupRead);
	Q_ASSERT(flags_permissions[S_IWGRP] == GroupWrite);
	Q_ASSERT(flags_permissions[S_IXGRP] == GroupExec);
	Q_ASSERT(flags_permissions[S_IROTH] == OthersRead);
	Q_ASSERT(flags_permissions[S_IWOTH] == OthersWrite);
	Q_ASSERT(flags_permissions[S_IXOTH] == OthersExec);
}

FileAccesor::~FileAccesor()
{
	if (m_file != -1)
		::close(m_file);
}

bool FileAccesor::isValid() const
{
	return m_file != -1;
}

QString FileAccesor::lastError() const
{
	return QString::fromUtf8(::strerror(errno));
}

int FileAccesor::permissions() const
{
	struct stat st;

	if (::fstat(m_file, &st) == 0)
		return
				flags_permissions[st.st_mode & S_IRUSR] |
				flags_permissions[st.st_mode & S_IWUSR] |
				flags_permissions[st.st_mode & S_IXUSR] |
				flags_permissions[st.st_mode & S_IRGRP] |
				flags_permissions[st.st_mode & S_IWGRP] |
				flags_permissions[st.st_mode & S_IXGRP] |
				flags_permissions[st.st_mode & S_IROTH] |
				flags_permissions[st.st_mode & S_IWOTH] |
				flags_permissions[st.st_mode & S_IXOTH];
	else
		return 0;
}

FileAccesor::size_type FileAccesor::size()
{
	struct stat st;

	if (::fstat(m_file, &st) == 0)
		return st.st_size;
	else
		return 0;
}

bool FileAccesor::setPermissions(int mode)
{
	return ::fchmod(m_file,
			permissions_flags[mode & UserRead]    |
			permissions_flags[mode & UserWrite]   |
			permissions_flags[mode & UserExec]    |
			permissions_flags[mode & GroupRead]   |
			permissions_flags[mode & GroupWrite]  |
			permissions_flags[mode & GroupExec]   |
			permissions_flags[mode & OthersRead]  |
			permissions_flags[mode & OthersWrite] |
			permissions_flags[mode & OthersExec]) == 0;
}

FileAccesor::size_type FileAccesor::read(value_type *data, size_type size)
{
	return ::read(m_file, data, size);
}

FileAccesor::size_type FileAccesor::write(const value_type *data, size_type size)
{
	return ::write(m_file, data, size);
}

FileAccesor::size_type FileAccesor::seek(size_type offset, Seek from)
{
	return ::lseek(m_file, offset, seek_flags[from]);
}

DEFAULT_PLUGIN_NS_END
