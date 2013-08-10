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
#include "servicefilelog.h"


TOOLS_NS_BEGIN

ServiceFileLog *ServiceFileLog::m_instance = 0;

ServiceFileLog::ServiceFileLog(const PString &fileName) :
	m_file(fileName.data(), std::ios_base::out | std::ios_base::app | std::ios_base::binary)
{
	if ((m_file.rdstate() & std::ofstream::failbit) != 0)
		m_lastError = PString("Error opening log file.");

	m_instance = this;
}

TOOLS_NS_END
