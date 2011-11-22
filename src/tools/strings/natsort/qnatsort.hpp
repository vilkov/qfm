#ifndef __QNATSORT_HPP__
#define __QNATSORT_HPP__

#include <QString>

/**
* The two methods can be used in qSort to sort strings which contain
* numbers in natural order.
*
* Normally strings are ordered like this: fam10g fam1g fam2g fam3g
* natural ordered it would look like this: fam1g fam2g fam3g fam10g
*
* Code:
*
* QStringList list;
* list << "fam10g" << "fam1g" << "fam2g" << "fam5g";
*
* qSort( list.begin(), list.end(), caseSensitiveNaturalOderLessThen );
*/

/**
* Returns whether the left string is lesser than the right string
* in natural order.
*/
bool caseSensitiveNaturalOrderLessThen (const QString &left, const QString &right);

/**
* Returns whether the left string is lesser than the right string
* in natural order and case insensitive.
*/
bool caseInsensitiveNaturalOrderLessThen (const QString &left, const QString &right);

#endif
