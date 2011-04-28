#ifndef COPYPROGRESSROUTINE_WIN_H_
#define COPYPROGRESSROUTINE_WIN_H_

#include <QtCore/QFile>
#include <QtCore/qt_windows.h>
#define COPY_FILE_OPEN_SOURCE_FOR_WRITE 0x00000004


class CopyProgressRoutine
{
public:
	CopyProgressRoutine(PerformCopyEntryTask *instance, const volatile bool &stopedFlag) :
		m_instance(instance),
		m_stopedFlag(stopedFlag),
		m_prevTotalBytesTransferred(0)
	{
		Q_ASSERT(m_instance != 0);
	}

	bool copy(QFile &source, QFile &dest)
	{
		return CopyFileEx(
				(const wchar_t *)source.fileName().utf16(),
				(const wchar_t *)dest.fileName().utf16(),
				progress,
				this,
				NULL,
				COPY_FILE_OPEN_SOURCE_FOR_WRITE);
	}

	QString lastError() const { return qt_error_string(GetLastError()); }

private:
	static DWORD WINAPI progress(
			LARGE_INTEGER TotalFileSize,
			LARGE_INTEGER TotalBytesTransferred,
			LARGE_INTEGER StreamSize,
			LARGE_INTEGER StreamBytesTransferred,
			DWORD dwStreamNumber,
			DWORD dwCallbackReason,
			HANDLE hSourceFile,
			HANDLE hDestinationFile,
			LPVOID lpData)
	{
		CopyProgressRoutine *self = static_cast<CopyProgressRoutine*>(lpData);

		if (self->m_stopedFlag ||
			self->m_instance->m_canceled ||
			self->m_instance->isControllerDead())
			return PROGRESS_CANCEL;
		else
		{
			self->m_instance->m_progress.update(TotalBytesTransferred.QuadPart - self->m_prevTotalBytesTransferred);
			self->m_prevTotalBytesTransferred = TotalBytesTransferred.QuadPart;
			return PROGRESS_CONTINUE;
		}
	}

private:
	PerformCopyEntryTask *m_instance;
	const volatile bool &m_stopedFlag;
	LONGLONG m_prevTotalBytesTransferred;
};

#endif /* COPYPROGRESSROUTINE_WIN_H_ */
