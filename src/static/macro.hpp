#ifndef GEOACOUSTIC_STATIC_MACRO_HPP_
#define GEOACOUSTIC_STATIC_MACRO_HPP_

#ifdef GEO_DEBUG
    #ifndef NDEBUG
        #define NDEBUG 1
    #endif

    #define GEO_ON_DEBUG(EXPR) (EXPR)
#else
    #ifndef NDEBUG
        #define NDEBUG 0
    #endif

    #define GEO_ON_DEBUG(EXPR) (static_cast<void>("DEBUG_OFF"))
#endif

#endif // GEOACOUSTIC_STATIC_MACRO_HPP_
