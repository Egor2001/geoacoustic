# geoacoustic
Acoustic wave geomodelling package based on fdtd algorithms

## run benchmarks
To build and run benchmarks and store GCell/sec results in directory OUTDIR.

```console
$ OMP_NUM_THREADS=$(grep -c ^processor /proc/cpuinfo) run.sh OUTDIR
```

Requires modern C++14 GNU/Clang compiler to be used.  
Default compiler is GNU.

## contacts
- telegram: [@geome\_try](https://t.me/geome_try)
- mail:
  - elchinov.es@phystech.edu
  - elchinov.es@gmail.com
