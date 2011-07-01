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
			return EXCEPTION_EXECUTE_HANDLER;    // если авария
		else
			return EXCEPTION_CONTINUE_SEARCH;    // если нет аварии
	}
	int ExceptionHandler::exception(const QString &where, unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		return ExceptionHandler::instance()->handleException(where);

		if (code == EXCEPTION_ACCESS_VIOLATION)
			return EXCEPTION_EXECUTE_HANDLER;    // если авария
		else
			return EXCEPTION_CONTINUE_SEARCH;    // если нет аварии
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
