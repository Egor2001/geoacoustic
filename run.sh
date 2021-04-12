#!/bin/bash

# Checking args
#///////////////////////////////////////////////////////////////////////////////

if [ $# -ne 1 ]; then
    printf "USAGE: $0 OUTPUT_DIR\n"
    exit 1
else
    case $1 in
        "-h"|"-?"|"--help")
            printf "USAGE: $0 OUTPUT_DIR\n"
            exit 1
            ;;
        *)
            output_dir=$(readlink -f $1)
            mkdir -p $output_dir
            printf "Writing result to $output_dir\n"
            ;;
    esac
fi

# Checking OpenMP setup
#///////////////////////////////////////////////////////////////////////////////

if [ -z ${OMP_NUM_THREADS} ]; then
    printf "\nWARNING: OMP_NUM_THREADS was not set.\n"
    printf "\nRunning with thread count 1.\n"
    trd_max=1
else
    printf "\nRunning with thread count from 1 to ${OMP_NUM_THREADS}.\n"
    trd_max=${OMP_NUM_THREADS}
fi

if [ $trd_max -gt $(grep -c ^processor /proc/cpuinfo) ]; then
    printf "\nWARNING: OMP_NUM_THREADS is greater than number of cores.\n"
    omp_places="threads"
else
    omp_places="cores"
fi

# Checking architecture
#///////////////////////////////////////////////////////////////////////////////

if [ $(uname -m) == 'aarch64' ]; then
    printf "Configuring for aarch64\n"
    build_dir="$(dirname $(readlink -f "$0"))/armbuild"
    cells_cnt=256
    steps_cnt=256
    cfg_arr=(\
        "VectorCell_4" "ZC4VecCell_1" "ZCube4Cell_1" "LinearCell_4" \
        "VectorCell_0" "ZC4VecCell_0" "ZCube4Cell_0" "LinearCell_0"\
    )
elif [ $(uname -m) == 'x86_64' ]; then
    printf "Configuring for x86_64\n"
    build_dir="$(dirname $(readlink -f "$0"))/x86build"
    cells_cnt=256
    steps_cnt=256
    cfg_arr=(\
        "VectorCell_5" "ZC4VecCell_3" "ZCube4Cell_2" "LinearCell_5" \
        "VectorCell_0" "ZC4VecCell_0" "ZCube4Cell_0" "LinearCell_0"\
    )
else 
    printf "ERROR: $(uname -m) architecture is not supported!\n"
    exit 1
fi

# Compilation
#///////////////////////////////////////////////////////////////////////////////

cd $build_dir
for cfg in ${cfg_arr[@]}; do
    cfg_pair=(${cfg//_/ })
    cell=${cfg_pair[0]}
    rank=${cfg_pair[1]}

    cat > "${build_dir}/setup.h" <<EOF
    #define GEO_SETUP_CELL $cell
    #define GEO_SETUP_RANK $rank

    #define GEO_SETUP_CELLS_CNT $cells_cnt
    #define GEO_SETUP_STEPS_CNT $steps_cnt

    #define GEO_SETUP_DSPACE 3.0
    #define GEO_SETUP_DTIME 0.001

    #define GEO_SETUP_BULK 1000.0
    #define GEO_SETUP_RHO 0.001
EOF

    printf "\nCompiling [cell=$cell, rank=$rank]...\n\n"

    make bench
    mkdir -p "${output_dir}/bin"
    mv "${build_dir}/bin/main" "${output_dir}/bin/${cell}_${rank}"
done

# Running
#///////////////////////////////////////////////////////////////////////////////

cd $output_dir
for cfg in ${cfg_arr[@]}; do
    cfg_pair=(${cfg//_/ })
    cell=${cfg_pair[0]}
    rank=${cfg_pair[1]}

    binary="${output_dir}/bin/${cell}_${rank}"
    output="${output_dir}/${cell}_${rank}.txt"

    printf "threads,gcells\n" >> $output
    for trd_cnt in $(seq 1 $trd_max); do
        printf "\nRunning [cell=$cell, rank=$rank, threads=$trd_cnt]...\n"
        gcells=$(OMP_NUM_THREADS=$trd_cnt OMP_PLACES=$omp_places \
            $binary $cells_cnt $steps_cnt)
        printf "$trd_cnt, $gcells\n" >> $output
    done

    printf "\nOutput is appended to $output\n"
done

# Finalization
#///////////////////////////////////////////////////////////////////////////////

printf "Done [$output_dir generated]\n"
