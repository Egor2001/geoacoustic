#ifndef GEOACOUSTIC_STATIC_MEMORY_ALIGNED_ALLOCATOR_HPP_
#define GEOACOUSTIC_STATIC_MEMORY_ALIGNED_ALLOCATOR_HPP_

/**
 * @file
 * @author Egor Elchinov <elchinov.es@gmail.com>
 * @version 2.0
 */

#include <memory>
#include <cstdlib>
#include <malloc.h>

/// @brief
namespace geo {

#if !defined(__OSX__) // To detect if aligned_alloc is unavailable
/**
 * @brief Allocates memory with respect to type alignment
 * @tparam T Type of objects to allocate
 * @tparam NA Desired alignment (defaults to alignof(T))
 */
template<typename T, size_t NA = alignof(T)>
class AlignedAllocator
{
public:
    /// See basic allocator interface
    template<typename U, size_t NB = alignof(U)>
    struct rebind
    {
        using other = AlignedAllocator<U, NB>;
    };

    /// See basic allocator interface
    using value_type = T;
    /// See basic allocator interface
    using size_type = size_t;
    /// See basic allocator interface
    using difference_type = ptrdiff_t;

    /// Desired alignment
    static constexpr size_t align_value = NA;

    /// Default constructor
    AlignedAllocator() = default;

    /// See basic allocator interface
    template<typename U, size_t NB> 
    constexpr AlignedAllocator(const AlignedAllocator<U, NB>&) noexcept:
        AlignedAllocator()
    {}

    /**
     * @brief Allocates memory (using std::aligned_alloc)
     * @param count Number of objects to allocate memory for
     * @return Pointer to the allocated memory (respectively aligned)
     */
    value_type* allocate(size_type count)
    {
        // TODO: conditional macro
        return static_cast<value_type*>(
                memalign(align_value, count * sizeof(value_type)));
/*
        return static_cast<value_type*>(
                aligned_alloc(align_value, count * sizeof(value_type)));
*/
    }

    /**
     * @brief Deallocates memory (using std::free)
     * @param ptr Pointer to the memory being previously allocate()'d
     * @param count [unused] Number of objects the memory contains
     */
    void deallocate(value_type* ptr, size_type count) noexcept
    {
        static_cast<void>(count); // [[maybe_unused]] before C++17
        return std::free(ptr);
    }

private:
};

/// See basic allocator interface
template<typename T, typename U, size_t NA, size_t NB>
bool operator == (const AlignedAllocator<T, NA>&, 
                  const AlignedAllocator<U, NB>&) 
{ 
    return true; 
}

/// See basic allocator interface
template<typename T, typename U, size_t NA, size_t NB>
bool operator != (const AlignedAllocator<T, NA>&, 
                  const AlignedAllocator<U, NB>&) 
{ 
    return false; 
}

#else // defined(__OSX__)

template<typename T, size_t NA = alignof(T)>
using AlignedAllocator<T, NA> = std::allocator<T>;

#endif // defined(__OSX__)

} // namespace geo

#endif // GEOACOUSTIC_STATIC_MEMORY_ALIGNED_ALLOCATOR_HPP_
