## Introduction

This is a small gtk program of [N-Body Simulation](http://en.wikipedia.org/wiki/N-body_simulation) implemented by multi-thread / multi-process programming library.

## Compile
Dependencies: openmpi

Compile:
```
$ ./make_all_version
```
This will generate 4 versions of the program: sequential, openmp, mpi, pthread

## Example
```
$ ./omp
```
![demo](https://github.com/ppwwyyxx/N-Body/raw/master/report/res/screen.png)

```
$ ./omp -d 0
```
![demo2](https://github.com/ppwwyyxx/N-Body/raw/master/report/res/screen2.png)

## Usage
See Help on Command Line Arguments, e.g.:

```
$ ./omp --help
Usage:
    ./omp -b NUM [-r <switch>] [-w <switch>] [-n <NUM_OF_PROC>] [-s <SIZE>] [-t <STEP>] [-h]
Options:
    --ball=NUM,  -b      number of balls. Default: 20.
    --disp=0/1,  -d      a switch on display mode containing a big ball. Default: 1
    --wall=0/1,  -w      a switch on whether to use window border as walls. Default: 1
    --nproc=NUM, -n      number of threads(pthread only). number of CPUs by default
    --size=SIZE, -s      size of window(by pixels). Format: [width]x[height]
                         eg. 1200x800 (default)
    --step=NUM,  -t      number of loops to operate in simulation.
                         NOTE: GUI will be off to calculate time.
    --help,              -h      print help.
```

In GUI, use 'Space' to pause, 'ESC' to quit.
