#ifndef SETTINGSTAB_H_
#define SETTINGSTAB_H_

#include <QtGui/QWidget>
#include <QtCore/QByteArray>
#include "options/settingsoption.h"


SETTINGS_NS_BEGIN

class Tab
{
public:
	virtual ~Tab();

	virtual const QByteArray &id() const = 0;
	virtual const QString &name() const = 0;
	virtual QWidget *centralWidget() const = 0;

protected:
	typedef QList<Option *> List;

private:
	List m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSTAB_H_ */
