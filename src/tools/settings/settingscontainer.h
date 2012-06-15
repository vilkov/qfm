#ifndef SETTINGSCONTAINER_H_
#define SETTINGSCONTAINER_H_

#include <QtCore/QList>
#include "options/settingslistoption.h"


SETTINGS_NS_BEGIN

class Container : public ListOption
{
public:
	Container(const QString &storage);
	virtual ~Container();

	void save() const;
    void load();

protected:
	virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);

protected:
	static QString storageLocation(const QString &applicationFolder);

private:
	QString m_storage;
};

SETTINGS_NS_END

#endif /* SETTINGSCONTAINER_H_ */
