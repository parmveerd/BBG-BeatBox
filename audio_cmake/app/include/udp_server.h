
#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <pthread.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sys/sysinfo.h>

int udp_init(int*);
void* udp_check();
char* update_time();
int rec_init();
void udp_cleanup();


#endif