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
#ifndef SERVICEFILELOG_H_
#define SERVICEFILELOG_H_

#include <time.h>
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include "../threads/pmutex.h"
#include "../pstrings/pstring.h"


/*
 * TODO:
 *     MSVC: __FUNCTION__, __FUNCDNAME__, __FUNCSIG__
 *     GCC: __func__, __FUNCTION__, __PRETTY_FUNCTION__
 *     __FILE__ __LINE__
 *
 */

class ServiceFileLog
{
public:
	ServiceFileLog(const PString &fileName);

	ServiceFileLog &operator<<(const PString &string)
	{
		PMutexLocker locker(m_mutex);

		printTime();

		if (checkLastOperation())
		{
			m_file << string.c_str() << std::endl;

			if (checkLastOperation())
				m_file.flush();
		}

		return *this;
	}

	void print(const PString &format, ...)
	{
		PMutexLocker locker(m_mutex);

		printTime();

		if (checkLastOperation())
		{
			char buffer[1024];
			va_list args;

			va_start(args, format);
			vsprintf(buffer, format.c_str(), args);
			m_file << buffer << std::endl;
			va_end(args);

			if (checkLastOperation())
				m_file.flush();
		}
	}

	bool isValid() { return m_file.good(); }
	const PString &lastError() const { return m_lastError; }
	static ServiceFileLog *instance() { return m_instance; }

private:
	bool checkLastOperation()
	{
		if ((m_file.rdstate() & std::ofstream::failbit) != 0)
		{
			m_lastError = PString("Write to log failed because of an error related to the internal logic of the operation itself.");
			return false;
		}
		else
			if ((m_file.rdstate() & std::ofstream::badbit) != 0)
			{
				m_lastError = PString("Write to log failed because of the failure of an input/output operation on the stream buffer.");
				return false;
			}

		return true;
	}
	void printTime()
	{
		time_t rawtime;
		tm *timeinfo;

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		char *buffer = asctime(timeinfo);
		buffer[strlen(buffer) - 1] = 0;
		m_file << buffer << ": ";
	}

private:
	PMutex m_mutex;
	std::ofstream m_file;
	PString m_lastError;
	static ServiceFileLog *m_instance;
};

#endif /* SERVICEFILELOG_H_ */
