#ifndef GEOACOUSTIC_STATIC_VOLUME_ARRAY_HPP_
#define GEOACOUSTIC_STATIC_VOLUME_ARRAY_HPP_

#include "types.hpp"

// TODO(@geome_try): to replace with the custom assert
#include <cassert>

namespace geo {

// Const pointer - like wrapper 
template<typename TData>
class VolumeConstSpan
{
public:
    explicit VolumeSpan(const TData* data):
        data_{data}
    {}

    const TData& operator * () const { return *data_; }
    const TData* operator -> () const { return data_; }

    const TData* at(int3_t dim3, int3_t idx3) const
    {
        idx3 += int3_t{1, 1, 1};
        dim3 += int3_t{2, 2, 2};
        assert(idx3.x >= 0 && idx3.y >= 0 && idx3.z >= 0 && 
               idx3.x < dim3.x && idx3.y < dim3.y && idx3.z < dim3.z);

        return data_[idx3.x + dim3_.x * idx3.y + dim3_.x * dim3_.y * idx3.z];
    }

private:
    const TData* data_;
};

// Pointer - like wrapper 
template<typename TData>
class VolumeSpan
{
public:
    explicit VolumeSpan(TData* data):
        data_{data}
    {}

    TData& operator * () { return *data_; }
    TData* operator -> () { return data_; }
    const TData& operator * () const { return *data_; }
    const TData* operator -> () const { return data_; }

    operator VolumeConstSpan() { return VolumeConstSpan{data_}; }

    TData* at(int3_t dim3, int3_t idx3)
    {
        idx3 += int3_t{1, 1, 1};
        dim3 += int3_t{2, 2, 2};
        assert(idx3.x >= 0 && idx3.y >= 0 && idx3.z >= 0 && 
               idx3.x < dim3.x && idx3.y < dim3.y && idx3.z < dim3.z);

        return data_[idx3.x + dim3_.x * idx3.y + dim3_.x * dim3_.y * idx3.z];
    }

    const TData* at(int3_t dim3, int3_t idx3) const
    {
        return const_cast<VolumeSpan*>(this)->at(dim3, idx3);
    }

private:
    TData* data_;
};

template<typename TD>
class VolumeArray
{
public:
    using TData = TD;

    explicit VolumeArray(int3_t dim3):
        dim3_{dim3}, data_{}
    {
        assert(dim3_.x > 0 && dim3_.y > 0 && dim3_.z > 0);

        dim3_ += int3_t{2, 2, 2};
        data_.resize(dim3_.z * dim3_.y * dim3_.x); 
    }

    int3_t dim3() const
    {
        return dim3_;
    }

    TData& operator [] (int3_t idx3)
    {
        idx3 += int3_t{1, 1, 1};
        assert(idx3.x >= 0 && idx3.y >= 0 && idx3.z >= 0 && 
               idx3.x < dim3_.x && idx3.y < dim3_.y && idx3.z < dim3_.z);

        return data_[idx3.x + dim3_.x * idx3.y + dim3_.x * dim3_.y * idx3.z];
    }

    const TData& operator [] (int3_t idx3) const
    {
        return const_cast<VolumeSpan*>(this)->operator[](idx3);
    }

    VolumeSpan span()
    {
        return VolumeSpan{data_.data()};
    }

    VolumeConstSpan span() const
    {
        return VolumeConstSpan{data_.data()};
    }

private:
    int3_t dim3_;
    std::vector<TData> data_;
};

} // namespace geo

#endif // GEOACOUSTIC_STATIC_VOLUME_ARRAY_HPP_
