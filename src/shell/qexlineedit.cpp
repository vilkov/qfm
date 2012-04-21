#include "qexlineedit.h"
#include "qexlineedit_p.h"

#include <QTimer>
#include <QClipboard>
#include <QStyleOption>
#include <QToolTip>
#include <QApplication>

#include <QStringListModel>
#include <QCompleter>
#include <QKeyEvent>
#include <QMouseEvent>


class QExLineEditPrivate
{
public:
    QExLineEditPrivate(QExLineEdit* q) :
    	clickInClear(false),
    	clickInLeft(false),
    	clearButton(0),
    	leftButton(0),
        wideEnoughForClear(true),
        hideOnEscape(false),
        hideOnMouseOut(false),
        completer(0),
        model(0),
        maintainHistory(false),
        historyPos(0),
        q(q)
    {}
    
    ~QExLineEditPrivate()
    {}
    
    bool clickInClear : 1;
    bool clickInLeft : 1;
    QExLineEditButton *clearButton;
    QExLineEditButton *leftButton;
    bool wideEnoughForClear : 1;
    
    QPixmap pxm;
    QPixmap lpxm;
    
	bool hideOnEscape;
	bool hideOnMouseOut;
    
	QCompleter *completer;
	QStringListModel *model;
    
	int prefixStart;
	int prefixLen;
    
    bool maintainHistory;
    QStringList history;
    QString currentText;
    int historyPos;
    
    QExLineEdit *q;
};


QExLineEdit::QExLineEdit(QWidget *parent) :
	QLineEdit(parent),
	d(new QExLineEditPrivate(this))
{
    setClearButtonShown(false);
    
    connect(this, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(textEdited(const QString &)));
}

QExLineEdit::QExLineEdit(const QPixmap &clearPxm, QWidget *parent ) :
	QLineEdit(parent),
	d(new QExLineEditPrivate(this))
{
    d->pxm = clearPxm.scaled(16, 16);
    setClearButtonShown(!d->pxm.isNull());
    
    connect(this, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(textEdited(const QString &)));
}

QExLineEdit::~QExLineEdit()
{
	delete d;
}

bool QExLineEdit::isClearButtonShown() const
{
    return d->clearButton != 0;
}

void QExLineEdit::setClearButtonShown(bool show)
{
    if (show)
    {
        if (d->clearButton)
        	return;
        
        d->clearButton = new QExLineEditButton(this);
        d->clearButton->setCursor(Qt::ArrowCursor);
        d->clearButton->setToolTip("Clear text");
        
        updateClearButtonIcon(text());
        updateClearButton();
        connect(this, SIGNAL(textChanged(QString)), this, SLOT(updateClearButtonIcon(QString)));
    }
    else
    {
        disconnect(this, SIGNAL(textChanged(QString)), this, SLOT(updateClearButtonIcon(QString)));
        delete d->clearButton;
        d->clearButton = 0;
        d->clickInClear = false;
    }
}

void QExLineEdit::setPixmap(const QPixmap &pxm)
{
    d->lpxm = pxm.scaled(16, 16);

    if (d->lpxm.isNull())
    {
        if (d->leftButton)
        {
            delete d->leftButton;
            d->leftButton = 0;
            d->clickInLeft = false;
            setStyleSheet(QString::fromLatin1(":enabled { margin-left: 0; }"));
        }
    }
    else
    {
        d->leftButton = new QExLineEditButton(this);
        d->leftButton->setCursor(Qt::ArrowCursor);
        d->leftButton->setPixmap(d->lpxm);
        d->leftButton->animateVisible(true);
        d->leftButton->setVisible(true);
    }

    updateLeftButton();
}

void QExLineEdit::setCompleterStrings(const QStringList &list)
{
    if (list.isEmpty())
    {
        if (d->model)
        {
        	delete d->model;
        	d->model = 0;
        }

        if (d->completer)
        {
        	delete d->completer;
        	d->completer = 0;
        }

        disconnect(this, SIGNAL(textEdited(const QString &)), this, SLOT(checkCompletion(const QString &)));
    }
    else
    {
        if (!d->model)
        	d->model = new QStringListModel(this);

        if (!d->completer)
        {
            d->completer = new QCompleter (d->model, this);
            d->completer->setWidget(this);
            //d->completer->setCompletionMode(QCompleter::InlineCompletion);

            connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(checkCompletion(const QString &)));
            connect(d->completer, SIGNAL(highlighted(const QString &)), this, SLOT(highlighted(const QString &)));
        }

        d->model->setStringList(list);
    }
}

void QExLineEdit::setCompleterPrefixes(int start, int len)
{
    d->prefixStart = start;
	d->prefixLen = len;
}

void QExLineEdit::setHideOnEscape(bool hide)
{
	d->hideOnEscape = hide;
}

//void QexLineEdit::setHideOnMouseOut(bool hide)
//{
//	d->hideOnMouseOut = hide;
//}

void QExLineEdit::setMaintainHistory(bool maintain)
{
    if (d->maintainHistory != maintain)
		if (!(d->maintainHistory = maintain))
			d->history.clear();
}

QCompleter *QExLineEdit::completer () const
{
	return d->completer;
}

void QExLineEdit::resizeEvent(QResizeEvent * ev)
{
    updateClearButton();
    updateLeftButton();
    QLineEdit::resizeEvent(ev);
}

void QExLineEdit::mousePressEvent(QMouseEvent* e)
{
    if ((e->button() == Qt::LeftButton || e->button() == Qt::MidButton))
    {
        if (d->clearButton)
        	d->clickInClear = d->clearButton->underMouse();

        if (d->leftButton)
        	d->clickInLeft = d->leftButton->underMouse();

        if (d->clickInClear || d->clickInLeft)
        	return;
    }
    
	if (d->hideOnMouseOut && !rect().contains(e->pos()))
		hide();
    
    QLineEdit::mousePressEvent(e);
}

void QExLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    if (d->clickInClear)
    {
        if (d->clearButton->underMouse())
        {
            QString newText;

            if (event->button() == Qt::MidButton)
            {
                newText = QApplication::clipboard()->text(QClipboard::Selection);
                setText(newText);
            }
            else
            {
            	clear();
//                setSelection(0, text().size());
//                del();
                emit clearButtonClicked();
            }

            emit textChanged(newText);
        }
        
        d->clickInClear = false;
        event->accept();
        return;
    }
    
    if (d->clickInLeft && d->leftButton->underMouse())
    {
        emit pixmapClicked();
        return;
    }
    
    QLineEdit::mouseReleaseEvent(event);
}

void QExLineEdit::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() == Qt::NoModifier)
		switch (event->key())
		{
			case Qt::Key_Escape:
			{
			    if (d->hideOnEscape)
			        if (d->completer && d->completer->popup()->isVisible())
			            d->completer->popup()->hide();
			        else
			            hide();

	    		event->accept();
			    return;
			}
			case Qt::Key_Return:
			case Qt::Key_Enter:
			{
			    if (d->completer && d->completer->popup()->isVisible())
			        d->completer->popup()->hide();

	    		event->accept();
			    return;
			}
			case Qt::Key_Up:
			{
		    	if (d->maintainHistory && d->historyPos > 0)
		    		setText(d->history[--d->historyPos]);

	    		event->accept();
			    return;
			}
			case Qt::Key_Down:
			{
	    		if (d->maintainHistory)
	    			if (d->historyPos < d->history.count() - 1)
	    				setText(d->history[++d->historyPos]);
	    			else
	    				if (d->historyPos == d->history.count() - 1)
	    					setText(d->currentText);

	    		event->accept();
			    return;
			}
			default:
				break;
		}

	QLineEdit::keyPressEvent(event);
}

void QExLineEdit::showEvent(QShowEvent *e)
{
	QLineEdit::showEvent(e);
	
	if (d->hideOnMouseOut)
		grabMouse();
	
	clear();
	setFocus();
}

void QExLineEdit::hideEvent(QHideEvent *e)
{
	if (d->hideOnMouseOut)
		releaseMouse();
    
    QLineEdit::hideEvent(e);
}

void QExLineEdit::checkCompletion(const QString &text)
{
	qint32 cur = cursorPosition();
	qint32 pos = cur - 1;
	
	while (pos >= 0)
		if (text[pos] == ' ')
			break;
		else
			--pos;
    ++pos;
    
	if (pos == cur)
	{
		d->completer->popup()->hide();
		return;
	}
	
	d->completer->setCompletionPrefix(text.mid(pos, cur - pos));
	d->completer->complete();
	d->prefixStart = pos;
	d->prefixLen = cur - pos;
}

void QExLineEdit::returnPressed()
{
    if (d->maintainHistory && (d->history.isEmpty() || d->history.last() != text()))
    {
        d->history << text();
        d->historyPos = d->history.count();
        d->currentText = QString();
        selectAll();
    }
}

void QExLineEdit::textEdited(const QString &text)
{
    if (d->maintainHistory)
        d->currentText = text;
}

void QExLineEdit::highlighted(const QString &completion)
{
	QString text = this->text();
	text.replace(d->prefixStart, d->prefixLen, completion);
	setText(text);
	d->prefixLen = completion.length();
}

void QExLineEdit::updateClearButtonIcon(const QString& text)
{
    if (!d->clearButton || isReadOnly())
        return;

    if (d->wideEnoughForClear && text.length() > 0)
        d->clearButton->animateVisible(true);
    else
        d->clearButton->animateVisible(false);

    if (!d->clearButton->pixmap().isNull())
        return;

    d->clearButton->setPixmap(d->pxm);
    d->clearButton->setVisible(!text.isEmpty());
}

void QExLineEdit::updateClearButton()
{
    if (!d->clearButton || isReadOnly())
        return;

    const QSize geom = size();
    const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth,0,this);
    const int buttonWidth = d->clearButton->sizeHint().width();
    const QSize newButtonSize(buttonWidth, geom.height());
    const QFontMetrics fm(font());
    const int em = fm.width("m");

    // make sure we have enough room for the clear button
    // no point in showing it if we can't also see a few characters as well
    const bool wideEnough = geom.width() > 4 * em + buttonWidth + frameWidth;

    if (newButtonSize != d->clearButton->size())
        d->clearButton->resize(newButtonSize);

    d->clearButton->move(geom.width() - frameWidth - buttonWidth - 1, 0);

    if (wideEnough != d->wideEnoughForClear)
    {
        // we may (or may not) have been showing the button, but now our
        // positiong on that matter has shifted, so let's ensure that it
        // is properly visible (or not)
        d->wideEnoughForClear = wideEnough;
        updateClearButtonIcon(text());
    }

    setStyleSheet(QString::fromLatin1(":enabled { padding-right: %1; }").arg(wideEnough ? buttonWidth : 0));
}

void QExLineEdit::updateLeftButton()
{
    if (!d->leftButton)
    	return;

    const QSize geom = size();
    const int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth,0,this);
    const int buttonWidth = d->leftButton->sizeHint().width();
    const QSize newButtonSize(buttonWidth, geom.height());
    const QFontMetrics fm(font());
    const int em = fm.width("m");

    // make sure we have enough room for the button
    // no point in showing it if we can't also see a few characters as well
    const bool wideEnough = geom.width() > 4 * em + buttonWidth + frameWidth;

    if (newButtonSize != d->leftButton->size())
        d->leftButton->resize(newButtonSize);

    d->leftButton->move(frameWidth, 0);
    setStyleSheet(QString::fromLatin1(":enabled { margin-left: %1; }").arg(wideEnough ? buttonWidth + frameWidth : 0));
}
