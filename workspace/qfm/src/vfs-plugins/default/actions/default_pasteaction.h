/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DEFAULT_PASTEACTION_H_
#define DEFAULT_PASTEACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/asynchronous/vfs_asyncaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class PasteAction : public AsyncAction
{
	Q_DECLARE_TR_FUNCTIONS(PasteAction)

public:
	PasteAction();

	virtual const QString &lockReason() const;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;

private:
	QString m_lockReason;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PASTEACTION_H_ */
