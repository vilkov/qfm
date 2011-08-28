#ifndef ENTITIESLISTDELEGATE_H_
#define ENTITIESLISTDELEGATE_H_

#include <QtGui/QStyledItemDelegate>
#include "../../../containeres/idmcontainer.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesListDelegate : public QStyledItemDelegate
{
	Q_DISABLE_COPY(EntitiesListDelegate)

public:
	EntitiesListDelegate(const IdmContainer *container, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    const IdmContainer *container() const { return m_container; }

private:
    const IdmContainer *m_container;
};

IDM_PLUGIN_NS_END

#endif /* ENTITIESLISTDELEGATE_H_ */
