#ifndef RANGEINTERSECTION_H_
#define RANGEINTERSECTION_H_

#include <QtCore/QList>


class RangeIntersection
{
public:
	typedef qint32     value_type;
	typedef value_type distance_type;

public:
	class Range
	{
	public:
		Range()
		{}
		Range(value_type top, value_type bottom) :
			m_top(top),
			m_bottom(bottom)
		{}

		value_type top() const { return m_top; }
		value_type &top() { return m_top; }

		value_type bottom() const { return m_bottom; }
		value_type &bottom() { return m_bottom; }

	private:
		value_type m_top;
		value_type m_bottom;
	};

public:
	typedef QList<Range> RangeList;

public:
	RangeIntersection(distance_type maxDistance = 1) :
		m_maxDistance(maxDistance)
	{}

	RangeList::size_type size() const { return m_ranges.size(); }
	const Range &at(RangeList::size_type index) const { return m_ranges.at(index); }

	void add(value_type top, value_type bottom)
	{
		qint32 index1 = 0;

		for (RangeList::size_type size = m_ranges.size(); index1 < size; ++index1)
			if (top < m_ranges.at(index1).top())
				if (index1 > 0 && top - m_maxDistance <= m_ranges.at(index1 - 1).bottom())
				{
					qint32 index2 = m_ranges.size() - 1;

					for (; index2 >= index1; --index2)
						if (bottom > m_ranges.at(index2).bottom())
							break;

					if (index2 < index1)
					{
						m_ranges[index1 - 1].bottom() = m_ranges.at(index1).bottom();
						m_ranges.removeAt(index1);
					}
					else
					{
						if (index2 + 1 < m_ranges.size() && bottom + m_maxDistance >= m_ranges.at(index2 + 1).top())
							m_ranges[index1 - 1].bottom() = m_ranges[++index2].bottom();
						else
							m_ranges[index1 - 1].bottom() = bottom;

						for (RangeList::size_type i = index2 - index1 + 1; i > 0; --i)
							m_ranges.removeAt(index1);
					}

					return;
				}
				else
					break;

		if (index1 == m_ranges.size())
			if (m_ranges.isEmpty())
				m_ranges.push_back(Range(top, bottom));
			else
				if (top - m_maxDistance <= m_ranges.last().bottom())
				{
					if (bottom > m_ranges.last().bottom())
						m_ranges.last().bottom() = bottom;
				}
				else
					m_ranges.push_back(Range(top, bottom));
		else
		{
			qint32 index2 = m_ranges.size() - 1;

			for (; index2 >= index1; --index2)
				if (bottom > m_ranges.at(index2).bottom())
					break;

			if (index2 < index1)
				if (bottom + m_maxDistance >= m_ranges.at(index1).top())
					m_ranges[index1].top() = top;
				else
					m_ranges.insert(index1, Range(top, bottom));
			else
			{
				if (index2 + 1 < m_ranges.size() && bottom + m_maxDistance >= m_ranges.at(index2 + 1).top())
					bottom = m_ranges[++index2].bottom();

				for (RangeList::size_type i = index2 - index1 + 1; i > 0; --i)
					m_ranges.removeAt(index1);

				m_ranges.insert(index1, Range(top, bottom));
			}
		}
	}

private:
	RangeList m_ranges;
	distance_type m_maxDistance;
};

#endif /* RANGEINTERSECTION_H_ */
