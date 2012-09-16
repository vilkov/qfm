#ifndef ARC_LIBUNRAR_SCANNER_H_
#define ARC_LIBUNRAR_SCANNER_H_

#include <QtCore/QMutex>
#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../arc_libunrar_ns.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class Scanner : public IFileContainerScanner
{
	Q_DECLARE_TR_FUNCTIONS(Scanner)

public:
	Scanner(const IFileContainer *container, const QByteArray &filePath);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

	static QString errorDescription(int code);

private:
	const IFileContainer *m_container;
	QByteArray m_filePath;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

ARC_PLUGIN_LIBUNRAR_NS_END

#endif /* ARC_LIBUNRAR_SCANNER_H_ */
