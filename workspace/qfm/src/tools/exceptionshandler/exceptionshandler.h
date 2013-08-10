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
#ifndef EXCEPTIONSHANDLER_H_
#define EXCEPTIONSHANDLER_H_

#include <QtCore/QString>


class ExceptionHandler
{
public:
	ExceptionHandler();
	virtual ~ExceptionHandler();

#if defined(Q_CC_MSVC) && defined(USE_SEH_EXCEPTIONS)
	static int exception(const char *where, unsigned int code, struct _EXCEPTION_POINTERS *ep);
	static int exception(const QString &where, unsigned int code, struct _EXCEPTION_POINTERS *ep);
#else
	static void exception(const char *where);
	static void exception(const QString &message);
	static void exception(const char *where, const char *what);
#endif

protected:
#ifdef USE_SEH_EXCEPTIONS
	virtual int handleException(const char *where) = 0;
	virtual int handleException(const QString &message) = 0;
#else
	virtual void handleException(const char *where) = 0;
	virtual void handleException(const QString &message) = 0;
	virtual void handleException(const char *where, const char *what) = 0;
#endif

private:
	static ExceptionHandler *instance();
};


#ifdef Q_OS_WIN
#	if defined(Q_CC_MSVC) && defined(USE_SEH_EXCEPTIONS)
#		define TRY __try
#		define CATCH(A) __except(EXCEPTION_EXECUTE_HANDLER)
#		define CATCH_ALL(where, execute_this_code) __except(ExceptionHandler::exception(where, GetExceptionCode(), GetExceptionInformation())) { execute_this_code }
#	else
#		define TRY try
#		define CATCH(A) catch (A)
#		define CATCH_ALL(where, execute_this_code) catch (...) { ExceptionHandler::exception(where); execute_this_code }
#	endif
#elif defined(QT_TRY)
#	define TRY QT_TRY
#	define CATCH(A) QT_CATCH(A)
#	define CATCH_ALL(where, execute_this_code) QT_CATCH(...) { ExceptionHandler::exception(where); execute_this_code }
#else
#	define TRY try
#	define CATCH(A) catch(A)
#	define CATCH_ALL(where, execute_this_code) \
		catch(const std::exception &__error__) { ExceptionHandler::exception(where, __error__.what()); execute_this_code } \
		catch(...) { ExceptionHandler::exception(where); execute_this_code }
#endif

#define DO_NOTHING {}

#endif /* EXCEPTIONSHANDLER_H_ */
