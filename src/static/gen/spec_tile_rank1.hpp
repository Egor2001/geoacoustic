
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 0, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_000_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 0, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_001_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 0, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_002_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 1, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_010_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 1, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_011_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 1, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_012_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 2, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_020_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 2, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_021_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 0, 2, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_022_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 0, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_100_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 0, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_101_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 0, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_102_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 1, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_110_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 1, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_111_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 1, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_112_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 2, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_120_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 2, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_121_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 1, 2, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_122_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 0, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_200_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 0, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_201_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 0, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_202_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 1, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_210_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 1, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_211_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 1, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_212_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 2, 0, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_220_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 2, 1, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_221_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    
    template<typename TCell>
    struct RecursiveTile<TCell, 1, 2, 2, 2, void>
    {
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {
        #define PROC_POS_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \
                CellLayout<TCell>::cell_proc(idx3 + \
                int3_t{(X), (Y), (Z)}, cfg, ampl, ampl_next);
            #include "proc_tile_rank1/proc_tile_222_rank1.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }
    };
    