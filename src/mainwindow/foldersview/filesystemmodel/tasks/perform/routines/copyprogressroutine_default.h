#ifndef COPYPROGRESSROUTINE_DEFAULT_H_
#define COPYPROGRESSROUTINE_DEFAULT_H_

#include <QtCore/QFile>


class CopyProgressRoutine
{
public:
	CopyProgressRoutine(PerformCopyEntryTask *instance, const volatile bool &stopedFlag) :
		m_instance(instance),
		m_stopedFlag(stopedFlag)
	{
		Q_ASSERT(m_instance != 0);
	}

	bool copy(QFile &source, QFile &dest)
	{
		if (dest.exists())
			if (dest.remove())
				if (source.copy(dest.fileName()))
				{
					if (dest.open(QFile::ReadOnly))
					{
						m_instance->m_progress.update(dest.size());
						dest.close();
					}

					return true;
				}
				else
					m_lastError = source.errorString();
			else
				m_lastError = dest.errorString();
		else
			if (source.copy(dest.fileName()))
			{
				if (dest.open(QFile::ReadOnly))
				{
					m_instance->m_progress.update(dest.size());
					dest.close();
				}

				return true;
			}
			else
				m_lastError = source.errorString();

		return false;
	}

	QString lastError() const { return m_lastError; }

private:
	PerformCopyEntryTask *m_instance;
	const volatile bool &m_stopedFlag;
	QString m_lastError;
};

#endif /* COPYPROGRESSROUTINE_DEFAULT_H_ */
