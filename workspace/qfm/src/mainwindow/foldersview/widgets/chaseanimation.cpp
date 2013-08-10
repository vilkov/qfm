/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
