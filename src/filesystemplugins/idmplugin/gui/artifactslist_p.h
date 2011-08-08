/********************************************************************************
** Form generated from reading UI file 'artifactslist.ui'
**
** Created: Mon Aug 8 16:16:23 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ARTIFACTSLIST_H
#define ARTIFACTSLIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ArtifactsListClass
{
public:
    QGridLayout *gridLayout;
    QToolButton *createToolButton;
    QToolButton *deleteToolButton;
    QSpacerItem *horizontalSpacer;
    QTreeView *treeView;
    QToolButton *addToToolButton;
    QToolButton *delFromToolButton;

    void setupUi(QWidget *ArtifactsListClass)
    {
        if (ArtifactsListClass->objectName().isEmpty())
            ArtifactsListClass->setObjectName(QString::fromUtf8("ArtifactsListClass"));
        ArtifactsListClass->resize(472, 554);
        gridLayout = new QGridLayout(ArtifactsListClass);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        createToolButton = new QToolButton(ArtifactsListClass);
        createToolButton->setObjectName(QString::fromUtf8("createToolButton"));

        gridLayout->addWidget(createToolButton, 0, 1, 1, 1);

        deleteToolButton = new QToolButton(ArtifactsListClass);
        deleteToolButton->setObjectName(QString::fromUtf8("deleteToolButton"));

        gridLayout->addWidget(deleteToolButton, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(370, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 5, 1, 1);

        treeView = new QTreeView(ArtifactsListClass);
        treeView->setObjectName(QString::fromUtf8("treeView"));

        gridLayout->addWidget(treeView, 1, 0, 1, 6);

        addToToolButton = new QToolButton(ArtifactsListClass);
        addToToolButton->setObjectName(QString::fromUtf8("addToToolButton"));

        gridLayout->addWidget(addToToolButton, 0, 3, 1, 1);

        delFromToolButton = new QToolButton(ArtifactsListClass);
        delFromToolButton->setObjectName(QString::fromUtf8("delFromToolButton"));

        gridLayout->addWidget(delFromToolButton, 0, 4, 1, 1);


        retranslateUi(ArtifactsListClass);
        QObject::connect(createToolButton, SIGNAL(clicked()), ArtifactsListClass, SLOT(actCreate()));
        QObject::connect(deleteToolButton, SIGNAL(clicked()), ArtifactsListClass, SLOT(actDelete()));
        QObject::connect(addToToolButton, SIGNAL(clicked()), ArtifactsListClass, SLOT(addTo()));
        QObject::connect(delFromToolButton, SIGNAL(clicked()), ArtifactsListClass, SLOT(delFrom()));
        QObject::connect(treeView, SIGNAL(doubleClicked(QModelIndex)), ArtifactsListClass, SLOT(actOpen(QModelIndex)));

        QMetaObject::connectSlotsByName(ArtifactsListClass);
    } // setupUi

    void retranslateUi(QWidget *ArtifactsListClass)
    {
        ArtifactsListClass->setWindowTitle(QApplication::translate("ArtifactsListClass", "Structure artifacts", 0, QApplication::UnicodeUTF8));
        createToolButton->setText(QApplication::translate("ArtifactsListClass", "Create", 0, QApplication::UnicodeUTF8));
        deleteToolButton->setText(QApplication::translate("ArtifactsListClass", "Delete", 0, QApplication::UnicodeUTF8));
        addToToolButton->setText(QApplication::translate("ArtifactsListClass", "Add to", 0, QApplication::UnicodeUTF8));
        delFromToolButton->setText(QApplication::translate("ArtifactsListClass", "Del from", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ArtifactsListClass: public Ui_ArtifactsListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ARTIFACTSLIST_H
