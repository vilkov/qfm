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
