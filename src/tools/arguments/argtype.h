#ifndef ARGTYPE_H_
#define ARGTYPE_H_


namespace Templates
{
	enum Type
	{
		Unknown          = 0x0,
		Char             = 0x1,
		WChar            = 0x2,
		Int              = 0x3,
		Float            = 0x4,
		Double           = 0x5,
		Const            = 0x8,
		Volatile         = 0x10,
		Signed           = 0x20,
		Pointer          = 0x40,
		Reference        = 0x80,
		ReferenceTo      = 0x100,
		ConstReferenceTo = 0x200
	};


	template <typename T> struct qualifiers                             { typedef T type; enum { value = Unknown }; };
	template <typename T> struct qualifiers<const T>                    { typedef T type; enum { value = Const }; };
	template <typename T> struct qualifiers<volatile T>                 { typedef T type; enum { value = Volatile }; };
	template <typename T> struct qualifiers<const volatile T>           { typedef T type; enum { value = Const | Volatile }; };
	template <typename T> struct qualifiers<T *>                        { typedef T type; enum { value = Pointer }; };
	template <typename T> struct qualifiers<const T *>                  { typedef T type; enum { value = Const | Pointer }; };
	template <typename T> struct qualifiers<volatile T *>               { typedef T type; enum { value = Volatile | Pointer }; };
	template <typename T> struct qualifiers<const volatile T *>         { typedef T type; enum { value = Const | Volatile | Pointer }; };
	template <typename T> struct qualifiers<T &>                        { typedef T type; enum { value = Reference }; };
	template <typename T> struct qualifiers<const T &>                  { typedef T type; enum { value = Const | Reference }; };
	template <typename T> struct qualifiers<volatile T &>               { typedef T type; enum { value = Volatile | Reference }; };
	template <typename T> struct qualifiers<const volatile T &>         { typedef T type; enum { value = Const | Volatile | Reference }; };
	template <typename T> struct qualifiers<T * &>                      { typedef T type; enum { value = Pointer | ReferenceTo }; };
	template <typename T> struct qualifiers<const T * &>                { typedef T type; enum { value = Const | Pointer | ReferenceTo }; };
	template <typename T> struct qualifiers<volatile T * &>             { typedef T type; enum { value = Volatile | Pointer | ReferenceTo }; };
	template <typename T> struct qualifiers<const volatile T * &>       { typedef T type; enum { value = Const | Volatile | Pointer | ReferenceTo }; };
	template <typename T> struct qualifiers<T * const &>                { typedef T type; enum { value = Pointer | ConstReferenceTo }; };
	template <typename T> struct qualifiers<const T * const &>          { typedef T type; enum { value = Const | Pointer | ConstReferenceTo }; };
	template <typename T> struct qualifiers<volatile T * const &>       { typedef T type; enum { value = Volatile | Pointer | ConstReferenceTo }; };
	template <typename T> struct qualifiers<const volatile T * const &> { typedef T type; enum { value = Const | Volatile | Pointer | ConstReferenceTo }; };


	template <typename T> struct type_is { enum { value = qualifiers<T>::value }; };
}

#endif /* ARGTYPE_H_ */
