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
#ifndef ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_
#define ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "../arc_libarchive_ns.h"
#include "../../actions/arc_action.h"


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_ */
