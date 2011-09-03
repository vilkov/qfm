#ifndef NEWPROPERTYNAME_H_
#define NEWPROPERTYNAME_H_

#include <QtCore/QList>
#include "../../storage/entities/idmentity.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"


using namespace FileSystem::Plugins::Idm;

class NewPropertyNameDialog : public StringDialog
{
    Q_OBJECT

public:
    typedef QList<IdmEntity::Property> Properties;

public:
    NewPropertyNameDialog(const Properties &properties, QWidget *parent = 0);

protected:
    virtual bool check();

private:
    const Properties &m_properties;
};

#endif /* NEWPROPERTYNAME_H_ */
