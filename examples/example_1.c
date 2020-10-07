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

#include "../libmpshm/log.h"

#include "../libmpshm/mpshm.h"

static void *thread1(void *arg) {
	char recv_buffer[100];
	
    int chid1 = mpshm_channel_create("channel_1", 1000, 0);
    DEBUG("chid1=%d", chid1);
    assert(chid1 >= 0);
	memset(recv_buffer, 0, sizeof(recv_buffer));

	int s = mpshm_receive(chid1, recv_buffer, sizeof(recv_buffer), 1000);
	DEBUG("@@@ s=%d %d [%s]", s, strlen(recv_buffer), recv_buffer);
	assert(!strcmp(recv_buffer, MSG_SENT_1));

}

static void *thread2(void *arg) {
	char reply_buffer[50];

	int chid1;
	for (;;) {
		chid1 = mpshm_channel_connect("channel_1", 1000);
		if (chid1 != -1)
			break;
		sleep(1);
	}
	DEBUG("chid1=%d", chid1);

}

int main(int argc, char *argv[]) {

	pthread_t tid1, tid2;
	pthread_attr_t tattr1, tattr2;

	pthread_attr_init(&tattr1);
	pthread_create(&tid1, &tattr1, &thread1, NULL);
	
	pthread_attr_init(&tattr2);
	pthread_create(&tid2, &tattr2, &thread2, NULL);
	
	printf("Press <CR> to stop");
	getchar();

	return 0;
}
