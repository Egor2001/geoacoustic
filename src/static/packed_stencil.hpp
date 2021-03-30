#ifndef GEOACOUSTIC_STATIC_PACKED_STENCIL_HPP_
#define GEOACOUSTIC_STATIC_PACKED_STENCIL_HPP_

#if defined(__x86_64__)
    // TODO: to add FMA and AVX512 versions
    #if true // defined(__AVX__)
        // AVX version
        #include "x86/avx256_stencil.hpp"
    #else
        #error "avx simd extension is not supported"
    #endif
#elif defined(__aarch64__)
    // TODO: to add SVE and HELIUM versions
    #if defined(__ARM_NEON)
        // NEON version
        #include "arm/neon128_stencil.hpp"
    #else
        #error "neon simd extension is not supported"
    #endif
#else
    #error "only aarch64 and x86_64 are supported"
#endif

#endif // GEOACOUSTIC_STATIC_PACKED_STENCIL_HPP_
