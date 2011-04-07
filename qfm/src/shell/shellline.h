#ifndef SHELLLINE_H_
#define SHELLLINE_H_

#include "qexlineedit.h"


class ShellLine : public QExLineEdit
{
    Q_OBJECT

public:
    ShellLine (QWidget *parent = 0);

    void setCurrentPath(const QString &path, bool showHidden);

protected:
    virtual void checkCompletion(const QString &text);
    virtual void returnPressed();

private:
    QString m_currentPath;
    QCompleter *m_completer;
    qint32 m_completionPos;
    qint32 m_completionLen;
};

#endif /* SHELLLINE_H_ */
