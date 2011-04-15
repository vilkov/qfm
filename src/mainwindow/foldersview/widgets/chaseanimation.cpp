#include "chaseanimation.h"
#include <QtGui/QApplication>


ChaseAnimation::ChaseAnimation() :
	m_segment(0),
	m_delay(100),
	m_step(40)
{}

void ChaseAnimation::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    const int extent = qMin(option.rect.width() - 8, option.rect.height() - 8);
    const int displ = extent / 4;
    const int ext = extent / 4 - 1;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::gray);
    painter->translate(option.rect.width() / 2, option.rect.height() / 2);

    for (qint32 segment = 0; segment < segmentCount(); ++segment)
    {
        painter->rotate(QApplication::isRightToLeft() ? m_step : -m_step);
        painter->setBrush(colorForSegment(segment));
        painter->drawEllipse(QRect(displ, -ext / 2, ext, ext));
    }
}

void ChaseAnimation::timer()
{
	++m_segment;
}

qint32 ChaseAnimation::segmentCount() const
{
    return 360 / m_step;
}

QColor ChaseAnimation::colorForSegment(qint32 seg) const
{
    int index = ((seg + m_segment) % segmentCount());
    int comp = qMax(0, 255 - (index * (255 / segmentCount())));
    return QColor(comp, comp, comp, 255);
}
