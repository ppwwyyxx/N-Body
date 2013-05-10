// File: Body.hh
// Date: Fri Aug 31 11:52:57 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#ifndef __HEAD__BODY
#define __HEAD__BODY
#include "common.hh"
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>

class Body {
	public:
		Body(){};
		Body(real_t m_r){
			r = m_r;
			m = r * r * 1e18;
		}

		Vec pos, v;
		real_t r,m;
		int index;

		void cal_vel(const Body& b, const real_t dt);
		void col_vel(const Body& b);
		void move(real_t dt);

		void random_init();
		void set_r(real_t m_r){
			r = m_r, m = r * r;
			rr = (double)(rand() % 70 + 30) / 100,
			  g = (double)(rand() % 70 + 30) / 100,
			  b = (double)(rand() % 70 + 30) / 100;
		
		}
		void judge_border(const real_t dt);
		double rr, g, b;
};


real_t col_time(const Body &b1, const Body &b2, const real_t dt);
void col_vel_two(Body &b1, const Body &b2);
bool overlap(const Body &b1, const Body &b2);
bool tooclose(const Body &b1, const Body &b2);

#endif //HEAD
