#include "qnatsort.hpp"

// ***************************************
// NOTA BENE: L'HO PESANTEMENTE MODIFICATO
// ***************************************

static int compare_right (const QString &leftStr, const QString &rightStr, int pos)
{
    int bias = 0;
    
    /**
    * The longest run of digits wins.  That aside, the greatest
    * value wins, but we can't know that it will until we've scanned
    * both numbers to know that they have the same magnitude, so we
    * remember it in BIAS.
    */
    for (;; pos++) {
        if (!leftStr[pos].isDigit() && !rightStr[pos].isDigit())
            return bias;
        else if (!leftStr[pos].isDigit())
            return -1;
        else if (!rightStr[pos].isDigit())
            return +1;
        else if (leftStr[pos] < rightStr[pos]) {
            if (!bias)
                bias = -1;
        } else if (leftStr[pos] > rightStr[pos]) {
            if (!bias)
                bias = +1;
        }
        //else if (leftStr[pos].isNull() && rightStr[pos].isNull())
        //    return bias;
        
        if (pos == leftStr.count()-1 && leftStr.count() < rightStr.count())
            return -1;
        else if (pos == rightStr.count()-1 && leftStr.count() > rightStr.count())
            return +1;
        else if (pos == leftStr.count()-1 && pos == rightStr.count()-1)
            return bias;
    }
    
    return 0;
}

static int compare_left(const QString &leftStr, const QString &rightStr, int pos)
{
    /**
    * Compare two left-aligned numbers: the first to have a
    * different value wins.
    */
    for (;; pos++) {
        if (!leftStr[pos].isDigit() && !rightStr[pos].isDigit())
            return 0;
        else if (!leftStr[pos].isDigit())
            return -1;
        else if (!rightStr[pos].isDigit())
            return +1;
        else if (leftStr[pos] < rightStr[pos])
            return -1;
        else if (leftStr[pos] > rightStr[pos])
            return +1;
        
        // se sono arrivato qui le stringhe sono pari
        else if (pos == leftStr.count()-1 && leftStr.count() < rightStr.count())
            return -1;
        else if (pos == rightStr.count()-1 && leftStr.count() > rightStr.count())
            return +1;
        else if (pos == leftStr.count()-1 && pos == rightStr.count()-1)
            return 0;
    }
    
    return 0;
}

static int natural_order_compare(const QString &leftStr, const QString &rightStr, bool fold_case)
{
    // questa parte e' abbastanza inutile
    if (leftStr.isEmpty())
        return rightStr.isEmpty() ? 0 : -1;
    if (rightStr.isEmpty())
        return 1;
    
    int i;
    QChar ca, cb;
    int fractional, result;
    
    i = 0;
    
    forever {
        ca = leftStr[i];
        cb = rightStr[i];
        
        /* process run of digits */
        if (ca.isDigit() && cb.isDigit()) {
            fractional = (ca == '0' || cb == '0');
            
            if (fractional) {
                if ((result = compare_left(leftStr, rightStr, i)) != 0)
                    return result;
            } else {
                if ((result = compare_right(leftStr, rightStr, i)) != 0)
                    return result;
            }
        }
        
        if (fold_case) {
            ca = ca.toUpper();
            cb = cb.toUpper();
        }
        
        if (ca < cb)
            return -1;
        else if (ca > cb)
            return +1;
        
        ++i;
        
        if (i == leftStr.count()) {
            if (i == rightStr.count())
                return 0;
            return -1;
        }
        if (i == rightStr.count())
            return 1;
    }
}

bool caseSensitiveNaturalOrderLessThen(const QString &left, const QString &right)
{
    return (natural_order_compare(left, right, false) < 0);
}

bool caseInsensitiveNaturalOrderLessThen(const QString &left, const QString &right)
{
    return (natural_order_compare(left, right, true) < 0);
}

