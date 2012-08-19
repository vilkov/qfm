#ifndef SETTINGSDECORATIONTEXT_H_
#define SETTINGSDECORATIONTEXT_H_

#include <QtCore/QString>
#include "settingsdecoration.h"


SETTINGS_NS_BEGIN

class DecorationText : public Decoration
{
public:
	DecorationText(const QString &text);

	virtual QWidget *createEditor();

private:
	QString m_text;
};

SETTINGS_NS_END

#endif /* SETTINGSDECORATIONTEXT_H_ */
