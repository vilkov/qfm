#include "filesystemtaskdialog.h"
#include "filesystemuserinputdialog.h"


FILE_SYSTEM_NS_BEGIN

void QuestionEvent::showDialog(QWidget *parent)
{
	QMutexLocker lock(&m_result->m_mutex);
	m_result->m_answer = QMessageBox::question(parent, m_title, m_question, QMessageBox::StandardButtons(m_buttons));
	m_result->m_done = true;
	m_result->m_condition.wakeAll();
}

void UserInputEvent::showDialog(QWidget *parent)
{
	QMutexLocker lock(&m_result->m_mutex);
	UserInputDialog dialog(UserInputDialog::Question, m_title, m_question, m_buttons, parent);

	dialog.exec();
	m_result->m_answer = dialog.answer();
	m_result->m_value = dialog.value();
	m_result->m_done = true;
	m_result->m_condition.wakeAll();
}

FILE_SYSTEM_NS_END
