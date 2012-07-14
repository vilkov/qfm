#include "shellline.h"

#include <QDir>
#include <QProcess>
#include <QCompleter>
#include <QAbstractItemView>


ShellLine::ShellLine(QWidget *parent) :
	QExLineEdit(QPixmap(QString::fromLatin1(":/icons/clear.png")), parent)
{
    setHideOnEscape(true);
    //setHideOnMouseOut(true);
    setMaintainHistory(true);
    
    setPixmap(QPixmap(QString::fromLatin1(":/icons/shell.png")));
}

void ShellLine::setCurrentPath(const QString &path, bool showHidden)
{
    m_currentPath = path;
    
    QDir::Filters filter = QDir::AllEntries | QDir::NoDotAndDotDot;
    if (showHidden)
    	filter |= QDir::Hidden;
    
//    QStringList files = QDir(path).entryList(filter, QDir::Name | QDir::IgnoreCase);

#ifdef Q_OS_UNIX
//    for (int i = 0; i < files.count(); ++i)
//    {
//        QString &file = files[i];
//        file.replace('(', "\\(");
//        file.replace(')', "\\)");
//        file.replace('[', "\\[");
//        file.replace(']', "\\]");
//        file.replace('{', "\\{");
//        file.replace('}', "\\}");
//        file.replace(' ', "\\ ");
//    }
#endif
    
//    setCompleterStrings(files /*<< SystemUtils::commands()*/);
}

void ShellLine::checkCompletion(const QString &text)
{
	qint32 cur = cursorPosition();
	qint32 pos = cur - 1;
    
    while (pos >= 0)
    {
//        const QChar &c = text.at(pos);
//
//        if (pos - 1 >= 0 && ((c == '/' && text.at(pos - 1) == '.') || (c == ' ' && text.at(pos - 1) != '\\')))
//            break;
//        else
//        	--pos;
    }
    ++pos;
    
    if (pos == cur)
    {
//        completer()->popup()->hide();
        return;
    }
    
//    completer()->setCompletionPrefix(text.mid(pos, cur - pos));
//    completer()->complete();
    setCompleterPrefixes(pos, cur - pos);
}

void ShellLine::returnPressed()
{
#ifdef Q_OS_UNIX
//    QProcess::startDetached("bash", QStringList() << "-c" << text(), m_currentPath);
#endif
    hide();
    QExLineEdit::returnPressed();
}
