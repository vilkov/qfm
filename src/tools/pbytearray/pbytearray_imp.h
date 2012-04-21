#ifndef PBYTEARRAY_IMP_H_
#define PBYTEARRAY_IMP_H_


class PByteArrayImplementation
{
public:
	typedef char         value_type;
	typedef unsigned int size_type;

public:
	virtual ~PByteArrayImplementation() {}

	virtual size_type size() const = 0;
	virtual const value_type *c_str() const = 0;
	virtual const value_type *data() const = 0;
	virtual value_type *data() = 0;
	virtual void copyTo(void *dest) const = 0;

	virtual void truncate(size_type pos) = 0;
	virtual void append(const value_type *string, size_type size) = 0;
	virtual void prepend(const value_type *string, size_type size) = 0;
};

#endif /* PBYTEARRAY_IMP_H_ */
