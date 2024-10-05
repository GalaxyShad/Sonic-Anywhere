#ifndef SONIC_ANYWHERE_DEBUG_H
#define SONIC_ANYWHERE_DEBUG_H

void assert__ (const char *msg, const char *file, int line);
void not_implemented__(const char* function, const char* file, int line);

#define ASSERT(EX) if (!(EX)) { assert__ (#EX, __FILE__, __LINE__); }

#define RAISE_NOT_IMPLEMENTED not_implemented__(__func__, __FILE__, __LINE__)

#endif // SONIC_ANYWHERE_DEBUG_H
