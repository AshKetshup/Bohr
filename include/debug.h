//------------------------------------------------------------------------------
//
// Bohr - Very Small PDB Molecular Visualizer
//
// RUNTIME LIBRARIES PACKAGE
//    debug.h
//
// DESCRIPTION:
// -----------
// Provides quick methods to provide debugging output on the console.
// 
//------------------------------------------------------------------------------

#ifndef INCLUDE_DEBUG_H
#define INCLUDE_DEBUG_H

#include <stdio.h>
#include <stdlib.h>

#ifndef NDEBUG      /* Debug mode */

// Outputs a debug message with a description of the file, line number and function
#define debug(...)                                                        \
    do {                                                                  \
        fprintf(stderr, "%s:%d: %s(): ", __FILE__, __LINE__, __func__);   \
        fprintf(stderr, __VA_ARGS__);                                     \
    } while(0)

// Outputs a simplified debug message
#define debugs(...)                                                       \
    do {                                                                  \
        fprintf(stderr, __VA_ARGS__);                                     \
    } while(0)

// Assert function, but with a personalized output message
#define expects(X)                           \
    do {                                     \
        if (!(X)) {                          \
            debug("Expected '%s'.\n", #X);   \
            exit(1);                         \
        }                                    \
    } while(0)

// Alias of expects()
#define ensures(X) expects(X)

#else               /* Release mode */

#define debug(...) 
#define debugs(...) 
#define expects(...)
#define ensures(...)

#endif  // NDEBUG

#endif  // INCLUDE_DEBUG_H