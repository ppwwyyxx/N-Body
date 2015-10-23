// File: NBody.hh
// Date: Sat May 11 23:36:20 2013 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#ifndef __HEAD__NBODY
#define __HEAD__NBODY

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <climits>
#include <cassert>
#include <vector>

#include "common.hh"
#include "utils.hh"
#include "Body.hh"
using namespace std;

extern int myid, nproc;

void* thread_do_caller(void* intptr);

class NBody {
	public:
		int MAXN;
		real_t dt;
		vector<Body> b;

		NBody(){};
		NBody(int n, real_t m_dt):
			MAXN(n), dt(m_dt), b(MAXN)
		{}

		void init();
		void step();
		void go_steps(int n);
		Body& get_body(Vec pos);

		void mpi_slave();
		bool mpi_slave_do(int tag);

		void thread_do(long myid);

	private:
		void make_circle(real_t r, int start, int end);
		void vel_change(int target);
		void collision_change(int target);

		void mpi_master();
		void mpi_master_do(int tag);
		void share_data();

		void omp();
		void pthread();


};

extern NBody world;



#define Recv(a,b,c,d) MPI_Recv(a, b, MPI_INT, c, d, MPI_COMM_WORLD, &status)
#define Send(a,b,c,d) MPI_Send(a, b, MPI_INT, c, d, MPI_COMM_WORLD)
#define Bcast(a) MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD)

// need double defined
#define Recv_d(a,b,c,d) MPI_Recv(a, b, MPI_DOUBLE, c, d, MPI_COMM_WORLD, MPI_STATUS_IGNORE)
#define Send_d(a,b,c,d) MPI_Send(a, b, MPI_DOUBLE, c, d, MPI_COMM_WORLD)
#define Bcast_d(a) MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD)

enum { FINISH, DATA, BEGIN, EXIT, QUIT};


#endif //HEAD
