#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include "../../../tasks/scan/scanfilesbasetask.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public DestControlableTask::Event
	{
	public:
		Event(Type type, bool canceled, PScopedPointer<InfoListItem> &entries) :
			DestControlableTask::Event(type, canceled),
			entries(entries.take())
		{}

		PScopedPointer<InfoListItem> entries;
	};

public:
	ScanFilesTask(QObject *receiver, const Info &root, const QStringList &files);

	virtual void run(const volatile bool &aborted);

protected:
	const PScopedPointer<InfoListItem> &subnode() const { return m_subnode; }
	PScopedPointer<InfoListItem> &subnode() { return m_subnode; }

private:
	Info m_root;
	QStringList m_files;
	PScopedPointer<InfoListItem> m_subnode;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
