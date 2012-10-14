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
#include "arc_libunrar_unpackintosubdiraction.h"
#include "tasks/arc_libunrar_unpackintosubdiractiontask.h"

#include <desktop/theme/desktop_theme.h>


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

UnPackIntoSubdirAction::UnPackIntoSubdirAction() :
	Action(tr("Unpacking..."), ::Desktop::Theme::current()->unpackActionIcon(), tr("Unpack into subfolder"))
{}

PerformActionTask *UnPackIntoSubdirAction::process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const
{
	return new UnPackIntoSubdirActionTask(receiver, container, files);
}

ARC_PLUGIN_LIBUNRAR_NS_END
