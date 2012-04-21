#ifndef WORKTHREAD_H_
#define WORKTHREAD_H_

#include <QThread>
#include <QMutex>
#include <QWaitCondition>


class WorkThread : public QThread
{
	Q_OBJECT

public:
	WorkThread(QObject *parent = 0);

	volatile bool isStoped() const { return m_stoped; }
	void start(const Priority &priority = NormalPriority);
	void stop();
	void stop(bool wait);
    void terminate();

Q_SIGNALS:
	void done();
	void abortRequested();

protected:
	const volatile bool &stopedFlag() const { return m_stoped; }

protected:
	virtual void run();
	virtual void execute() = 0;

private:
	QMutex m_mutex;
	bool m_stoped;
	bool m_abortRequested;
	QWaitCondition m_condStoped;
	QWaitCondition m_condStopAccepted;
};

#endif /* WORKTHREAD_H_ */
