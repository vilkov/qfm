#ifndef VALUEDIALOGMETAFUNCTIONS_H_
#define VALUEDIALOGMETAFUNCTIONS_H_

#include <QtCore/QVariant>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>
#include <QtGui/QDateTimeEdit>


/* Select editor meta-function */
template <typename Value> struct Editor {};

template <> struct Editor<QString>        { typedef QLineEdit     type; typedef QString        value_type; };
template <> struct Editor<char>           { typedef QSpinBox      type; typedef char           value_type; };
template <> struct Editor<unsigned char>  { typedef QSpinBox      type; typedef unsigned char  value_type; };
template <> struct Editor<short>          { typedef QSpinBox      type; typedef short          value_type; };
template <> struct Editor<unsigned short> { typedef QSpinBox      type; typedef unsigned short value_type; };
template <> struct Editor<int>            { typedef QSpinBox      type; typedef int            value_type; };
template <> struct Editor<unsigned int>   { typedef QSpinBox      type; typedef unsigned int   value_type; };
template <> struct Editor<long>           { typedef QSpinBox      type; typedef long           value_type; };
template <> struct Editor<unsigned long>  { typedef QSpinBox      type; typedef unsigned long  value_type; };
template <> struct Editor<QDate>          { typedef QDateEdit     type; typedef QDate          value_type; };
template <> struct Editor<QTime>          { typedef QTimeEdit     type; typedef QTime          value_type; };
template <> struct Editor<QDateTime>      { typedef QDateTimeEdit type; typedef QDateTime      value_type; };


/* Editor value meta-function */
template <typename T> struct EditorValue {};

template <> struct EditorValue<QString>
{
	typedef Editor<QString>                     EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static QString value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->text().simplified();
	}
	static void setValue(QWidget *editor, const QString &value)
	{
		static_cast<EditorType*>(editor)->setText(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setText(value.value<Value>());
	}
};

template <> struct EditorValue<char>
{
	typedef Editor<char>                        EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static char value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->value();
	}
	static void setValue(QWidget *editor, int value)
	{
		static_cast<EditorType*>(editor)->setValue(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setValue(value.value<Value>());
	}
};

template <> struct EditorValue<int>
{
	typedef Editor<int>                         EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static char value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->value();
	}
	static void setValue(QWidget *editor, int value)
	{
		static_cast<EditorType*>(editor)->setValue(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setValue(value.value<Value>());
	}
};

template <> struct EditorValue<QDate>
{
	typedef Editor<QDate>                       EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static QDate value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->date();
	}
	static void setValue(QWidget *editor, const QDate &value)
	{
		static_cast<EditorType*>(editor)->setDate(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setDate(value.value<Value>());
	}
};

template <> struct EditorValue<QTime>
{
	typedef Editor<QTime>                       EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static QTime value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->time();
	}
	static void setValue(QWidget *editor, const QTime &value)
	{
		static_cast<EditorType*>(editor)->setTime(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setTime(value.value<Value>());
	}
};

template <> struct EditorValue<QDateTime>
{
	typedef Editor<QDateTime>                   EditorBaseType;
	typedef const typename EditorBaseType::type ConstEditorType;
	typedef typename EditorBaseType::type       EditorType;
	typedef typename EditorBaseType::value_type Value;

	static QDateTime value(const QWidget *editor)
	{
		return static_cast<ConstEditorType*>(editor)->dateTime();
	}
	static void setValue(QWidget *editor, const QDateTime &value)
	{
		static_cast<EditorType*>(editor)->setDateTime(value);
	}
	static void setValue(QWidget *editor, const QVariant &value)
	{
		static_cast<EditorType*>(editor)->setDateTime(value.value<Value>());
	}
};

#endif /* VALUEDIALOGMETAFUNCTIONS_H_ */
