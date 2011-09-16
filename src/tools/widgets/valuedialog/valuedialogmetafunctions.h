#ifndef VALUEDIALOGMETAFUNCTIONS_H_
#define VALUEDIALOGMETAFUNCTIONS_H_

#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>


/* Select editor meta-function */
template <typename Value> struct Editor {};

template <> struct Editor<QString>        { typedef QLineEdit type; };
template <> struct Editor<char>           { typedef QSpinBox type; };
template <> struct Editor<unsigned char>  { typedef QSpinBox type; };
template <> struct Editor<short>          { typedef QSpinBox type; };
template <> struct Editor<unsigned short> { typedef QSpinBox type; };
template <> struct Editor<int>            { typedef QSpinBox type; };
template <> struct Editor<unsigned int>   { typedef QSpinBox type; };
template <> struct Editor<long>           { typedef QSpinBox type; };
template <> struct Editor<unsigned long>  { typedef QSpinBox type; };


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
		return editor.setText(value);
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
		return editor.setValue(value);
	}
};

#endif /* VALUEDIALOGMETAFUNCTIONS_H_ */
