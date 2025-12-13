#ifdef DEBUG
    #define ON_DEBUG(...) __VA_ARGS__

#else
    #define ON_DEBUG(...)
    
#endif
#ifndef DOUBLE
#define DOUBLE

    #define DOUBLE_EQ(a, b) (fabs((a) - (b)) <= DBL_EPSILON)

#endif
