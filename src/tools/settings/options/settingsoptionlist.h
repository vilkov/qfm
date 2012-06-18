#ifndef SETTINGSOPTIONSLIST_H_
#define SETTINGSOPTIONSLIST_H_

#include "settingslist.h"


SETTINGS_NS_BEGIN

class OptionList : public List
{
public:
	OptionList(const QString &id, Option *parent = 0) :
		List(parent),
		m_id(id)
	{}
	virtual ~OptionList();

	void add(Option *option) { m_items.push_back(option); }
	void remove(size_type index) { delete m_items.takeAt(index); }
	void clear();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
	virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	virtual Option *create() = 0;

protected:
	QString m_id;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONSLIST_H_ */
