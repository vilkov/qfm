#ifndef QEXLINEEDIT_H_
#define QEXLINEEDIT_H_

#include <QLineEdit>


class QExLineEditPrivate;

class QExLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    QExLineEdit(QWidget *parent = 0);
    QExLineEdit(const QPixmap &clearPxm, QWidget *parent = 0);
    virtual ~QExLineEdit();

    bool isClearButtonShown() const;
    void setClearButtonShown(bool show);
    
    void setPixmap(const QPixmap &pxm);
    
    void setCompleterStrings(const QStringList &);
    void setCompleterPrefixes(int start, int len);

	void setHideOnEscape (bool hide);
	//void setHideOnMouseOut (bool hide);
    
    void setMaintainHistory(bool maintain);
    
protected:
	QCompleter *completer () const;

protected:
	virtual void resizeEvent(QResizeEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

    virtual void keyPressEvent(QKeyEvent *);
	virtual void showEvent(QShowEvent *);
	virtual void hideEvent(QHideEvent *);

Q_SIGNALS:
	void clearButtonClicked();
	void pixmapClicked();

protected Q_SLOTS:
	virtual void checkCompletion(const QString &);
    virtual void returnPressed();
    virtual void textEdited(const QString &text);

private Q_SLOTS:
	void highlighted(const QString &);
    void updateClearButtonIcon(const QString &);

private:
    void updateClearButton();
    void updateLeftButton();

private:
    friend class QExLineEditPrivate;
    QExLineEditPrivate * const d;
};

#endif /* QEXLINEEDIT_H_ */
