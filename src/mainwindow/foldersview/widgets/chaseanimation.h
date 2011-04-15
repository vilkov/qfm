#ifndef CHASEANIMATION_H_
#define CHASEANIMATION_H_

#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtCore/QModelIndex>
#include <QtGui/QStyleOptionViewItem>


class ChaseAnimation
{
public:
	ChaseAnimation();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index);
    void timer();

private:
    qint32 segmentCount() const;
    QColor colorForSegment(qint32 segment) const;

private:
	QPainter *m_painter;
	qint32 m_segment;
	qint32 m_delay;
	qint32 m_step;
};

#endif /* CHASEANIMATION_H_ */
