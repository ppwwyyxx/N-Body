// File: common.hh
// Date: Fri Aug 31 11:07:45 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#ifndef __HEAD__COMMON
#define __HEAD__COMMON
using namespace std;

extern int SIZE_X, SIZE_Y, MAXN, nproc, nstep;
extern bool disp, wall;

#define real_t double
#define GRAVITY 80000
#define EPS 1e-8
#define COLLISION_FACTOR 0.9
#define BORDER_COLLISION_FACTOR 0.7
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstdarg>

class Vec {
	public:
		Vec(){ };
		Vec(real_t m_x, real_t m_y):x(m_x),y(m_y){};
		real_t x,y;

		real_t sqr() const {
			return x * x + y * y;
		}

		real_t dot(const Vec v) const { // const
			return x * v.x + y * v.y;
		}

		real_t distsqr(const Vec v) const {
			return Vec(x - v.x, y - v.y).sqr();
		}

		Vec& operator = (const Vec& right_vec){
			x = right_vec.x, y = right_vec.y;
			return *this;
		}
		Vec& operator += (const Vec &c){
			x += c.x, y += c.y;
			return *this;
		}
		Vec& operator -= (const Vec &c){
			x -= c.x, y -= c.y;
			return *this;
		}
		const Vec operator +(const Vec &c) const {
			Vec tmp = *this;
			tmp += c;
			return tmp;
		}
		const Vec operator -(const Vec &c) const {
			Vec tmp = *this;
			tmp -= c;
			return tmp;
		}
		const Vec operator -() const {
			return Vec(-x, -y);
		}

		const Vec operator *(real_t f) const {
			Vec tmp = *this;
			tmp.x *= f, tmp.y *= f;
			return tmp;
		}
		Vec& operator *= (real_t f){
			x *= f, y *= f;
			return *this;
		}
		const Vec operator /(real_t f) const {
			Vec tmp = *this;
			tmp.x /= f, tmp.y /= f;
			return tmp;
		}
		friend ostream& operator << (ostream&, const Vec&);
		friend istream& operator >> (istream&, Vec&);
};

Vec operator * (real_t f, const Vec& vec);

void ssleep(double t);

#endif //HEAD
