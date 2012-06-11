#ifndef LIBUNRARSCANNER_H_
#define LIBUNRARSCANNER_H_

#include <QtCore/QMutex>
#include "../libunrar_ns.h"
#include "../../../../filesystem/interfaces/filesystemifilecontainer.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class Scanner : public IFileContainerScanner
{
public:
	Scanner(const IFileContainer *container, const QString &fileName);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual void enumerate(IEnumerator::Holder &enumerator) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;

private:
	const IFileContainer *m_container;
	QString m_fileName;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARSCANNER_H_ */
