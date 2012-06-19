#ifndef SETTINGSCONTAINER_H_
#define SETTINGSCONTAINER_H_

#include <QtCore/QList>
#include "options/settingslist.h"


SETTINGS_NS_BEGIN

class Container : public List
{
public:
	Container(const QString &storage);

	void save() const;
    void load();

protected:
	void manage(Option *option);

protected:
	virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
	static QString storageLocation(const QString &applicationFolder);

private:
	QString m_storage;
};

SETTINGS_NS_END

#endif /* SETTINGSCONTAINER_H_ */
