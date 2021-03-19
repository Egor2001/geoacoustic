#ifndef GEOACOUSTIC_STATIC_MACRO_HPP_
#define GEOACOUSTIC_STATIC_MACRO_HPP_

#ifndef GEO_DEBUG
    #define NDEBUG
#endif

#ifdef GEO_DEBUG
    #define GEO_ON_DEBUG(EXPR) (EXPR)
#else
    #define GEO_ON_DEBUG(EXPR) (static_cast<void>("DEBUG_OFF"))
#endif

#endif // GEOACOUSTIC_STATIC_MACRO_HPP_
