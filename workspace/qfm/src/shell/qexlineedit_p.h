#ifndef QEXLINEEDIT_P_H_
#define QEXLINEEDIT_P_H_

#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QTimeLine>


class QExLineEditButton : public QWidget
{
    Q_OBJECT

public:
    QExLineEditButton(QWidget *parent) :
    	QWidget(parent),
    	m_timeline(200, this)
    {
        m_timeline.setFrameRange(0, 255);
        m_timeline.setCurveShape(QTimeLine::EaseInOutCurve);
        m_timeline.setDirection(QTimeLine::Backward);
        connect(&m_timeline, SIGNAL(finished()), this, SLOT(animationFinished()));
        connect(&m_timeline, SIGNAL(frameChanged(int)), this, SLOT(update()));
    }

    virtual QSize sizeHint () const { return m_pixmap.size(); }

    void animateVisible(bool visible)
    {
        if (visible)
        {
            if (m_timeline.direction() == QTimeLine::Forward)
                return;
            
            m_timeline.setDirection(QTimeLine::Forward);
            m_timeline.setDuration(250);

            show();
        }
        else
        {
            if (m_timeline.direction() == QTimeLine::Backward)
                return;
            
            m_timeline.setDirection(QTimeLine::Backward);
            m_timeline.setDuration(250);
        }
        
        if (m_timeline.state() != QTimeLine::Running)
            m_timeline.start();
        //setVisible(m_timeline.direction() == QTimeLine::Forward);
    }

    void setPixmap(const QPixmap &p) { m_pixmap = p; }
    QPixmap pixmap() const { return m_pixmap; }

    void setAnimationsEnabled(bool animationsEnabled)
    {
        // We need to set the current time in the case that we had the clear
        // button shown, for it being painted on the paintEvent(). Otherwise
        // it wont be painted, resulting (m->timeLine->currentTime() == 0) true,
        // and therefore a bad painting. This is needed for the case that we
        // come from a non animated widget and want it animated. (ereslibre)
        if (animationsEnabled && m_timeline.direction() == QTimeLine::Forward)
            m_timeline.setCurrentTime(150);
    }

protected:
    void paintEvent(QPaintEvent *)
    {
        if (true) // animation enabled
        {
            if (m_pixmap.isNull() || m_timeline.currentTime() == 0)
                return;
            
            QPainter p(this);
            p.setOpacity(m_timeline.currentValue());
            p.drawPixmap((width() - m_pixmap.width()) / 2,
                         (height() - m_pixmap.height()) / 2,
                         m_pixmap);
        }
        else // animation disabled
        {
            QPainter p(this);
            p.setOpacity(1); // make sure
            p.drawPixmap((width() - m_pixmap.width()) / 2,
                         (height() - m_pixmap.height()) / 2,
                         m_pixmap);
        }
    }

protected Q_SLOTS:
    void animationFinished()
    {
        if (m_timeline.direction() == QTimeLine::Forward)
            update();
        else
            hide();
    }

private:
    QTimeLine m_timeline;
    QPixmap m_pixmap;
};

#endif /* QEXLINEEDIT_P_H_ */
