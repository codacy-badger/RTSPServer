#include <cstdio>
#include <cstdlib>
#include <execinfo.h>
#include <limits.h>
#include <sys/resource.h>
#include "utils/util.h"

void Util::enable_core_dump() {
    struct rlimit corelim;

    corelim.rlim_cur = RLIM_INFINITY;
    corelim.rlim_max = RLIM_INFINITY;

    if (setrlimit(RLIMIT_CORE, &corelim) != 0) {
        printf("[%s#%d]setrlimit failed!!!\n", __func__, __LINE__);
    }

    if (0 != system("echo \"/data/core-%p\" > /proc/sys/kernel/core_pattern")) {
        printf("[%s#%d]configure core_pattern failed\n", __func__, __LINE__);
    }
}