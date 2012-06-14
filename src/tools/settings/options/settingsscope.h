#ifndef SETTINGSSCOPE_H_
#define SETTINGSSCOPE_H_

#include "settingslistoption.h"


SETTINGS_NS_BEGIN

class Scope : public ListOption
{
public:
	Scope(const QString &id, Option *parent = 0) :
		ListOption(id, parent)
	{}
	virtual ~Scope();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
};

SETTINGS_NS_END

#endif /* SETTINGSSCOPE_H_ */
