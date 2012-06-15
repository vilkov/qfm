#ifndef SETTINGSSCOPE_H_
#define SETTINGSSCOPE_H_

#include "settingslistoption.h"


SETTINGS_NS_BEGIN

class Scope : public ListOption
{
public:
	Scope(const QString &id, Option *parent = 0) :
		ListOption(parent),
		m_id(id)
	{}
	virtual ~Scope();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);

protected:
	QString m_id;
};

SETTINGS_NS_END

#endif /* SETTINGSSCOPE_H_ */
