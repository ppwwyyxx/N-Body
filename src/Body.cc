// File: Body.cc
// Date: Sat May 11 23:29:56 2013 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include "Body.hh"

inline real_t distsqr(const Body& b1, const Body& b2)
{ return (b1.pos - b2.pos).sqr(); }

inline real_t dist(const Body& b1, const Body& b2)
{ return sqrt(distsqr(b1, b2)); }


inline void cal_vel_two(Body& b1, const Body& b2, const real_t dt){
	real_t r = distsqr(b2, b1);
	Vec F_tmp = dt * GRAVITY / (r) * (b2.pos - b1.pos)/sqrt(r);
	b1.v += F_tmp * b2.m;
	return;
}

real_t col_time(const Body &b1, const Body &b2, const real_t dt){
	// calculate whether they will collide in dt, if yes, return collision time, else return -1
	// equal to solve |b1.pos - b2.pos + t (b1.v - b2.v)| == b1.r + b2.r
	// (b1.pos.x - b2.pos.x + t (b1.v.x - b2.v.x))^2 +
	//		 (b1.pos.y - b2.pos.y + t (b1.v.y - b2.v.y))^2 == (b1.r + b2.r)^2
	real_t dpx = b1.pos.x - b2.pos.x, dpy = b1.pos.y - b2.pos.y,
		   dvx = b1.v.x - b2.v.x, dvy = b1.v.y - b2.v.y,
	   A = dvx * dvx + dvy * dvy,
	   B = 2 * dpx * dvx + 2 * dpy * dvy,
	   C = dpx * dpx + dpy * dpy - (b1.r + b2.r) * (b1.r + b2.r),
	   D = sqrt(B * B - 4 * A * C);
	real_t t1 = ( -B + D) / 2 / A, t2 = ( -B - D) / 2 / A;
	real_t t;
	if ((t1 < -EPS) || (t1 > dt) || (t1 > t2)) t = t2;
	else t = t1;
	if ((t > -EPS) && (t < dt))
		return t;
	else return -1;
}

inline void col_vel_two(Body &b1, const Body &b2){
	const Vec col = (b2.pos - b1.pos)/dist(b2, b1), mass = col / (b1.m + b2.m);
	real_t vc1 = b1.v.dot(col), vc2 = b2.v.dot(col);
	b1.v += 2 * b2.m * (vc2 - vc1) * mass;
	b1.v *= COLLISION_FACTOR;
}

bool overlap(const Body &b1, const Body &b2){
	return distsqr(b1, b2) < (b1.r + b2.r) * (b1.r + b2.r) / 3;
	// cannot be too close
}

bool tooclose(const Body &b1, const Body &b2){
	if (abs(b1.pos.x - b2.pos.x) < EPS)
		if (abs(b1.pos.y - b2.pos.y) < EPS)
			return true;
	return false;
}

void Body::cal_vel(const Body &b, const real_t dt)
{ cal_vel_two(*this, b, dt); }


void Body::col_vel(const Body& b)
{ col_vel_two(*this, b); }

void Body::move(real_t dt)
{ pos += dt * v; }

void Body::random_init(){
	pos.x = rand() % SIZE_X;
	pos.y = rand() % SIZE_Y;
}

void Body::judge_border(real_t dt){
	real_t tx = INT_MAX, ty = INT_MAX;
	bool col_x, col_y;
	bool real_col_x;
	if (v.x == 0)
		col_x = false;
	else {
		if (v.x < 0) tx = (pos.x - r) / (-v.x);
		else tx = (SIZE_X - r - pos.x) / v.x;
		if (tx > dt) col_x = false;
		else col_x = true;
	}
	if (v.y == 0)
		col_y = false;
	else {
		if (v.y < 0) ty = (pos.y - r) / (-v.y);
		else ty = (SIZE_Y - r - pos.y) / v.y;
		if (ty > dt) col_y = false;
		else col_y = true;
	}
	if ((!col_x) && (!col_y)) return;
	if (!col_y) real_col_x = true;
	else if (!col_x) real_col_x = false;
	else real_col_x = (tx < ty);
	if (real_col_x){
		move(tx);
		v.x = -v.x * BORDER_COLLISION_FACTOR;
	} else {
		move(ty);
		v.y = -v.y * BORDER_COLLISION_FACTOR;
	}
}
