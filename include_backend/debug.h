#ifndef SONIC_ANYWHERE_DEBUG_H
#define SONIC_ANYWHERE_DEBUG_H

void assert__ (const char *msg, const char *file, int line);
void not_implemented__(const char* function, const char* file, int line);

void log__(const char* function, const char* file, int line, int level, const char* format, ...);

#define ASSERT(EX) if (!(EX)) { assert__ (#EX, __FILE__, __LINE__); }

#define LOG(...) log__(__func__, __FILE__, __LINE__, 0, __VA_ARGS__);
#define LOG_WARN(...) log__(__func__, __FILE__, __LINE__, 1, __VA_ARGS__);
#define LOG_ERROR(...) log__(__func__, __FILE__, __LINE__, 2, __VA_ARGS__);

#define RAISE_NOT_IMPLEMENTED LOG_ERROR("FUNCTION NOT IMPLEMENTED!!! [%s] | LINE: %d | FILE: %s", __func__, __LINE__, __FILE__);



#endif // SONIC_ANYWHERE_DEBUG_H
