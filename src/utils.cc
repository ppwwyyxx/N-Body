// File: utils.cc
// Date: Thu Aug 30 19:36:23 2012 +0800
// Author: Yuxin Wu <ppwwyyxxc@gmail.com>

 
#include "utils.hh"
#include "common.hh"

using namespace std;
char* prog_name;
void _print_debug_(int line, int myid, const char* fmt, ...){
	fprintf(stderr, "[proc %d line %d] ", myid, line);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
}

void print_help(){
	printf("Usage:\n");
	printf("    %s -b NUM [-r <switch>] [-w <switch>] [-n <NUM_OF_PROC>] [-s <SIZE>] [-t <STEP>] [-h]\n", prog_name);
	printf("Options:\n");
	printf("    --ball=NUM,	 -b      number of balls. Default: 20.\n");
	printf("    --disp=0/1,	 -d      a switch on display mode containing a big ball. Default: 1\n");
	printf("    --wall=0/1,	 -w      a switch on whether to use window border as walls. Default: 1\n");
	printf("    --nproc=NUM, -n      number of threads(pthread only). number of CPUs by default\n");
	printf("    --size=SIZE, -s      size of window(by pixels). Format: [width]x[height]\n");
	printf("                         eg. 1200x800 (default)\n");
	printf("    --step=NUM,	 -t      number of loops to operate in simulation.\n");
	printf("                         NOTE: GUI will be off to calculate time.\n");
	printf("    --help,		 -h      print help.\n");
	exit(0);
}

void parse_args(int argc, char* argv[]){
	prog_name = argv[0];
	option long_options[] = {
		{"nproc",		required_argument,	NULL, 'n'},
		{"size",		required_argument,	NULL, 's'},
		{"step",		required_argument,  NULL, 't'},
		{"ball",		required_argument,  NULL, 'b'},
		{"disp",		required_argument,	NULL, 'd'},
		{"wall",		required_argument,	NULL, 'w'},
		{"help",		no_argument,		NULL, 'h'}
	};
	int opt;
	while ((opt = getopt_long(argc, argv, "-n:s:t:b:d:w:h", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':
				print_help();
				exit(0);
			case 'n':
				nproc = atoi(optarg);
				nproc = max(nproc, 1);
				break;
			case 's':
				{
					int w, h;
					if (sscanf(optarg, "%dx%d", &w, &h) != 2)
						parse_error("Invalid Size `%s'", optarg);
					SIZE_X = w, SIZE_Y = h;
				}
				break;
			case 't':
				nstep = atoi(optarg);
				break;
			case 'b':
				MAXN = atoi(optarg);
				break;
			case 'd':
				{
					int t = atoi(optarg);
					if (t == 0) disp = false;
					else if (t != 1) parse_error("Invalid Switch %s", optarg);
					break;
				}
			case 'w':
				{
					int t = atoi(optarg);
					if (t == 0) wall = false;
					else if (t != 1) parse_error("Invalid Switch %s", optarg);
					break;

				}
			case '1':
			case '?':
			default:
				print_help();
				break;
		}
	}
}
