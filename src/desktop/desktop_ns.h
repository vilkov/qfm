#ifndef DESKTOP_NS_H_
#define DESKTOP_NS_H_

#define DESKTOP_NS_BEGIN namespace Desktop {
#define DESKTOP_NS_END }

#define DESKTOP_NS(Expression) \
		DESKTOP_NS_BEGIN       \
			Expression;        \
		DESKTOP_NS_END

#endif /* DESKTOP_NS_H_ */
