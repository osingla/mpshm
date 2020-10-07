/*  
 *
 *  
 *  
 *  
 */

#ifndef __MPSHM_H__
#define __MPSHM_H__

#define MPSHM_MAXNB_CHANNELS		512
#define MPSHM_MAXLEN_CHANNEL_NAME	31

int mpshm_channel_create(const char *name, int msec_timeout, int maxnb_msg_buffered);

#endif /* __MPSHM_H__ */
