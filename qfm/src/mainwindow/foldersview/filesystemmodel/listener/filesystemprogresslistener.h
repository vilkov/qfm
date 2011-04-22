#ifndef FILESYSTEMPROGRESSLISTENER_H_
#define FILESYSTEMPROGRESSLISTENER_H_

#include <QtCore/QObject>
#include <QtCore/QEvent>


class FileSystemProgressListener : public QObject
{
private:
	friend class InitEvent;
	friend class UpdateEvent;
	enum
	{
		Init =  QEvent::User + 1,
		Update = QEvent::User + 2
	};

public:
	class InitEvent : public QEvent
	{
	public:
		InitEvent(qint32 maximum) :
			QEvent(static_cast<QEvent::Type>(Init)),
			m_maximum(maximum)
		{}

		qint32 maximum() const { return m_maximum; }

	private:
		qint32 m_maximum;
	};

	class UpdateEvent : public QEvent
	{
	public:
		UpdateEvent(qint32 progress) :
			QEvent(static_cast<QEvent::Type>(Update)),
			m_progress(progress)
		{}

		qint32 progress() const { return m_progress; }

	private:
		qint32 m_progress;
	};

public:
	FileSystemProgressListener(QObject *parent = 0);

    virtual bool event(QEvent *e);

private:
};

#endif /* FILESYSTEMPROGRESSLISTENER_H_ */
