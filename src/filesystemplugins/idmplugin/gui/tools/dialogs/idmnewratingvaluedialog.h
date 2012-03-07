#ifndef IDMNEWRATINGVALUEDIALOG_H_
#define IDMNEWRATINGVALUEDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QVBoxLayout>
#include <QtGui/QRadioButton>
#include <QtGui/QDialogButtonBox>
#include "../../../storage/values/idmentityvalue.h"


using namespace FileSystem::Plugins::Idm;

class NewRatingValueDialog : public QDialog
{
	Q_OBJECT

public:
	NewRatingValueDialog(const QString &title, QWidget *parent = 0);

    int value() const { return m_result; }

private Q_SLOTS:
	void button1Click(bool checked);
	void button2Click(bool checked);
	void button3Click(bool checked);
	void button4Click(bool checked);
	void button5Click(bool checked);

private:
	int m_result;
    QRadioButton m_button1;
    QRadioButton m_button2;
    QRadioButton m_button3;
    QRadioButton m_button4;
    QRadioButton m_button5;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* IDMNEWRATINGVALUEDIALOG_H_ */
