#ifndef INCLUDE_DEBUG_H
#define INCLUDE_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG

#define debug(...)                                                        \
    do {                                                                  \
        fprintf(stderr, "%s:%d: %s(): ", __FILE__, __LINE__, __func__);   \
        fprintf(stderr, __VA_ARGS__);                                     \
    } while(0)

#define debugs(...)                                                       \
    do {                                                                  \
        fprintf(stderr, __VA_ARGS__);                                     \
    } while(0)

#define expects(X)                           \
    do {                                     \
        if (!(X)) {                          \
            debug("Expected '%s'.\n", #X);   \
            exit(1);                         \
        }                                    \
    } while(0)

#define ensures(X) expects(X)

#else

#define debug(...) 
#define debugs(...) 
#define expects(...)
#define ensures(...)

#endif  // NDEBUG

#endif  // INCLUDE_DEBUG_H