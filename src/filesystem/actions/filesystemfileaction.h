#ifndef FILESYSTEMFILEACTION_H_
#define FILESYSTEMFILEACTION_H_

#include <QtCore/QList>
#include <QtGui/QAction>
#include "../model/items/filesystemnodeitem.h"
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class FileAction
{
public:
	typedef QPair<const NodeItem *, const IFileInfo *> FileItem;
	typedef QList<FileItem>                            FilesList;

	class FilesListAdapter
	{
	public:
		typedef FilesList              base_class;
		typedef const NodeItem *       value_type;
		typedef base_class::size_type  size_type;

	public:
		FilesListAdapter(const FilesList &filesList) :
			m_filesList(filesList)
		{}

		const value_type &operator[](int index) const { return m_filesList[index].first; }
		const value_type &at(int index) const { return m_filesList.at(index).first; }
		int size() const { return m_filesList.size(); }

	private:
		const FilesList &m_filesList;
	};

public:
	FileAction(const QIcon &icon, const QString &text);
	virtual ~FileAction();

	const QAction *action() const { return &m_action; }

	static const FileAction *fromQAction(const QAction *action);
	static FileAction *fromQAction(QAction *action);

	virtual bool isAsynchronous() const = 0;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEACTION_H_ */
