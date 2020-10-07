/*  
 *
 *  
 *  
 *  
 */

#ifndef __LIBMPSHM_H__
#define __LIBMPSHM_H__

typedef struct {
	char    name[MPSHM_MAXLEN_CHANNEL_NAME+1];  ///< Channel name
    pid_t   pid;                                ///< Channel owner pid
    int     prev_free;                          ///< Previous free channel (previously deleted) 
} mpshm_channel_t;

typedef struct {
	int 			nb_channels;						///< Number of channels defined
    int             last_free;                          ///< Last deleted entry, or -1 if none
	mpshm_channel_t channels[MPSHM_MAXNB_CHANNELS]; 	///< Channels definition
} mpshm_channels_t;

#endif /* __LIBMPSHM_H__ */
