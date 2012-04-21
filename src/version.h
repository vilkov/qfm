#ifndef VERSION_H_
#define VERSION_H_

#define MAJOR_VERSION_NUMBER    0
#define MINOR_VERSION_NUMBER    0
#define RELEASE_VERSION_NUMBER  0
#define BUILD_VERSION_NUMBER    1
#ifdef RC_INVOKED
#	define VERSION_STRING       "0.0.0.1\0"
#else
#	define VERSION_STRING       "0.0.0.1"
#endif


#endif /* VERSION_H_ */
