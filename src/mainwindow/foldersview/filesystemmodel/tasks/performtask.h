#ifndef PERFORMTASK_H_
#define PERFORMTASK_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QWaitCondition>
#include <QtGui/QMessageBox>
#include "controlabletask.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"


/********************************************************************************************************/
class PerformTask : public ControlableTask
{
public:
	struct Params : public ControlableTask::Params
	{
		QObject *receiver;
		FileSystemTree *fileSystemTree;
	};
	struct EventParams : public FileSystemModelEvent::Params
	{
		FileSystemTree *fileSystemTree;
	};

	struct QuestionAnswerParams : public FileSystemModelEvent::Params
	{
		QString question;
		int answer;
		QWaitCondition condition;
		QMessageBox::StandardButtons buttons;
	};
	typedef FileSystemModelEventTemplate<QuestionAnswerParams> QuestionAnswerEvent;

public:
	PerformTask(Params *params, QObject *controller1);
	PerformTask(Params *params, QObject *controller1, QObject *controller2);
	PerformTask(Params *params, QObject *controller1, QObject *controller2, QObject *controller3);

protected:
	inline Params *parameters() const { return static_cast<Params*>(ControlableTask::parameters()); }
};


/********************************************************************************************************/
class PerformRemoveTask : public PerformTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveTask)

public:
	struct Params : public PerformTask::Params
	{
		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	struct EventParams : public PerformTask::EventParams
	{
		FileSystemEntry *entry;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	PerformRemoveTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(PerformTask::parameters()); }

private:
	bool remove(FileSystemTree *tree, const volatile bool &stopedFlag);
};

#endif /* PERFORMTASK_H_ */
