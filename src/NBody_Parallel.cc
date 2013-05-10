// File: NBody_Parallel.cc
// Date: Mon Sep 03 22:41:29 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>


#include <pthread.h>
#include "NBody.hh"
using namespace std;
#define VEL_WORK 2
#define COLL_WORK 4

int mpi_unit = 30;
int pth_unit = 10,
	pth_now;
pthread_mutex_t now_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier; 
pthread_barrierattr_t barrier_attr;

void NBody::share_data(){
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < MAXN; i ++){
		Bcast_d(b[i].pos.x);
		Bcast_d(b[i].pos.y);
		Bcast_d(b[i].v.x);
		Bcast_d(b[i].v.y);
	}
}

void NBody::mpi_master(){
	mpi_master_do(VEL_WORK);
	share_data();
	mpi_master_do(COLL_WORK); // col
}

void NBody::mpi_master_do(int tag){
	MPI_Status status;
	int nFinished = 0, now = 0, buf, proc_used = 0;

	for (int i = 1; i < nproc; i ++){
		Send(&now, 1, i, BEGIN);
		int num_work = min(mpi_unit, MAXN - now);
		now += num_work;
		proc_used = i;
		if (now >= MAXN) break;
	}
	assert(proc_used);

	while ((now < MAXN) || (nFinished < proc_used)){
		// buf is the beginning line of next task
		Recv(&buf, 1, MPI_ANY_SOURCE, FINISH);
		int source = status.MPI_SOURCE;
		int num_work = min(mpi_unit, MAXN - buf);
		nFinished ++;

		// recv data
		real_t data[tag * num_work];
		Recv_d(&data, num_work * tag, source, DATA);
		for (int i = buf; i < buf + num_work; i ++){
			b[i].v.x = data[tag * (i - buf)];
			b[i].v.y = data[tag * (i - buf) + 1];
			if (tag == COLL_WORK){
				b[i].pos.x = data[4 * (i - buf) + 2];
				b[i].pos.y = data[4 * (i - buf) + 3];
			}
		}

		if (now < MAXN){
			nFinished --;
			Send(&now, 1, source, BEGIN);
			int num_work = min(mpi_unit, MAXN - now);
			now += num_work;
		}
	}
	for (int i = 1; i < nproc; i ++){
		Send(&buf, 1, i, EXIT);
	}
}

bool NBody::mpi_slave_do(int tag){
	bool done = false;
	MPI_Status status;

	// for update velocity
	while (!done){ // this refresh
		int buf;
		Recv(&buf, 1, 0, MPI_ANY_TAG);
		switch (status.MPI_TAG) {
			case BEGIN:
				{
					int num_work = min(mpi_unit, MAXN - buf);
					for (int i = buf; i < buf + num_work; i ++){
						if (tag == VEL_WORK)
							vel_change(i);
						else 
							collision_change(i);
					}
					Send(&buf, 1, 0, FINISH);

					// send data
					real_t data[tag * num_work];
					for (int i = buf; i < buf + num_work; i ++){
						data[(i - buf) * tag] = b[i].v.x;
						data[(i - buf) * tag + 1] = b[i].v.y;
						if (tag == COLL_WORK){
							data[(i - buf) * 4 + 2] = b[i].pos.x;
							data[(i - buf) * 4 + 3] = b[i].pos.y;
						}
					}
					Send_d(data, num_work * tag, 0, DATA);
				}
				break;
			case EXIT:
				done = true;
				break;
			case QUIT:
				done = true;
				return true;
				break;
			default:
				break;
		}
	} 
	return false;
}

void NBody::mpi_slave(){
	while (1){ // not quit program
		if (mpi_slave_do(2)) break;
		share_data(); // before collision
		if (mpi_slave_do(4)) break;
		share_data(); // before next timestep
	}
}

void* thread_do_caller(void* intptr){
	long id = (long)intptr;
	world.thread_do(id);
	pthread_exit(NULL);
}

void NBody::thread_do(long myid){
	int local, num_line;
	while (1){
		pthread_mutex_lock(&now_mutex);

		if (pth_now >= MAXN) break;
		local = pth_now;
		pth_now = min(pth_now + pth_unit, MAXN);
		num_line = pth_now - local;

		pthread_mutex_unlock(&now_mutex);

		for (int i = local; i < local + num_line; i ++){
			vel_change(i);
		}
	}
	pthread_mutex_unlock(&now_mutex);

	// barrier
	pthread_barrier_wait(&barrier);
	if (myid == 0)
		pth_now = 0;
	pthread_barrier_wait(&barrier);

	while (1){
		pthread_mutex_lock(&now_mutex);

		if (pth_now >= MAXN) break;
		local = pth_now;
		pth_now = min(pth_now + pth_unit, MAXN);
		num_line = pth_now - local;

		pthread_mutex_unlock(&now_mutex);

		for (int i = local; i < local + num_line; i ++)
			collision_change(i);
	}
	pthread_mutex_unlock(&now_mutex);
}

void NBody::pthread(){
	pthread_t threads[nproc];
	pth_now = 0;

	int ret = pthread_barrier_init(&barrier, &barrier_attr, nproc);
	if (ret) cerr << "ret from pthread_barrier_init error"  << endl;

	for (long i = 0; i < nproc; i ++){
		int ret = pthread_create(&threads[i], NULL, thread_do_caller, (void*)i);
		if (ret) cerr << "ret from pthread_create error" << endl;
	}
	void *status;
	for (int i = 0; i < nproc; i ++){
		int ret = pthread_join(threads[i], &status);
		if (ret) cerr << "ret from pthread_join error" << endl;
	}
	pthread_barrier_destroy(&barrier);
}

void NBody::omp(){
#ifdef USE_OMP
#pragma omp parallel for schedule(dynamic)
#endif
	for (int i = 0; i < MAXN; i ++)
		vel_change(i);
#ifdef USE_OMP
#pragma omp parallel for schedule(dynamic)
#endif
	for (int i = 0; i < MAXN; i ++)
		collision_change(i);
}
