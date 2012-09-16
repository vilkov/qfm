#ifndef IDM_NEWPROPERTYNAME_H_
#define IDM_NEWPROPERTYNAME_H_

#include <QtCore/QList>
#include "../../storage/entities/idm_entity.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"


using namespace ::VFS::Plugins::Idm;

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

#endif /* IDM_NEWPROPERTYNAME_H_ */
