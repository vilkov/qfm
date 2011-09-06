#ifndef IDMSHORTFORMAT_H_
#define IDMSHORTFORMAT_H_

#include <QtCore/QString>
#include "../../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmShortFormat
{
public:
	IdmShortFormat(const QString &format);

	bool isValid() const;
	const QString &format() const { return m_format; }
	const QString &lastError() const { return m_lastError; }


private:
	QString m_format;
	QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDMSHORTFORMAT_H_ */
