#include <sys/resource.h>

#include "src/multiThreaded.h"
#include "src/sequential.h"

int main() {
    const rlim_t stackSize = 1L * 1024L * 1024L * 1024L; // 1GB stack
    struct rlimit rl;
    getrlimit(RLIMIT_STACK, &rl);
    if (rl.rlim_cur < stackSize)
        rl.rlim_cur = stackSize;
    setrlimit(RLIMIT_STACK, &rl);

//    sequentialStatic();
//    sequentialDynamic();
    multiThreadedStatic();
    multiThreadedDynamic();
    return 0;
}
