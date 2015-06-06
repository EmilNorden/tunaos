#ifndef SYSTEM_H_
#define SYSTEM_H_

void panic(const char *file, const char *function, int line, const char *message);

#ifdef ENABLE_ASSERTIONS

#define ASSERT(x, message) \
	PANIC_ON(!(x), message)
		
#else

#define ASSERT(x, message)

#endif /* ENABLE_ASSERTIONS */

#define PANIC_ON(x, message) \
	if(x) panic(__FILE__, __FUNCTION__, __LINE__, message);

#endif /* SYSTEM_H_ */