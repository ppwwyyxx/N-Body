// File: common.cc
// Date: Thu Aug 30 12:45:06 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#include <cstdlib>
#include <iostream>
#include "common.hh"
using namespace std;

ostream& operator << (ostream& o, const Vec& vec){
	o << "x: " << vec.x << ",y: " << vec.y;
	return o;
}
 
istream& operator >> (istream& in, Vec& vec){
	in >> vec.x >> vec.y;
	return in;
}

Vec operator * (real_t f, const Vec& vec){
	return vec * f;
}

void ssleep(double t){
	usleep(t * 1000000);
}

