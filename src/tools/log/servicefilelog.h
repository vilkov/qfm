#ifndef SERVICEFILELOG_H_
#define SERVICEFILELOG_H_

#include <time.h>
#include <fstream>
#include <stdarg.h>
#include <string.h>
#include <boost/thread/mutex.hpp>
#include "../pstrings/pstring.h"


class ServiceFileLog
{
public:
	ServiceFileLog(const PString &fileName);

	ServiceFileLog &operator<<(const PString &string)
	{
		boost::unique_lock<boost::mutex> locker(m_mutex);

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
		boost::unique_lock<boost::mutex> locker(m_mutex);

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
	boost::mutex m_mutex;
	std::ofstream m_file;
	PString m_lastError;
	static ServiceFileLog *m_instance;
};

#endif /* SERVICEFILELOG_H_ */
