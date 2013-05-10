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

```
$ ./omp -d 0
```

## Usage
For Help on Command Line Arguments, ``./omp --help``

```
$ ./omp --help
```

In GUI, use 'ESC' to quit.
