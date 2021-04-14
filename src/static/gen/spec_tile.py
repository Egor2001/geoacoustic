#!/usr/bin/python3

def gen(out, rank, xt, yt, zt):
    out.write("""
    template<typename TCell>
    struct RecursiveTile<TCell, {}, {}, {}, {}, void>
    {{
        static void tile_proc(int3_t idx3, const Config<TCell>& cfg,
                VolumeSpan<TCell> ampl_next, VolumeSpan<TCell> ampl)
        {{
        #define PROC_POS_CELL_(X, Y, Z) \\
                CellLayout<TCell>::cell_proc(idx3 + \\
                int3_t{{(X), (Y), (Z)}}, cfg, ampl_next, ampl);
        #define PROC_NEG_CELL_(X, Y, Z) \\
                CellLayout<TCell>::cell_proc(idx3 + \\
                int3_t{{(X), (Y), (Z)}}, cfg, ampl, ampl_next);
            #include "proc_tile_rank{}/proc_tile_{}{}{}_rank{}.hpp"
        #undef PROC_POS_CELL_
        #undef PROC_NEG_CELL_
        }}
    }};
    """.format(rank, xt, yt, zt, rank, xt, yt, zt, rank)
    )

for rank in [1, 2]:
    with open("spec_tile_rank{}.hpp".format(rank), 'w') as out:
        for xt in [0, 1, 2]:
            for yt in [0, 1, 2]:
                for zt in [0, 1, 2]:
                    gen(out, rank, xt, yt, zt)
