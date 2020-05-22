#ifndef _AIR_KISS_CONNECT_H_
#define _AIR_KISS_CONNECT_H_

void air_kiss_connect();

extern void (*signal_on)(void);
extern void (*signal_off)(void);
extern void (*signal_toggle)(void);

#endif
