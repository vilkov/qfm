#ifndef CHOOSEDIALOG_H_
#define CHOOSEDIALOG_H_

#include <QtCore/QList>
#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QDialogButtonBox>


class ChooseDialog : public QDialog
{
    Q_OBJECT

public:
    typedef QList<QPair<QString, void*> > List;

public:
    ChooseDialog(const QString &title, const List &values, QWidget *parent = 0);

    void *value() const;
    virtual void accept();

private:
    bool check();

private:
    typedef QList<QPair<void*, QRadioButton*> > Container;

private:
    Container m_values;
    QVBoxLayout m_layout;
    QDialogButtonBox m_buttonBox;
};

#endif /* CHOOSEDIALOG_H_ */
