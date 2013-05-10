// File: utils.hh
// Date: Thu Aug 30 14:59:43 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

#ifndef __HEAD__UTILS
#define __HEAD__UTILS

#include <cstdarg>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <getopt.h>
#include <fstream>

void _print_debug_(int line, int myid, const char* fmt, ...);

#ifdef DEBUG
#define print_debug(fmt, ...) \
	_print_debug_(__LINE__, myid, fmt, ## __VA_ARGS__)
#else
#define print_debug(fmt, ...)
#endif



#define parse_error(fmt, ...) \
	do { \
		static const int MSG_LEN_MAX = 1000; \
		static char msg[MSG_LEN_MAX + 1]; \
		sprintf(msg, "error : " fmt, ##__VA_ARGS__); \
		fprintf(stderr, "%s\n", msg); \
		print_help(); \
	} while (0)

void print_help();
void parse_args(int argc, char* argv[]);
#endif //HEAD
