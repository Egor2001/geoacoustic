#ifndef GEOACOUSTIC_MT_GRID_HPP_
#define GEOACOUSTIC_GMT_RID_HPP_

#include <cassert>

#include "types.hpp"
#include "tiling.hpp"
#include "domain.hpp"

namespace geo {

class Grid
{
public:
    struct Routine;

    explicit Grid(std::vector<Tiling::Tile>&& tiles):
        tiles_(std::move(tiles))
    {}

    void run_threads(Domain& cur, Domain& next, int_t time_ticks)
    {
        for (size_t idx = 0; idx < threads_cnt_; ++idx)
        {
            std::vector<size_t> tile_nums;

            runners_.emplace_back(Routine { 
                /* .that = */ this, 
                /* .bound_time = */ time_ticks,
                /* .tile_nums = */ std::move(tile_nums)
            });
        }
    }

    void join_threads()
    {
        for (auto& runner : runners_)
            runner.join();
    }

private:
    size_t threads_cnt_{std::thread::hardware_concurrency()};
    std::atomic_size_t arrived_cnt_{0};
    std::binary_semaphore on_arrived_{0};
    std::latch on_bound_{threads_cnt_ + 1};

    std::vector<Tiling::Tile> tiles_;
    std::vector<std::thread> runners_;
};

struct Grid::Routine
{
    void operator () ()
    {
        assert(that);
        for (int_t num : tile_nums)
        {
            if (that->tiles_[num].start_time > bound_time)
            {
                if (that->arrived_cnt_.fetch_inc() + 1 == 
                    that->threads_cnt_)
                {
                    that->arrived_cnt_.store(0);
                    that->on_arrived_.release();
                }

                that->on_bound.arrive_and_wait();
            }

            that->tiles_[num].process(bound_time);
        }
    }

    Grid* that;
    int_t bound_time;
    std::vector<size_t> tile_nums;
};

} // namespace geo

#endif // GEOACOUSTIC_MT_GRID_HPP_
