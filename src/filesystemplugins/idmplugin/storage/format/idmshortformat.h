#ifndef IDMSHORTFORMAT_H_
#define IDMSHORTFORMAT_H_

#include <QtCore/QString>
#include "../../idmplugin_ns.h"
#include "../../../../tools/containers/hashedlist.h"


IDM_PLUGIN_NS_BEGIN

class IdmShortFormat
{
public:
	typedef HashedList<QString, QString> Container;
	typedef int                          size_type;

public:
	IdmShortFormat(const QString &format);

	bool isValid() const;
	const QString &format() const { return m_format; }
	const QString &lastError() const { return m_lastError; }

private:
	void dollarToken(size_type &pos, const QString &source);
	void nameToken(size_type &pos, const QString &source);

private:
	QString m_format;
	Container m_items;
	QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDMSHORTFORMAT_H_ */
