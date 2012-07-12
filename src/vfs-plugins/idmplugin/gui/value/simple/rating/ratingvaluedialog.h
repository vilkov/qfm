#ifndef RATINGVALUEDIALOG_H_
#define RATINGVALUEDIALOG_H_

#include "widgets/ratingvaluewidget.h"
#include "../../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


class RatingValueDialog : public NestedPlainDialog
{
public:
	RatingValueDialog(const QString &title, QWidget *parent = 0);

    int value() const { return m_mainWidget.value(); }

private:
    MainRatingValueWidget m_mainWidget;
};

#endif /* RATINGVALUEDIALOG_H_ */
