#ifndef ARTIFACTSDELEGATE_H_
#define ARTIFACTSDELEGATE_H_

#include <QItemDelegate>
#include <QModelIndex>
#include <QWidget>
#include <QDateTime>
#include <QVariant>
#include "artifactssingleitem.h"
#include "artifactsvirtualitem.h"
#include "../../data/editordata.h"
#include "../../../../artifact/artifact.h"
#include "../../../../instance/artifactinstance.h"
#include "../../../../artifactmanager.h"


class ArtifactEdit;

class ArtifactsDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ArtifactsDelegate(EditorData &data, QObject *parent = 0);

    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    inline QWidget *parentWidget() const { return static_cast<QWidget*>(QItemDelegate::parent()); }
    void updateInstance(const QModelIndex &index, const QVariant &value) const;
    template <typename T> T data(const QModelIndex &index) const;

private:
    EditorData &m_internal;
};

#endif /* ARTIFACTSDELEGATE_H_ */
