#ifndef SETTINGSSCOPE_H_
#define SETTINGSSCOPE_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class Scope : public List
{
public:
	Scope(const QString &id, Option *parent) :
		List(parent),
		m_id(id)
	{}

	void manage(Option *option);

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	QString m_id;
};

SETTINGS_NS_END

#endif /* SETTINGSSCOPE_H_ */
