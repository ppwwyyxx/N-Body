// File: NBody.cc
// Date: Fri Aug 31 11:06:48 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>


#include "NBody.hh"
#include "Gui.hh"
using namespace std;

void NBody::make_circle(real_t R, int start, int end){
	for (int i = start; i < end; i ++){
		real_t V = 1000,
			   angle = M_PI * 2 / (end - start) * (i - start);
		b[i].set_r(3);
		b[i].pos = Vec(SIZE_X / 2, SIZE_Y / 2) + Vec(cos(angle), sin(angle)) * R;
		b[i].v = Vec(cos(angle + M_PI / 2), sin(angle + M_PI / 2)) *  V;
	}
}
 
void NBody::init(){
	if (disp){
		b[0].pos = Vec(SIZE_X / 2, SIZE_Y / 2);
		b[0].v = Vec(0,0);
		b[0].set_r(60);
		b[0].r = 20;
		make_circle(300, 1, MAXN);
	} else {
		for (int i = 0; i < MAXN; i ++){
			b[i].random_init();
			int r = rand() % 6 + 1;
			b[i].set_r(r);
		}
		b[0].v = Vec(0,0);
		b[0].set_r(70);
		b[0].r = 20;
	}
}

Body& NBody::get_body(Vec pos){
	real_t mind = INT_MAX;
	int mini = 0;
	for (int i = 0; i < MAXN; i ++){
		real_t tmp = pos.distsqr(b[i].pos);
		if (tmp < mind)
			mind = tmp, mini = i;
	}
	return b[mini];
}

void NBody::vel_change(int target){
	Body& body = b[target];
	for (int j = 0; j < MAXN; j ++){
		if (overlap(body, b[j])) 
			continue;
		body.cal_vel(b[j], dt);
	}
}

void NBody::collision_change(int target){
	Body& body = b[target];
	real_t mint = INT_MAX;
	int minj = 0;
	for (int j = 0 ; j < MAXN; j++){
		if (tooclose(body, b[j]))
			continue;

		real_t t = col_time(body, b[j], dt);
		if (t != -1){
			if (t < mint)
				mint = t, minj = j;
		}
	}
	if (mint == INT_MAX)
		return;
	body.move(mint);
	if (target * minj)
		b[minj].move(mint); // for solving two-pair
	body.col_vel(b[minj]);
	body.move(mint);
	if (target * minj)
		b[minj].move(mint);
}

void NBody::step(){
#ifdef USE_MPI
	mpi_master();
#else
#ifdef USE_PTH
	pthread();
#else
	omp();
#endif
#endif
	// judge crash border
#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < MAXN; i ++){
		if (wall)
			b[i].judge_border(dt);
		b[i].move(dt);
	}
#ifdef USE_MPI
	share_data();
#endif
}

void NBody::go_steps(int n){
	while (n--){
		step();
	}
#ifdef USE_MPI
	for (int i = 1; i < nproc; i ++)
		Send(&i, 1, i, QUIT);
#endif
}

