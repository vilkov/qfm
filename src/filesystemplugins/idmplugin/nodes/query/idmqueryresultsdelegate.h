#ifndef IDMQUERYRESULTSDELEGATE_H_
#define IDMQUERYRESULTSDELEGATE_H_

#include "../../model/idmdelegate.h"
#include "../../containeres/idmcontainer.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryResultsDelegate : public IdmDelegate
{
	Q_DISABLE_COPY(IdmQueryResultsDelegate)

public:
	IdmQueryResultsDelegate(const IdmContainer &container, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private:
    IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSDELEGATE_H_ */
