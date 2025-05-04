#ifndef SERIALPRINTF_HPP
#define SERIALPRINTF_HPP

#define SERIALPRINTF_DEBUG

void _SerialPrintf(const char *s, ...);
#ifdef SERIALPRINTF_DEBUG
#define SerialPrintf(...) _SerialPrintf(__VA_ARGS__)
#else
#define SerialPrintf(...)
#endif

#endif
