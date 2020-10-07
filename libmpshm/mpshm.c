#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syslog.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <assert.h>

#include "log.h"

#include "mpshm.h"

extern void mpshm_show(void);

int main(int argc, char *argv[]) {

    mpshm_show();
    return 0;
}
