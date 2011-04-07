#ifndef EXCEPTIONSHANDLER_H_
#define EXCEPTIONSHANDLER_H_

#include <QString>
#include <QGlobalStatic>
#include "../memory/memory_manager.h"


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
#endif

protected:
#ifdef USE_SEH_EXCEPTIONS
	virtual int handleException(const char *where) = 0;
	virtual int handleException(const QString &message) = 0;
#else
	virtual void handleException(const char *where) = 0;
	virtual void handleException(const QString &message) = 0;
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
#else
#	define TRY QT_TRY
#	define CATCH(A) QT_CATCH(A)
#	define CATCH_ALL(where, execute_this_code) QT_CATCH(...) { ExceptionHandler::exception(where); execute_this_code }
#endif

#define DO_NOTHING {}

#endif /* EXCEPTIONSHANDLER_H_ */
