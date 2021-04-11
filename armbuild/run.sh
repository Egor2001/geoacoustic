#!/bin/sh

cells_cnt=256
steps_cnt=256

run_test() {
    cell=$1; rank=$2; trd_max=$3; out=$4
    printf "\nGenerating setup for [cell=$cell, rank=$rank]...\n"
cat > setup.h <<EOF
    #define GEO_SETUP_CELL $cell
    #define GEO_SETUP_RANK $rank

    #define GEO_SETUP_CELLS_CNT $cells_cnt
    #define GEO_SETUP_STEPS_CNT $steps_cnt

    #define GEO_SETUP_DSPACE 3.0
    #define GEO_SETUP_DTIME 0.001

    #define GEO_SETUP_BULK 1000.0
    #define GEO_SETUP_RHO 0.001
EOF

    printf "\nBuilding benchmark target...\n\n"
    make bench

    for trd_cnt in $(seq 1 $trd_max); do
        printf "\nRunning benchmark with "
        printf "[cells=$cells_cnt, steps=$steps_cnt, threads=$trd_cnt]...\n"
        gcells=$(OMP_NUM_THREADS=$trd_cnt ./bin/main $cells_cnt $steps_cnt)

        printf "\nOutput is appended to $(realpath $out)\n"
        printf "$trd_cnt, $gcells\n" >> $out
    done
}

if [ $# -ne 1 ]; then
   printf "USAGE: $0 OUT\n"
   exit 1
else
    out=$(realpath $1)
fi

if [ -z ${OMP_NUM_THREADS} ]; then
    printf "\nWarning! OMP_NUM_THREADS was not set.\n"
    printf "\nRunning with thread count 1.\n"
    trd_max=1
else
    printf "\nRunning with thread count from 1 to ${OMP_NUM_THREADS}.\n"
    trd_max=${OMP_NUM_THREADS}
fi

working_dir=$(pwd)
script_dir=$(dirname "$0")

cell_arr=("VectorCell" "ZC4VecCell" "ZCube4Cell" "LinearCell")
rank_arr=(4 1 1 4)

cd $script_dir
for cell_idx in ${!cell_arr[@]}; do
    cell=${cell_arr[$cell_idx]}
    rank=${rank_arr[$cell_idx]}
    printf "[$cell $rank]\nthreads, gcells\n" >> $out
    run_test $cell $rank $trd_max $out
done
cd $working_dir

printf "Done [$out generated]\n"
