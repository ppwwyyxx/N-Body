// File: main.cc
// Date: Fri Aug 31 12:01:23 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <cassert>
#include <climits>
#include <getopt.h>
#include <ctime>
#include <sys/time.h>
#include <pthread.h>
#include <omp.h>

#include "common.hh"
#include "utils.hh"
#include "Gui.hh"
#include "Body.hh"
#include "NBody.hh"
using namespace std;
int SIZE_X = 1200,
	SIZE_Y = 800,
	FPS = 35,
	MAXN = 25;
real_t time_step = 0.004;
NBody world;
int myid, nproc;
int nstep = 0;
bool disp = true,
	 wall = true;

int main(int argc, char* argv[]){
	double t1 = 0, t2;
	struct timeval start, end;
	srand(time(NULL));

#ifdef USE_MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	if (nproc == 1){
		cerr << "You Need At Least 2 Processors To Run This Program !"  << endl;
		exit(1);
	}
#else
	myid = 0;
	nproc = sysconf(_SC_NPROCESSORS_ONLN);
#endif

	if (myid == 0){
		parse_args(argc, argv);
	}

#ifdef USE_MPI
	Bcast(SIZE_X);
	Bcast(SIZE_Y);
	Bcast(MAXN);
#endif

	world = NBody(MAXN, time_step);
	world.init();

	if (myid == 0){
		gettimeofday(&start, NULL);
		t1 = start.tv_sec + start.tv_usec / 1e6;
		if (!nstep){
			gtk_init(&argc, &argv);
			show_init();
			gtk_main();
		} else {
			world.go_steps(nstep);
		}
	} else {
		world.mpi_slave();
	}

	if (myid == 0){
		gettimeofday(&end, NULL);
		t2 = end.tv_sec + end.tv_usec / 1e6;
		printf("%lf seconds in total...\n", t2 - t1);
	}
	delete [] world.b;
#ifdef USE_MPI
	MPI_Finalize();
#endif
	return 0;
}
