#ifndef SETTINGSVISIBLEOPTIONFONT_H_
#define SETTINGSVISIBLEOPTIONFONT_H_

#include "settingsivisibleoption.h"
#include "../value/settingsoptionfont.h"
#include "../../../events/imp/mouseeventhandler.h"


class QLabel;


SETTINGS_NS_BEGIN

class VisibleOptioinFont : public OptionFont, public IVisibleOption
{
public:
	VisibleOptioinFont(const QString &label, const QString &id, Option *parent, const QFont &defaultValue);

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
	virtual bool chooseFont(QFont &font) const = 0;
	virtual void applyFont(const QFont &font) = 0;

private:
	typedef Events::MouseReleaseEventHandler<
				Events::EventHandlerBase<
					VisibleOptioinFont
				>
			> EventHandler;
	void chooseFontEvent();

private:
	EventHandler m_eventHandler;
	QString m_label;
	QFont m_currentFont;
	QLabel *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONFONT_H_ */
