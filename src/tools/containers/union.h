#ifndef UNION_H_
#define UNION_H_

#include <QtCore/QList>
#include "containers_ns.h"


CONTAINERS_NS_BEGIN

typedef qint32 Dot;

class Line
{
public:
	Line()
	{}
	Line(Dot dot) :
		m_top(dot),
		m_bottom(dot)
	{}
	Line(Dot top, Dot bottom) :
		m_top(top),
		m_bottom(bottom)
	{}

	Dot top() const { return m_top; }
	Dot bottom() const { return m_bottom; }

protected:
	friend class Union;
	void setTop(Dot dot) { m_top = dot; }
	void setBottom(Dot dot) { m_bottom = dot; }

private:
	Dot m_top;
	Dot m_bottom;
};


class Union
{
public:
	typedef qint32       distance_type;
	typedef QList<Line> List;

public:
	Union(distance_type distance = 1) :
		m_distance(distance)
	{}

	List::size_type size() const { return m_ranges.size(); }
	const Line &at(List::size_type index) const { return m_ranges.at(index); }

	void add(Dot dot)
	{
		for (List::size_type index1 = 0, size = m_ranges.size(); index1 < size; ++index1)
			if (dot < m_ranges.at(index1).top())
				if (m_ranges.at(index1).top() - m_distance <= dot)
				{
					if (index1 > 0 && m_ranges.at(index1 - 1).bottom() + m_distance >= dot)
					{
						m_ranges[index1].setTop(m_ranges.at(index1 - 1).top());
						m_ranges.removeAt(index1 - 1);
					}
					else
						m_ranges[index1].setTop(dot);

					return;
				}
				else
				{
					if (index1 > 0 && m_ranges.at(index1 - 1).bottom() + m_distance >= dot)
						m_ranges[index1 - 1].setBottom(dot);
					else
						m_ranges.insert(index1, Line(dot));

					return;
				}
			else
				if (dot <= m_ranges.at(index1).bottom())
					return;
				else
					if (m_ranges.at(index1).bottom() + m_distance >= dot)
					{
						if (index1 + 1 < size && m_ranges.at(index1 + 1).top() - m_distance <= dot)
						{
							m_ranges[index1].setBottom(m_ranges.at(index1 + 1).bottom());
							m_ranges.removeAt(index1 + 1);
						}
						else
							m_ranges[index1].setBottom(dot);

						return;
					}
					else
						if (index1 + 1 < size && m_ranges.at(index1 + 1).top() - m_distance <= dot)
						{
							m_ranges[index1 + 1].setTop(dot);
							return;
						}

		m_ranges.push_back(Line(dot));
	}

	void add(Dot top, Dot bottom)
	{
		qint32 index1 = 0;

		for (List::size_type size = m_ranges.size(); index1 < size; ++index1)
			if (top < m_ranges.at(index1).top())
				if (index1 > 0 && top - m_distance <= m_ranges.at(index1 - 1).bottom())
				{
					qint32 index2 = m_ranges.size() - 1;

					for (; index2 >= index1; --index2)
						if (bottom > m_ranges.at(index2).bottom())
							break;

					if (index2 < index1)
					{
						m_ranges[index1 - 1].setBottom(m_ranges.at(index1).bottom());
						m_ranges.removeAt(index1);
					}
					else
					{
						if (index2 + 1 < m_ranges.size() && bottom + m_distance >= m_ranges.at(index2 + 1).top())
							m_ranges[index1 - 1].setBottom(m_ranges[++index2].bottom());
						else
							m_ranges[index1 - 1].setBottom(bottom);

						for (List::size_type i = index2 - index1 + 1; i > 0; --i)
							m_ranges.removeAt(index1);
					}

					return;
				}
				else
					break;

		if (index1 == m_ranges.size())
			if (m_ranges.isEmpty())
				m_ranges.push_back(Line(top, bottom));
			else
				if (top - m_distance <= m_ranges.last().bottom())
				{
					if (bottom > m_ranges.last().bottom())
						m_ranges.last().setBottom(bottom);
				}
				else
					m_ranges.push_back(Line(top, bottom));
		else
		{
			qint32 index2 = m_ranges.size() - 1;

			for (; index2 >= index1; --index2)
				if (bottom > m_ranges.at(index2).bottom())
					break;

			if (index2 < index1)
				if (bottom + m_distance >= m_ranges.at(index1).top())
					m_ranges[index1].setTop(top);
				else
					m_ranges.insert(index1, Line(top, bottom));
			else
			{
				if (index2 + 1 < m_ranges.size() && bottom + m_distance >= m_ranges.at(index2 + 1).top())
					bottom = m_ranges[++index2].bottom();

				for (List::size_type i = index2 - index1 + 1; i > 0; --i)
					m_ranges.removeAt(index1);

				m_ranges.insert(index1, Line(top, bottom));
			}
		}
	}

private:
	List m_ranges;
	distance_type m_distance;
};

CONTAINERS_NS_END

#endif /* UNION_H_ */
