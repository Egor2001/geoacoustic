#ifndef WAVE_MODEL_TRIFLES_MACRO_HPP_
#define WAVE_MODEL_TRIFLES_MACRO_HPP_

#include <cassert>

#include "logger/logger.hpp"

#define TRIFLES_ASSERT(COND) \
    do { \
        assert(COND); \
    } while (0)

#define TRIFLES_LOG(LVL, MSG) \
    do { \
        TriflesGetLogger()->log(LVL, MSG); \
    } while (0)

#endif // WAVE_MODEL_TRIFLES_MACRO_HPP_
