#ifndef VALUEDIALOGMETAFUNCTIONS_H_
#define VALUEDIALOGMETAFUNCTIONS_H_

#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QDateTimeEdit>


/* Select editor meta-function */
template <typename Value> struct Editor {};

template <> struct Editor<QString>        { typedef QLineEdit     type; };
template <> struct Editor<char>           { typedef QSpinBox      type; };
template <> struct Editor<unsigned char>  { typedef QSpinBox      type; };
template <> struct Editor<short>          { typedef QSpinBox      type; };
template <> struct Editor<unsigned short> { typedef QSpinBox      type; };
template <> struct Editor<int>            { typedef QSpinBox      type; };
template <> struct Editor<unsigned int>   { typedef QSpinBox      type; };
template <> struct Editor<long>           { typedef QSpinBox      type; };
template <> struct Editor<unsigned long>  { typedef QSpinBox      type; };
template <> struct Editor<QDate>          { typedef QDateEdit     type; };
template <> struct Editor<QTime>          { typedef QTimeEdit     type; };
template <> struct Editor<QDateTime>      { typedef QDateTimeEdit type; };


/* Editor value meta-function */
template <typename Editor> struct EditorValue {};

template <> struct EditorValue<typename Editor<QString>::type>
{
	static QString value(const typename Editor<QString>::type &editor)
	{
		return editor.text().simplified();
	}
	static void setValue(typename Editor<QString>::type &editor, const QString &value)
	{
		editor.setText(value);
	}
};

template <> struct EditorValue<typename Editor<char>::type>
{
	static char value(const typename Editor<char>::type &editor)
	{
		return editor.value();
	}
	static void setValue(typename Editor<char>::type &editor, int value)
	{
		editor.setValue(value);
	}
};

template <> struct EditorValue<typename Editor<QDate>::type>
{
	static QDate value(const typename Editor<QDate>::type &editor)
	{
		return editor.date();
	}
	static void setValue(typename Editor<QDate>::type &editor, const QDate &value)
	{
		editor.setDate(value);
	}
};

template <> struct EditorValue<typename Editor<QTime>::type>
{
	static QTime value(const typename Editor<QTime>::type &editor)
	{
		return editor.time();
	}
	static void setValue(typename Editor<QTime>::type &editor, const QTime &value)
	{
		editor.setTime(value);
	}
};

template <> struct EditorValue<typename Editor<QDateTime>::type>
{
	static QDateTime value(const typename Editor<QDateTime>::type &editor)
	{
		return editor.dateTime();
	}
	static void setValue(typename Editor<QDateTime>::type &editor, const QDateTime &value)
	{
		editor.setDateTime(value);
	}
};

#endif /* VALUEDIALOGMETAFUNCTIONS_H_ */
