#include "servicefilelog.h"


ServiceFileLog *ServiceFileLog::m_instance = 0;

ServiceFileLog::ServiceFileLog(const PString &fileName) :
	m_file(fileName.data(), std::ios_base::out | std::ios_base::app | std::ios_base::binary)
{
	if ((m_file.rdstate() & std::ofstream::failbit) != 0)
		m_lastError = PString("Error opening log file.");

	m_instance = this;
}
