#include <sys/resource.h>

#include "sequential.h"
#include "multiThreaded.h"

int main(int argc, char** argv)
{
	const rlim_t stackSize = 1024L * 1024L * 1024L; // 1GB stack
	struct rlimit rl;
	getrlimit(RLIMIT_STACK, &rl);
	if (rl.rlim_cur < stackSize) 
		rl.rlim_cur = stackSize;
	setrlimit(RLIMIT_STACK, &rl);
    //sequentialStatic();
	//sequentialDynamic();
    multiThreadedStatic();
	//multiThreadedDynamic();
	return 0;
}
