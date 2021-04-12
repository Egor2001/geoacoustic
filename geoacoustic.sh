#!/bin/bash

printf "Cloning repository...\n"
git clone https://github.com/Egor2001/geoacoustic.git

if [ $(uname -m) == 'aarch64' ]; then
    workdir='geoacoustic/armbuild'
elif [ $(uname -m) == 'x86_64' ]; then
    workdir='geoacoustic/x86build'
else 
    printf "ERROR: $(uname -m) architecture is not supported!\n"
    printf "Cleanup repository...\n"
    rm -rf geoacoustic
    exit 1
fi

out=$(readlink -f 'geoacoustic-result.txt')
printf "Starting benchmark...\n"
cd $workdir
bash run.sh $out

printf "Cleanup repository...\n"
rm -rf geoacoustic

printf "Done (see $out for results)\n"
