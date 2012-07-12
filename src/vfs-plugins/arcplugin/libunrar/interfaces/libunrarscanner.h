#ifndef LIBUNRARSCANNER_H_
#define LIBUNRARSCANNER_H_

#include <QtCore/QMutex>
#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../libunrar_ns.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class Scanner : public IFileContainerScanner
{
	Q_DECLARE_TR_FUNCTIONS(Scanner)

public:
	Scanner(const IFileContainer *container, const QByteArray &filePath);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;

	static QString errorDescription(int code);

private:
	const IFileContainer *m_container;
	QByteArray m_filePath;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARSCANNER_H_ */
