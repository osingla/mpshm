#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <errno.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>

//#include "../kernel/mpik_ioctl.h"

#include "mpshm.h"
#include "libmpshm.h"

#include "log.h"

static sem_t *sema_channels = NULL;		        ///< File descriptor for global semaphore
static int shm_fd = -1;			                ///< File descriptor for /dev/shmem/mpshmem_channels
static mpshm_channels_t *shm_channels = NULL;   ///< Pointer to global channel data

static void constructor() __attribute__((constructor));
static void destructor() __attribute__((destructor));

void constructor() {

	sema_channels = sem_open("mpshmem_channels", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR, 1);
    if (sema_channels == NULL) {
		fprintf(stderr, "Could not create semaphore object - %m\n");
		return;
    }

	sem_wait(sema_channels);

    // Create the shared memory?
    shm_fd = shm_open("mpshmem_channels", O_EXCL | O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IXUSR);
    if (shm_fd != -1) {
DEBUG("@@@");
        // Create the shared memory
        if (ftruncate(shm_fd, sizeof(mpshm_channels_t)) == -1) {
		    fprintf(stderr, "Could not allocate shared memory object - %m\n");
		    close(shm_fd);
		    shm_fd = -1;
        }
        else {
            shm_channels = (mpshm_channels_t *)mmap(NULL, sizeof(mpshm_channels_t), PROT_READ | PROT_WRITE,
                MAP_SHARED, shm_fd, 0);
            if (shm_channels == NULL) {
		        fprintf(stderr, "Could not map shared memory object - %m\n");
		        close(shm_fd);
		        shm_fd = -1;
            }
            memset(shm_channels, 0, sizeof(mpshm_channels_t));
            shm_channels->last_free = -1;
        }
        goto done;
    }

    // Just open the shared memory
DEBUG("@@@");
    shm_fd = shm_open("mpshmem_channels", O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
    if (shm_fd == -1) {
    	fprintf(stderr, "Could not open shared memory object - %m\n");
		goto done;
    }
    shm_channels = (mpshm_channels_t *)mmap(NULL, sizeof(mpshm_channels_t), PROT_READ | PROT_WRITE,
        MAP_SHARED, shm_fd, 0);
    if (shm_channels == NULL) {
		fprintf(stderr, "Could not map shared memory object - %m\n");
		close(shm_fd);
		shm_fd = -1;
    }

done:
    DEBUG("constructor: shm_fd=%d - channels=%p", shm_fd, shm_channels);
	sem_post(sema_channels);

}

void destructor() {
    DEBUG("destructor: shm_fd=%d", shm_fd);
    if (shm_fd != -1) {
    	close(shm_fd);
	}
	if (sema_channels != NULL) {
		sem_close(sema_channels);
	}
}

static void create_channel(int chid, int prev_free, const char *name, int msec_timeout, int maxnb_msg_buffered) {
	
	mpshm_channel_t *c = &shm_channels->channels[chid];
    strcpy(c->name, name);
    c->pid = getpid();
    c->prev_free = prev_free;
}

int mpshm_channel_create(const char *name, int msec_timeout, int maxnb_msg_buffered) {

	int chid = -1;

	sem_wait(sema_channels);

	DEBUG("Create channel [%s]", name);
    if (shm_channels->last_free == -1) {
        chid = shm_channels->nb_channels++;
    	create_channel(chid, -1, name, msec_timeout, maxnb_msg_buffered);
    }
    else {
        assert(0);
    }

	sem_post(sema_channels);
mpshm_show();    
	return chid;
}

void mpshm_show(void) {
    

	sem_wait(sema_channels);

    printf("%d channel%s\n", shm_channels->nb_channels, (shm_channels->nb_channels > 1) ? "s" : "");
    for (int n = 0; n < MPSHM_MAXNB_CHANNELS; n++) {
	    mpshm_channel_t *c = &shm_channels->channels[n];
        if (strcmp(c->name, "")) {
            printf("%3d: %*s %6d\n", n, MPSHM_MAXLEN_CHANNEL_NAME, c->name, c->pid);
        }
    }

	sem_post(sema_channels);
}
