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
#include "exceptionshandler.h"
#include <assert.h>


static ExceptionHandler *global_instance = 0;

ExceptionHandler::ExceptionHandler()
{
	global_instance = this;
}

ExceptionHandler::~ExceptionHandler()
{

}

#if defined(Q_CC_MSVC) && defined(USE_SEH_EXCEPTIONS)
	int ExceptionHandler::exception(const char *where, unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		return ExceptionHandler::instance()->handleException(where);

		if (code == EXCEPTION_ACCESS_VIOLATION)
			return EXCEPTION_EXECUTE_HANDLER;    // ���� ������
		else
			return EXCEPTION_CONTINUE_SEARCH;    // ���� ��� ������
	}
	int ExceptionHandler::exception(const QString &where, unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		return ExceptionHandler::instance()->handleException(where);

		if (code == EXCEPTION_ACCESS_VIOLATION)
			return EXCEPTION_EXECUTE_HANDLER;    // ���� ������
		else
			return EXCEPTION_CONTINUE_SEARCH;    // ���� ��� ������
	}
#else
	void ExceptionHandler::exception(const char *where)
	{
		ExceptionHandler::instance()->handleException(where);
	}
	void ExceptionHandler::exception(const QString &message)
	{
		ExceptionHandler::instance()->handleException(message);
	}
	void ExceptionHandler::exception(const char *where, const char *what)
	{
		ExceptionHandler::instance()->handleException(where, what);
	}
#endif

ExceptionHandler *ExceptionHandler::instance()
{
	assert(global_instance);
	return global_instance;
}
